#include "Spliter.h"
#define CLASS Spliter

extern Setting setting;
#include "Database.h"
extern Database db;

#include "Calculator.h" 
#include "StructuresGspan.h"

void CLASS::initMinScore() {
	parent_score = Calculator::imp(db.ys, Calculator::trainOnly(targets));
	min_score = parent_score - setting.needed_impurity_decrease - std::numeric_limits<double>::epsilon();
}

void CLASS::prepare(const vector<ID>& _targets) {
	// std::cout << "debug Spliter prepare" << std::endl; // debug
	targets = _targets;
	initMinScore();
	// Debug::IDs(targets); // debug
	db.gspan.setSpliterPtr(this);
	db.gspan.minsup = setting.minsup;
	db.gspan.maxpat = setting.maxpat;
	db.gspan.run();
	std::cout << "prepare cache size: " << cache.size() << std::endl;
}

vector<ID> CLASS::run(const vector<ID>& _targets) {
	// std::cout << "debug spliter run" << std::endl; // debug
	TimeStart();
	std::ofstream file;
	file.open("search.dat", std::ios::out);
	targets = _targets;
	best_pattern = {};
	initMinScore();
	if (min_score < 0) {
		goto G_INVALID;
	}
	search();
	// std::cout << "debug best_pattern " << best_pattern << std::endl; // debug
	if (best_pattern.size() == 0) {
G_INVALID:
		valid_flg = false;
		return {};
	}
	// std::cout << "debug parent_score " << parent_score << " min_score " << min_score << std::endl; // debug
	valid_flg = true;
	vector<ID> posi = db.gspan.getPosiIds(cache.at(best_pattern).g2tracers);
	return Calculator::setIntersec(targets, posi);
}

void CLASS::search() {
	// std::cout << "serch Cache" << std::endl;
	for (auto& pattern : e1patterns) {
		const auto& g2tracers = cache[pattern].g2tracers;
		vector<ID> posi = db.gspan.getPosiIds(g2tracers);
		update(pattern, posi);
		if (isBounded(posi)) {
			// cout << "Prune one edge pattern" << endl;
			continue;
		}
		if (cache[pattern].scan) {
			search_childs(pattern);
		} else {
			db.gspan.run(pattern);
		}
	}
}

void CLASS::search_childs(const Pattern& pattern) {
	Pattern child = pattern;
	for (auto& child_dfs : cache[pattern].childs) {
		child.push_back(child_dfs);
		const auto& g2tracers = cache[child].g2tracers;
		vector<ID> posi = db.gspan.getPosiIds(g2tracers);
		update(child, posi);
		if (isBounded(posi)) {
			// cout << "Prune some edge pattern" << endl;
			child.pop_back();
			continue;
		}
		if (cache[child].scan) {
			search_childs(child);
		} else {
			db.gspan.run(child);
		}
		child.pop_back();
	}
}

void CLASS::update(Pattern pattern, vector<ID> posi) {
	double score = Calculator::score(db.ys, targets, posi);
	if (score < min_score ) { // old pattern may be used (this func is called from gspan)
		min_score = score;
		best_pattern = pattern;
		clock_t time = clock() - search_start;
		int gain_count = db.gradient_boosting.getGainCount();
		std::ofstream file;
		file.open("search.dat", std::ios::app);
		file << double(time) / CLOCKS_PER_SEC << "\t" << gain_count << "\t" << min_score / targets.size() << std::endl;
	}
}

bool CLASS::isBounded(vector<ID> posi) {
	double min_bound = Calculator::bound(db.ys, targets, posi);
	if (min_score <= min_bound) {
		return true;
	} else {
		return false;
	}
}

