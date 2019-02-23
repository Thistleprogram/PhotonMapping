#ifndef _SEARCHNEAR_H_
#define _SEARCHNEAR_H_

#include "trace.h"
#include "Construct.h"
#include <omp.h>

Color Search(int pos, Vec position, double d2) {
	Color ret = Color(0, 0, 0);

	double dis2 = 0;
	if (Kdtree[pos].dir == dirX) {
		dis2 = position.x - Kdtree[pos].photon.position.x;
	}
	if (Kdtree[pos].dir == dirY) {
		dis2 = position.y - Kdtree[pos].photon.position.y;
	}
	if (Kdtree[pos].dir == dirZ) {
		dis2 = position.z - Kdtree[pos].photon.position.z;
	}
	if (IsNode[pos * 2 + 1]) {
		if (dis2 <= 0) {
			ret = ret + Search(2 * pos + 1, position, d2);
			if (IsNode[pos * 2 + 2]) {
				if (dis2*dis2 < d2*d2) {
					ret = ret + Search(2 * pos + 2, position, d2);
				}
			}
		}
	}
	if (IsNode[pos * 2 + 2]) {
		if (dis2 > 0) {
			ret = ret + Search(2 * pos + 2, position, d2);
			if (dis2*dis2 < d2*d2) {
				ret = ret + Search(2 * pos + 1, position, d2);
			}
		}
	}
	Vec pho = Kdtree[pos].photon.position;
	double dis = (position - pho).length_squared();

	if (dis < d2*d2) {
		ret = ret + Kdtree[pos].photon.color;
	}
	return ret;
}

#endif