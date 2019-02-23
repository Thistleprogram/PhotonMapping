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
	for (int i = 0; i < HMEmission; i++) {//発光体ごとにばらまく
		Sphere Emitter = spheres[Light[i]];
		for (int j = 0; j < PerThing; j++) {//一つの発光体につきばらまく
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
			if (fabs(w.x) > EPS) // ベクトルwと直交するベクトルを作る。w.xが0に近い場合とそうでない場合とで使うベクトルを変える。
				u = normalize(cross(Vec(0.0, 1.0, 0.0), w));
			else
				u = normalize(cross(Vec(1.0, 0.0, 0.0), w));
			v = cross(w, u); 
			// コサイン項を使った重点的サンプリング
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