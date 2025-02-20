#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"
#include <random>

struct Dice {

	static std::random_device seed_gen;
	static std::mt19937 mt;
	static std::uniform_real_distribution<> dice;

	static bool p(double t) {
		assert(0 <= t and t <= 1);
		if (dice(mt) < t) {
			return true;
		} else {
			return false;
		}
	}

	static ID id(int size) {
		if (size == 1) {
			return 0;
		}
		std::uniform_int_distribution<> dice_i(0, size-1);
		return dice_i(mt);
	}

	static vector<ID> shuffle(int size) {
		vector<ID> v(size);
		std::iota(v.begin(), v.end(), 0);
		std::shuffle(v.begin(), v.end(), mt);
		return v;
	}

	static vector<ID> shuffle_ids(vector<ID> v) {
		std::shuffle(v.begin(), v.end(), mt);
		return v;
	}

	static Tracers shuffle_tracers(Tracers tracers) {
		std::shuffle(tracers.begin(), tracers.end(), mt);
		return tracers;
	}
};
