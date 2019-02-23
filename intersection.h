#ifndef	_INTERSECTION_H_
#define	_INTERSECTION_H_

#include "vec.h"
#include "constant.h"
struct Hitpoint {
	double distance;//���C�̎n�_��������_�܂ł̋���
	Vec normal;//�@��
	Vec position;//�Փ˓_

	Hitpoint() : distance(INF), normal(), position() {}
};

struct Intersection {
	Hitpoint hitpoint;
	int object_id;//�Ԃ��������̂̔ԍ�

	Intersection() : object_id(-1) {}
};

#endif