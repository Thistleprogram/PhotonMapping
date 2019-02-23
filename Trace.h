#ifndef	_Trace_H_
#define	_Trace_H_

#include<algorithm>
#include<vector>
#include "Constant.h"
#include "intersection.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "vec.h"
#include <omp.h>
using namespace std;

struct Photon {
	Ray photon;
	Color color;
	Photon(const Ray &position, const Color &color) :
		photon(position), color(color) {}
};

struct Memory {
	Vec position;
	Color color;
	Memory(const Vec &position = Vec(), const Color &color = Color()) :
		position(position), color(color) {}
};

Ray Bounce(Ray from, Hitpoint hit) {
	return Ray(hit.position, from.dir - hit.normal * 2.0 * dot(hit.normal, from.dir));
}

int already;//���łɂ��܂�ꂽ��
vector<Memory>photons;

const int TraceLimit = 40;
const int MinTrace = 10;
const int FinalDepth = 100;
int counting = 0;

void trace(Photon from, Random* rand, const int depth) {

	Intersection intersection;
	intersect_scene(from.photon, &intersection);
	if (intersection.object_id == -1) return;

	Color incoming;
	Color weight = 1.0;
	Sphere item = spheres[intersection.object_id];
	const Vec orienting_normal = dot(intersection.hitpoint.normal, from.photon.dir) < 0.0 ? intersection.hitpoint.normal : (-1.0 * intersection.hitpoint.normal); // �����ʒu�̖@���i���̂���̃��C�̓��o���l���j

	double russianProbability = std::max(item.color.x, std::max(item.color.y, item.color.z));
	if (depth > TraceLimit) {
		russianProbability *= pow(0.5, depth - TraceLimit);
	}
	if (depth > MinTrace) {
		if (rand->next01() >= russianProbability) {
			return;
		}
	}
	else {
		russianProbability = 1.0;
	}
	if (item.reflection_type == Mirror) {
		Ray going = Bounce(from.photon, intersection.hitpoint);
		trace(Photon(going, multiply(from.color, item.color) / russianProbability), rand, depth + 1);
		return;
	}

	else if (item.reflection_type == Refraction) {
		const Ray reflection_ray = Bounce(from.photon, intersection.hitpoint);
		const bool into = dot(intersection.hitpoint.normal, orienting_normal) > 0.0; // ���C���I�u�W�F�N�g����o��̂��A����̂�

																					 // Snell�̖@��
		const double nc = 1.0; // �^��̋��ܗ�
		const double nt = kIor; // �I�u�W�F�N�g�̋��ܗ�
		const double nnt = into ? nc / nt : nt / nc;
		const double ddn = dot(from.photon.dir, orienting_normal);
		const double cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn);

		if (cos2t < 0.0) { // �S����
			Ray going = Bounce(from.photon, intersection.hitpoint);
			trace(Photon(going, multiply(from.color, item.color) / russianProbability), rand, depth + 1);
			return;
		}
		const Ray refraction_ray = Ray(intersection.hitpoint.position,
			normalize(from.photon.dir * nnt - intersection.hitpoint.normal * (into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t))));

		// Schlick�ɂ��Fresnel�̔��ˌW���̋ߎ����g��
		const double a = nt - nc, b = nt + nc;
		const double R0 = (a * a) / (b * b);

		const double c = 1.0 - (into ? -ddn : dot(refraction_ray.dir, -1.0 * orienting_normal));
		const double Re = R0 + (1.0 - R0) * pow(c, 5.0); // ���˕����̌������˂���ray.dir�̕����ɉ^�Ԋ����B�����ɋ��ܕ����̌������˂�������ɉ^�Ԋ����B
		const double Tr = (1.0 - Re); // ���ܕ����̌������܂���ray.dir�̕����ɉ^�Ԋ���

		const double probability = 0.25 + 0.5*Re;
		if (rand->next01() < probability) {
			trace(Photon(reflection_ray, multiply(from.color, item.color)*Re / (probability* russianProbability)), rand, depth + 1);
			return;
		}
		else {
			trace(Photon(refraction_ray, multiply(from.color, item.color)*Tr / ((1.0 - probability)*russianProbability)), rand, depth + 1);
			return;
		}
	}

	else {
		Vec w, u, v;
		w = orienting_normal;
		if (fabs(w.x) > EPS) // �x�N�g��w�ƒ�������x�N�g�������Bw.x��0�ɋ߂��ꍇ�Ƃ����łȂ��ꍇ�ƂŎg���x�N�g����ς���B
			u = normalize(cross(Vec(0.0, 1.0, 0.0), w));
		else
			u = normalize(cross(Vec(1.0, 0.0, 0.0), w));
		v = cross(w, u);
		const double r1 = 2 * PI*rand->next01();
		const double r2 = rand->next01(), r2s = sqrt(r2);
		Vec dir = normalize((
			u*cos(r1)*r2s +
			v * sin(r1)*r2s +
			w * sqrt(1.0 - r2)
			));
		photons.push_back(Memory{ intersection.hitpoint.position,from.color });
		trace(Photon(Ray(intersection.hitpoint.position, dir), multiply(from.color, item.color) / russianProbability), rand, depth + 1);
		return;
	}
}

#endif