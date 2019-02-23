#ifndef	_INTERSECTION_H_
#define	_INTERSECTION_H_

#include "vec.h"
#include "constant.h"
struct Hitpoint {
	double distance;//レイの始点から交差点までの距離
	Vec normal;//法線
	Vec position;//衝突点

	Hitpoint() : distance(INF), normal(), position() {}
};

struct Intersection {
	Hitpoint hitpoint;
	int object_id;//ぶつかったものの番号

	Intersection() : object_id(-1) {}
};

#endif