#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"
#include "Spliter.h"
#include "IsMin.h"

struct CacheRecord {
	GraphToTracers g2tracers;
	vector<Pattern> childs;
	bool terminal;
	size_t count;
	double sum_score;
	double feature_importance;
	bool scan;
	bool complete;
	CacheRecord() {
		terminal = true;
		count = 0;
		sum_score = 0;
		feature_importance = 0;
		scan = false;
		complete = false;
	}
	CacheRecord(vector<Pattern> childs)
		: childs(childs){
			terminal = true;
			count = 0;
			sum_score = 0;
			feature_importance = 0;
			scan = false;
			complete = false;
		}
	CacheRecord(GraphToTracers g2tracers, vector<Pattern> childs)
		: g2tracers(g2tracers), childs(childs){
			terminal = true;
			count = 0;
			sum_score = 0;
			feature_importance = 0;
			scan = false;
			complete = false;
		}
};

class Gspan {

	public:
		size_t minsup;
		size_t maxpat;

		bool scanGspan(const Pattern&);
		void makeRoot(const vector<ID>&);
		void one_edge_report(GraphToTracers& g2tracers);

		inline map<Pattern, CacheRecord>& getCache() {
			return cache;
		}

		inline const Pattern& getRoot() {
			return root;
		}

		inline void updateFeatureImportance(const Pattern& pattern, double importance) {
			cache.at(pattern).feature_importance += importance;
		}

		inline vector<ID> getPosiIds(const GraphToTracers& tracers) {
			vector<ID> vec(tracers.size());
			size_t i = 0;
			for (auto x : tracers) {
				vec[i] = x.first;
				i++;
			}
			return vec;
		}

		void setSpliterPtr(Spliter* ptr) {
			spliter = ptr;
		}

		inline void setMinsup(size_t _minsup) {
			minsup = _minsup;
		}

		inline void setMaxpat(size_t _maxpat) {
			maxpat = _maxpat;
		}

		inline void clearUCB() {
			for (auto& x : cache) {
				x.second.terminal = true;
				x.second.count = 0;
				x.second.sum_score = 0;
				x.second.complete = false;
			}
			cache[root].terminal = false;
		}

	private:
		Spliter* spliter;
		Pattern root;
		IsMin is_min;
		map<Pattern, CacheRecord> cache; // inserted data must keep pointer

		size_t support(GraphToTracers& g2tracers);
		bool check_pattern(Pattern pattern, GraphToTracers& g2tracers);

};
