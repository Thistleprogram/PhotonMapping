#ifndef	_SPHERE_H_
#define	_SPHERE_H_

#include "Constant.h"
#include "intersection.h"
#include "material.h"
#include "Vec.h"
#include <omp.h>

int almade;
struct Sphere {
	double radius;
	Vec pos;
	Color emission;
	Color color;
	RefType reflection_type;
	int id;

	Sphere(const double radius, const Vec &pos, const Color &emission, const Color &color, const RefType reflection_type) :
		radius(radius), pos(pos), emission(emission), color(color), reflection_type(reflection_type), id(almade++) {}

	// 入力のrayに対する交差点までの距離を返す。交差しなかったら0を返す。
	// rayとの交差判定を行う。交差したらtrue,さもなくばfalseを返す。
	bool intersect(const Ray &ray, Hitpoint *hitpoint) const {
		const Vec p_o = pos - ray.org;
		const double b = dot(p_o, ray.dir);
		const double D4 = b * b - dot(p_o, p_o) + radius * radius;

		if (D4 < 0.0)
			return false;

		const double sqrt_D4 = sqrt(D4);
		const double t1 = b - sqrt_D4, t2 = b + sqrt_D4;

		if (t1 < EPS && t2 < EPS)
			return false;

		if (t1 > EPS) {
			hitpoint->distance = t1;
		}
		else {
			hitpoint->distance = t2;
		}

		hitpoint->position = ray.org + hitpoint->distance * ray.dir;
		hitpoint->normal = normalize(hitpoint->position - pos);
		return true;
	}
};

#endif