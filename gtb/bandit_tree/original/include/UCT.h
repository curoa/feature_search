#pragma once

#include "MyInclude.h"
#include "Gspan.h"
#include "StructuresGspan.h"

class UCT {
	public:
		UCT(map<Pattern, CacheRecord>& _cache, const Pattern& _root) : cache(_cache), root(_root) {}
		void run(const vector<ID>& _targets);

	private:
		vector<ID> targets;
		map<Pattern, CacheRecord>& cache;
		const Pattern& root;
		vector<Pattern> path;

		bool selection(const Pattern&);
		bool update(const Pattern&);
		bool expansion();
		bool expand_selection(const Pattern&);
		Pattern simulation(const Pattern&, const size_t);
		bool stop_condition(const Pattern, const size_t);
		void backpropagation(double);
};
