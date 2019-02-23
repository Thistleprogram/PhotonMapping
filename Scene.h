#ifndef	_SCENE_H_
#define	_SCENE_H_


#include "Constant.h"
#include "intersection.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "Sphere.h"
#include "Vec.h"
#include <omp.h>

// レンダリングするシーンデータ
const Sphere spheres[] = {
	Sphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Color(), Color(0.4, 0.1, 0.15), Lembert), // 左
	Sphere(1e5, Vec(-1e5 + 99, 40.8, 81.6),Color(), Color(0.05, 0.05, 0.4), Lembert), // 右
	Sphere(1e5, Vec(50, 40.8, 1e5), Color(),Color(0.4, 0.4, 0.4), Lembert), // 奥
	Sphere(1e5, Vec(50, 40.8, -1e5 + 250), Color(), Color(),Lembert), // 手前
	Sphere(1e5, Vec(50, 1e5, 81.6), Color(), Color(0.4, 0.4, 0.4), Lembert), // 床
	Sphere(1e5, Vec(50, -1e5 + 81.6, 81.6),Color(), Color(0.4, 0.4, 0.4), Lembert), // 天井
	Sphere(20,Vec(65, 20, 20), Color(),Color(0, 0.4, 0), Lembert), // 緑球
	Sphere(16.5,Vec(27, 16.5, 47), Color(),  Color(0.99, 0.99, 0.99), Mirror), // 鏡
	Sphere(16.5,Vec(77, 16.5, 78), Color(),Color(0.99, 0.99, 0.99), Refraction), //ガラス
	Sphere(5, Vec(50.0, 50.0, 50) ,Color(45000,45000,45000), Color(0.5,0.5,0.5), Lembert), //照明
};

const int HMEmission = 1;
const int Light[] = {
	9,
};
// シーンとの交差判定関数
inline bool intersect_scene(const Ray &ray, Intersection *intersection) {
	const double n = sizeof(spheres) / sizeof(Sphere);

	// 初期化
	intersection->hitpoint.distance = INF;
	intersection->object_id = -1;

	// 線形探索
	for (int i = 0; i < int(n); i++) {
		Hitpoint hitpoint;
		if (spheres[i].intersect(ray, &hitpoint)) {
			if (hitpoint.distance < intersection->hitpoint.distance) {
				intersection->hitpoint = hitpoint;
				intersection->object_id = i;
			}
		}
	}

	return (intersection->object_id != -1);
}

#endif