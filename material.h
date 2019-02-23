#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vec.h"
typedef Vec Color;

enum RefType {
	Lembert,	// 完全拡散面。いわゆるLambertian面。
	Mirror,	// 理想的な鏡面。
	Refraction,	// 理想的なガラス的物質。
};

const double kIor = 1.5; // 屈折率(Index of refraction)

#endif