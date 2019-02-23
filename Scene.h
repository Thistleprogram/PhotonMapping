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

// �����_�����O����V�[���f�[�^
const Sphere spheres[] = {
	Sphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Color(), Color(0.4, 0.1, 0.15), Lembert), // ��
	Sphere(1e5, Vec(-1e5 + 99, 40.8, 81.6),Color(), Color(0.05, 0.05, 0.4), Lembert), // �E
	Sphere(1e5, Vec(50, 40.8, 1e5), Color(),Color(0.4, 0.4, 0.4), Lembert), // ��
	Sphere(1e5, Vec(50, 40.8, -1e5 + 250), Color(), Color(),Lembert), // ��O
	Sphere(1e5, Vec(50, 1e5, 81.6), Color(), Color(0.4, 0.4, 0.4), Lembert), // ��
	Sphere(1e5, Vec(50, -1e5 + 81.6, 81.6),Color(), Color(0.4, 0.4, 0.4), Lembert), // �V��
	Sphere(20,Vec(65, 20, 20), Color(),Color(0, 0.4, 0), Lembert), // �΋�
	Sphere(16.5,Vec(27, 16.5, 47), Color(),  Color(0.99, 0.99, 0.99), Mirror), // ��
	Sphere(16.5,Vec(77, 16.5, 78), Color(),Color(0.99, 0.99, 0.99), Refraction), //�K���X
	Sphere(5, Vec(50.0, 50.0, 50) ,Color(45000,45000,45000), Color(0.5,0.5,0.5), Lembert), //�Ɩ�
};

const int HMEmission = 1;
const int Light[] = {
	9,
};
// �V�[���Ƃ̌�������֐�
inline bool intersect_scene(const Ray &ray, Intersection *intersection) {
	const double n = sizeof(spheres) / sizeof(Sphere);

	// ������
	intersection->hitpoint.distance = INF;
	intersection->object_id = -1;

	// ���`�T��
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