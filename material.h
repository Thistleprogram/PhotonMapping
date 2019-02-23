#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vec.h"
typedef Vec Color;

enum RefType {
	Lembert,	// ���S�g�U�ʁB������Lambertian�ʁB
	Mirror,	// ���z�I�ȋ��ʁB
	Refraction,	// ���z�I�ȃK���X�I�����B
};

const double kIor = 1.5; // ���ܗ�(Index of refraction)

#endif