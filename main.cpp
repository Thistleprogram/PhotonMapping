#include <iostream>

#include "PhotonMap.h"
#include "render.h"
#include "random.h"
#include "material.h"
#include "Vec.h"
#include "Construct.h"
#include "Trace.h"
#include <time.h>

int main(int argc, char **argv) {
	std::cout << "Path tracing renderer: photon mapping" << std::endl << std::endl;
	time_t tim = time(NULL);
	struct tm jikan;
	localtime_s(&jikan, &tim);
	Random rand(jikan.tm_sec*jikan.tm_min);
	MakeMap(100000,&rand);
	already = photons.size();
	MakeKD();
	// 640x480‚Ì‰æ‘œA(2x2) * 4 sample / pixel
	
	render(640, 480, 8, 3, 2.5, &rand);

}