#ifndef	_PHOTONMAP_H_
#define	_PHOTONMAP_H_

#include <omp.h>
#include<algorithm>
#include <iostream>
#include <time.h>
#include "Constant.h"
#include "intersection.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vec.h"
#include "Trace.h"

void MakeMap(int PerThing,Random *rnd) {

	// OpenMP
//#pragma omp parallel for schedule(dynamic, 1) num_threads(4)
	for (int i = 0; i < HMEmission; i++) {//�����̂��Ƃɂ΂�܂�
		Sphere Emitter = spheres[Light[i]];
		for (int j = 0; j < PerThing; j++) {//��̔����̂ɂ��΂�܂�
			Vec from;
			double x, y, z;
			while (true) {
				x = rnd->next01() * 30 + 35;
				y = rnd->next01() * 30 + 35;
				z = rnd->next01() * 30 + 35;
				if ((Vec(x, y, z) - Emitter.pos).length() < Emitter.radius) {
					from = Emitter.pos + normalize(Vec(x, y, z) - Emitter.pos)*Emitter.radius;
					break;
				}
			}
			Vec w, u, v;
			Vec orienting_normal = normalize(from - Emitter.pos);
			w = orienting_normal;
			if (fabs(w.x) > EPS) // �x�N�g��w�ƒ�������x�N�g�������Bw.x��0�ɋ߂��ꍇ�Ƃ����łȂ��ꍇ�ƂŎg���x�N�g����ς���B
				u = normalize(cross(Vec(0.0, 1.0, 0.0), w));
			else
				u = normalize(cross(Vec(1.0, 0.0, 0.0), w));
			v = cross(w, u); 
			// �R�T�C�������g�����d�_�I�T���v�����O
			const double r1 = 2 * PI * rnd->next01();
			const double r2 = rnd->next01(), r2s = sqrt(r2);
			Vec dir = normalize((
				u * cos(r1) * r2s +
				v * sin(r1) * r2s +
				w * sqrt(1.0 - r2)));

			Ray going = Ray(from, dir);	
			trace(Photon(going, Emitter.emission / PerThing), rnd, 0);
		}
	}
}

#endif