#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"

struct CacheRecord {
	GraphToTracers g2tracers;
	vector<DFSCode> childs;
	double feature_importance;
	bool scan;
	CacheRecord() {
		feature_importance = 0;
		scan = false;
	}
	CacheRecord(GraphToTracers g2tracers, vector<DFSCode> childs)
		: g2tracers(g2tracers), childs(childs){
			feature_importance = 0;
			scan = false;
		}
};

class Spliter {
	public:
		void prepare(const vector<ID>& _targets);
		vector<ID> run(const vector<ID>& _targets);
		void update(Pattern pattern, vector<ID> posi);
		void push_pq_bound(double bound, Pattern pattern);
		inline bool valid() {
			return valid_flg;
		}
		inline const Pattern& getBestPattern() {
			assert(valid_flg);
			return best_pattern;
		}
		inline double getImportance() {
			assert(valid_flg);
			return parent_score - min_score;
		}
		inline vector<ID> gettargets() {
			return  targets;
		}
		inline vector<Pattern>& gete1Patterns() {
			return e1patterns;
		}

		inline map<Pattern, CacheRecord>& getCache() {
			return cache;
		}
		void TimeStart() {
			search_start = clock();
		}


	private:
		bool valid_flg;
		vector<ID> targets;
		double parent_score;
		double min_score;
		Pattern best_pattern;
		map<Pattern, CacheRecord> cache;
		vector<Pattern> e1patterns;
		priority_queue<std::pair<double, Pattern>, vector<std::pair<double, Pattern>>, std::greater<std::pair<double, Pattern>>> pq_bound;
		clock_t search_start;

		void initMinScore();
		void search();
};

