#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include<vector>
#include<algorithm>
#include<queue>
#include "Trace.h"
#include "Vec.h"
#include "material.h"
#include <omp.h>

enum Which {
	dirX, dirY, dirZ, kyomu,
};
struct MK {
	int node;
	vector<Memory>many;//場所、色
};
struct KD {
	Memory photon;
	Which dir;
};

//左側のやつ、右側のやつ
vector<double>ve;
KD Kdtree[5000000];
bool IsNode[5000000];
vector<MK>que;
vector<Memory>down, up;

void construct();

void MakeKD() {
	que.push_back(MK{ 0,photons });
	construct();
}
int i;
void construct() {
	counting++;
	MK now = que.back(); que.pop_back();
	double maxx = -INF, maxy = -INF, maxz = -INF, minx = INF, miny = INF, minz = INF;
	if (now.many.size() == 0) {
		return;
	}
	for (i = 0; i < (int)now.many.size(); i++) {
		maxx = max(maxx, now.many[i].position.x);
		maxy = max(maxy, now.many[i].position.y);
		maxz = max(maxz, now.many[i].position.z);
		minx = min(minx, now.many[i].position.x);
		miny = min(miny, now.many[i].position.y);
		minz = min(minz, now.many[i].position.z);
	}
	double ma = max(maxx - minx, max(maxy - miny, maxz - minz));
	double med;
	int mednum;
	bool l = 1;
	int sum = 0;
	ve.clear();
	up.clear(); down.clear();
	if (maxx - minx == ma) {
		for (i = 0; i < (int)now.many.size(); i++) {
			ve.push_back(now.many[i].position.x);
		}
		sort(ve.begin(), ve.end());
		med = ve[ve.size() / 2];
		for (i = 0; i < (int)now.many.size(); i++) {
			if (now.many[i].position.x == med && l) {
				l = 0;
				mednum = i;
			}
			else  if (now.many[i].position.x <= med) {
				down.push_back(now.many[i]);
			}
			else {
				up.push_back(now.many[i]);
			}
		}
		Kdtree[now.node] = KD{ now.many[mednum],dirX };
	}
	else if (maxy - miny == ma) {
		for (int i = 0; i < (int)now.many.size(); i++) {
			ve.push_back(now.many[i].position.y);
		}
		sort(ve.begin(), ve.end());
		med = ve[ve.size() / 2];
		for (i = 0; i < (int)now.many.size(); i++) {
			if (now.many[i].position.y == med && l) {
				l = 0;
				mednum = i;
			}
			else  if (now.many[i].position.y <= med) {
				down.push_back(now.many[i]);
			}
			else {
				up.push_back(now.many[i]);
			}
		}
		Kdtree[now.node] = KD{ now.many[mednum],dirY };
	}
	else {
		for (i = 0; i < (int)now.many.size(); i++) {
			ve.push_back(now.many[i].position.z);
		}
		sort(ve.begin(), ve.end());
		med = ve[ve.size() / 2];
		for (i = 0; i < (int)now.many.size(); i++) {
			if (now.many[i].position.z == med && l) {
				l = 0;
				mednum = i;
			}
			else  if (now.many[i].position.z <= med) {
				down.push_back(now.many[i]);
			}
			else {
				up.push_back(now.many[i]);
			}
		}
		Kdtree[now.node] = KD{ now.many[mednum],dirZ };
	}
	IsNode[now.node] = 1;
	int ups = up.size(), dos = down.size();
	if (up.size()) {
		que.push_back(MK{ now.node * 2 + 2,up });
	}
	if (down.size()) {
		que.push_back(MK{ now.node * 2 + 1,down });
	}
	if (dos) {
		construct();
	}
	if (ups) {
		construct();
	}
}

#endif