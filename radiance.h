#ifndef	_RADIANCE_H_
#define	_RADIANCE_H_

#include<algorithm>
#include "Constant.h"
#include "intersection.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vec.h"
#include "SearchNear.h"
#include "PhotonMap.h"
#include "Trace.h"
#include <omp.h>

const Color BackgroundColor = Color(0.0, 0.0, 0.0);
const int DepthLimit = 20;//最高でここまで反射を繰り替えす
const int LemberDepth = 1;//拡散反射は一回まで
const int MinDepth = 5;


Color radiance(Ray from, Random* rand, const int depth, const int Lemb, const double d) {

	Intersection intersection;
	if (!intersect_scene(from, &intersection)) return BackgroundColor;

	Color Direct;
	Color incoming;
	Color weight = 1.0;
	Sphere item = spheres[intersection.object_id];
	const Vec orienting_normal = dot(intersection.hitpoint.normal, from.dir) < 0.0 ? intersection.hitpoint.normal : (-1.0 * intersection.hitpoint.normal); // 交差位置の法線（物体からのレイの入出を考慮）

	if (item.emission.x + item.emission.y + item.emission.z > 0&&Lemb==0) {
		Direct = multiply(item.emission, item.color);
	}
	double russianProbability = std::max(item.color.x, std::max(item.color.y, item.color.z));
	if (depth > DepthLimit) {
		russianProbability *= pow(0.5, depth - DepthLimit);
	}
	if (depth > MinDepth) {
		if (rand->next01() >= russianProbability) {
			return Direct + multiply((Search(0, intersection.hitpoint.position, d) / d / d / PI), item.color);
		}
	}
	else {
		russianProbability = 1.0;
	}
	if (item.reflection_type == Mirror) {
		Ray going = Bounce(from, intersection.hitpoint);
		incoming = radiance(going, rand, depth + 1, Lemb, d);
		weight = item.color / russianProbability;
	}

	else if (item.reflection_type == Refraction) {
		const Ray reflection_ray = Bounce(from, intersection.hitpoint);
		const bool into = dot(intersection.hitpoint.normal, orienting_normal) > 0.0; // レイがオブジェクトから出るのか、入るのか

																					 // Snellの法則
		const double nc = 1.0; // 真空の屈折率
		const double nt = kIor; // オブジェクトの屈折率
		const double nnt = into ? nc / nt : nt / nc;
		const double ddn = dot(from.dir, orienting_normal);
		const double cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn);

		if (cos2t < 0.0) { // 全反射
			Ray going = Bounce(from, intersection.hitpoint);
			return radiance(going, rand, depth + 1, Lemb, d);
		}
		const Ray refraction_ray = Ray(intersection.hitpoint.position,
			normalize(from.dir * nnt - intersection.hitpoint.normal * (into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t))));

		// SchlickによるFresnelの反射係数の近似を使う
		const double a = nt - nc, b = nt + nc;
		const double R0 = (a * a) / (b * b);

		const double c = 1.0 - (into ? -ddn : dot(refraction_ray.dir, -1.0 * orienting_normal));
		const double Re = R0 + (1.0 - R0) * pow(c, 5.0); // 反射方向の光が反射してray.dirの方向に運ぶ割合。同時に屈折方向の光が反射する方向に運ぶ割合。
		const double nnt2 = pow(into ? nc / nt : nt / nc, 2.0); // レイの運ぶ放射輝度は屈折率の異なる物体間を移動するとき、屈折率の比の二乗の分だけ変化する。
		const double Tr = (1.0 - Re) * nnt2; // 屈折方向の光が屈折してray.dirの方向に運ぶ割合

		const double probability = 0.25 + 0.5*Re;
		if (depth > 2) {
			if (rand->next01() < probability) {
				incoming = radiance(refraction_ray, rand, depth + 1, Lemb, d)*Re;
				weight = item.color / (probability*russianProbability);
			}
			else {
				incoming = radiance(refraction_ray, rand, depth + 1, Lemb, d)*Tr;
				weight = item.color / ((1.0 - probability)*russianProbability);
			}
		}
		else { // 屈折と反射の両方を追跡
			incoming =
				radiance(reflection_ray, rand, depth + 1, Lemb, d) * Re +
				radiance(refraction_ray, rand, depth + 1, Lemb, d) * Tr;
			weight = item.color / (russianProbability);;
		}
	}

	else {
		Color Result = Search(0, intersection.hitpoint.position, d) / d / d / PI;
		if (Lemb >= LemberDepth) {
			incoming = Result;
			weight = item.color / russianProbability;
			goto loop;
		}
		Vec w, u, v;
		w = orienting_normal;
		if (fabs(w.x) > EPS) // ベクトルwと直交するベクトルを作る。w.xが0に近い場合とそうでない場合とで使うベクトルを変える。
			u = normalize(cross(Vec(0.0, 1.0, 0.0), w));
		else
			u = normalize(cross(Vec(1.0, 0.0, 0.0), w));
		v = cross(w, u);
		// コサイン項を使った重点的サンプリング
		const double r1 = 2 * PI * rand->next01();
		const double r2 = rand->next01(), r2s = sqrt(r2);
		Vec dir = normalize((
			u * cos(r1) * r2s +
			v * sin(r1) * r2s +
			w * sqrt(1.0 - r2)));
		incoming = radiance(Ray(intersection.hitpoint.position, dir), rand, depth + 1, Lemb + 1, d) + Result;
		weight = item.color / russianProbability;

	}
loop:;
	return Direct / russianProbability + multiply(weight, incoming);
}

#endif