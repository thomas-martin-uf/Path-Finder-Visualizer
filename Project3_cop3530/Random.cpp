#include "Random.h"
#include "ctime"

// static redeclaration
std::mt19937 Random::random(time(0));

// get random int
int Random::Int(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(random);
}

// get random float
float Random::Float(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random);
}