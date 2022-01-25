#include "Func.h"


int score(Photo* p1, Photo* p2) {
	int diff1 = 0;
	int diff2 = 0;
	int comm = 0;

	set<string> s;

	set_union(p1->tags.begin(), p1->tags.end(),
		p2->tags.begin(), p2->tags.end(),
		inserter(s, s.begin()));
	diff1 = s.size() - p2->get_numberOfTags();

	diff2 = s.size() - p1->get_numberOfTags();

	comm = p1->get_numberOfTags() - diff1;

	return min({ diff1, diff2, comm });
}

int total_score(vector<Photo*>& slideshow) {
	int points = 0;
	for (int i = 0; i < slideshow.size() - 1; i++) {
		points += score(slideshow[i], slideshow[i + 1]);
	}
	return points;
}


vector<Photo> random_vertical_pairs(vector<Photo>& photos, vector<int> vertical) {
	vector<Photo> res;
	if (vertical.size() < 2) return res;
	random_shuffle(vertical.begin(), vertical.end());
	for (int i = 0; i < vertical.size() - 1; i += 2) {
		res.push_back(Photo(&photos[vertical[i]], &photos[vertical[i + 1]]));
	}
	return res;
}

void remove_same_photos(vector<Photo*>& slideshow) {
	set<int> ids;
	for (int i = 0; i < slideshow.size(); ) {
		if (ids.find(slideshow[i]->get_id()) != ids.end()) {
			slideshow.erase(slideshow.begin() + i);
		}
		else {
			ids.insert(slideshow[i]->get_id());
			i++;
		}
	}
}

vector<Photo*> generate_random_solution(vector<Photo>& slides, int sol_size) {
	vector<int> idx(slides.size());
	iota(idx.begin(), idx.end(), 0);
	random_shuffle(idx.begin(), idx.end());
	idx.resize(sol_size);
	vector<Photo*> res;
	for (int i = 0; i < sol_size; i++) {
		res.push_back(&slides[idx[i]]);
	}
	return res;
}

int duel(vector<vector<Photo*>*>& parents) {
	int p1 = rand() % parents.size();
	int p2 = rand() % parents.size();
	return total_score(*(parents[p1])) > total_score(*(parents[p2])) ? p1 : p2;
}