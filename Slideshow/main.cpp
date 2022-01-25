#include<iostream>
#include<vector>
#include <fstream>
#include<string>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <queue>
using namespace std;


class Photo {
	
	int id;
	char type;
	int numberOfTags;
	set<string> tags;

public:
	static int count;
	static void reset_count(int c) {
		count = c;
	}
	friend int score(Photo* p1, Photo* p2);
	Photo(char tp, int nt, vector<string> tg) {
		count++;
		id = count;
		type = tp;
		numberOfTags = nt;
		for (int i = 0; i < numberOfTags; i++) {
			tags.insert(tg[i]);
		}
	}
	Photo(Photo* p1, Photo* p2) {
		count++;
		id = count;
		type = 'H';
		set_union(p1->tags.begin(), p1->tags.end(),
			p2->tags.begin(), p2->tags.end(),
			inserter(tags, tags.begin()));
		numberOfTags = tags.size();
	}
	int get_id() {
		return this->id;
	}
	char get_type() {
		return this->type;
	}
	int get_numberOfTags() {
		return this->numberOfTags;
	}
	set<string> get_tags() {
		return this->tags;
	}
};

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

	comm = p1->get_numberOfTags()-diff1;

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
	return total_score(*parents[p1]) > total_score(*parents[p2])? p1 : p2;
}
int Photo::count=0;

int main() {
	string line;
	ifstream file("c_memorable_moments.txt");
	vector<Photo> photos;
	vector<int> vertical_index;
	getline(file, line);
	int n = stoi(line);

	srand(unsigned(time(0)));

	while (getline(file, line)) {
		vector<string> tags;
		string::iterator it = line.begin();
		char t = *it;
		it += 2;
		size_t i = it - line.begin();
		size_t j= line.find(' ', i);
		int nTags = stoi(line.substr(i,j-i));
		for (int k = 0; k < nTags;k++) {
			i = line.find(' ', j);
			j = line.find(' ', i + 1);
			tags.push_back(line.substr(i + 1, j-i));
		}

		photos.push_back(Photo(t, nTags, tags));
		if ((photos.back()).get_type() == 'V') {
			vertical_index.push_back(int(photos.size() - 1));
		}
	}
	file.close();

	vector<Photo> slides;
	for (int i = 0; i < n; i++) {
		if (photos[i].get_type() == 'H') {
			slides.push_back(photos[i]);

		}
	}
	vector<Photo> vertical_slides = random_vertical_pairs(photos, vertical_index);
	for (int i = 0; i < vertical_slides.size(); i++) {
		slides.push_back(vertical_slides[i]);
	}

	vector<vector<Photo*>> population;
	int population_size = 10;
	int init_solution_size = 100;
	int generations_count = 100;
	int parents_count = int(population_size * 0.2);
	while (population.size() < population_size) {
		population.push_back(generate_random_solution(slides, init_solution_size));
	}

	vector<int> scores;
	int scores_sum = 0;
	for (int i = 0; i < population_size; i++) {
		scores.push_back(total_score(population[i]));
		scores_sum += scores[i];
	}

	for (int i = 0; i < scores.size(); i++) {
		cout << scores[i] << '\n';
	}
	
	for (int i = 0; i < generations_count; i++) {
		vector<vector<Photo*>*> parents;
		vector<double> F(population_size,0);
		vector<vector<Photo*>> new_generation(population_size, vector<Photo*>());
		F[0] = scores[0] / scores_sum;
		for (int j = 1; j < population_size; j++) {
			F[j] = F[j - 1] + scores[j] / scores_sum;
		}
		for (int j = 0; j < parents_count; j++) {
			double pr = rand();
			int p = 0;
			while (F[p] < pr) {
				p++;
			}
			parents.push_back(&population[p]);
		}
		for (int j = 0; j < population_size; j++) {
			int p1 = duel(parents);
			int p2 = duel(parents);
			int cross = rand() % min((*parents[p1]).size(), (*parents[p2]).size());
			for (int k = 0; k < cross; k++) {
				new_generation[j].push_back((*parents[p1])[k]);
			}
			for (int k = cross; k < (*parents[p2]).size(); k++) {
				new_generation[j].push_back((*parents[p2])[k]);
			}
			remove_same_photos(new_generation[j]);

		}
		

		

	}



}