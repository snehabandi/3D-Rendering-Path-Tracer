#ifndef RANDOM_FLOAT_H
#define RANDOM_FLOAT_H
#include <cstdlib>

inline float rand_float(){
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
#endif
