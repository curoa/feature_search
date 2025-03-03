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
		double time_threshold;

		void prepare(const vector<ID>& _targets);
		vector<ID> run(const vector<ID>& _targets, const size_t tree_count, size_t depth);
		void update(Pattern pattern, vector<ID> posi);
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
		void TimeStart(const size_t _tree_count, size_t _depth) {
			search_idx++;
			if (tree_count != _tree_count) {
				tree_count = _tree_count;
				search_idx = 0;
			}
			if (depth != _depth) {
				depth = _depth;
				search_idx = 0;
			}
			std::ostringstream oss;
			oss << "./search/tree" << tree_count << "depth" << depth << "_" << search_idx << ".dat";
			string filename = oss.str();
			std::ofstream file;
			file.open(filename, std::ios::out);
			search_start = clock();
		}
		void Log(int gain_count, double min_score, Pattern pattern) {
			clock_t time = clock() - search_start;
			std::ostringstream oss;
			oss << "./search/tree" << tree_count << "depth" << depth << "_" << search_idx << ".dat";
			string filename = oss.str();
			std::ofstream file;
			file.open(filename, std::ios::app);
			file << double(time) / CLOCKS_PER_SEC << "," << gain_count << "," << min_score / targets.size() << "," << pattern << std::endl;
		}
		bool TimeStop() {
			clock_t time = clock() - search_start;
			double time_s = double(time) / CLOCKS_PER_SEC;
			if (time_s > time_threshold) {
				return true;
			}
			return false;
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
		size_t tree_count;
		size_t depth;
		size_t search_idx = 0;
		clock_t search_start;

		void initMinScore();
		void search();
};

