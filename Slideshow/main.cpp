#include<iostream>
#include <fstream>
#include <ctime>
#include <numeric>
#include <queue>
#include <cctype>
#include "Photo.h"
using namespace std;




#include "Func.h"
#include "Photo.h"
int Photo::count=0;

int main() {
	string line;
	ifstream file("b_lovely_landscapes.txt");
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
			string s = line.substr(i + 1, j - i);
			s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
			tags.push_back(s);
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

	vector<vector<Photo*>>* population = new vector<vector<Photo*>>();
	int population_size = 1000;
	int init_solution_size = slides.size()/4;
	int generations_count = 1000;
	int parents_count = int(population_size * 0.2);
	for (int i = 0; i < population_size; i++) {
		(*population).push_back(generate_random_solution(slides, init_solution_size));
	}

	vector<int> scores(population_size,0);
	double scores_sum = 0;
	int max_score = 0;

	for (int i = 0; i < population_size; i++) {
		scores[i] = total_score((*population)[i]);
		scores_sum += scores[i];
		max_score = scores[i] > max_score ? scores[i] : max_score;
	}

	cout << max_score << "\n";
	
	for (int i = 0; i < generations_count; i++) {
		vector<vector<Photo*>>* new_generation = new vector<vector<Photo*>>();
		vector<vector<Photo*>*> parents;
		vector<double> F(population_size,0);
		
		F[0] = scores[0] / scores_sum;
		
		for (int j = 1; j < population_size; j++) {
			F[j] = F[j - 1] + scores[j] / scores_sum;
		}
		
		for (int j = 0; j < parents_count; j++) {
			double pr = double(rand()) / RAND_MAX;
			int p = 0;
			while (F[p] < pr) {
				p++;
			}
			parents.push_back(&(*population)[p]);
		}

		for (int j = 0; j < population_size; j++) {
			int p1 = duel(parents);
			int p2 = duel(parents);
			int cross = rand() % min((*(parents[p1])).size(), (*(parents[p2])).size());
			(*new_generation).push_back(vector<Photo*>());
			for (int k = 0; k < cross; k++) {
				(*new_generation)[j].push_back((*parents[p1])[k]);
			}
			for (int k = cross; k < (*parents[p2]).size(); k++) {
				(*new_generation)[j].push_back((*parents[p2])[k]);
			}
			for(int k= cross; k<(*parents[p1]).size(); k++) {
				(*new_generation)[j].push_back((*parents[p1])[k]);
			}
			for (int k = 0; k < cross; k++) {
				(*new_generation)[j].push_back((*parents[p2])[k]);
			}

			int mut_count = (*new_generation)[j].size() / 5;
			for (int mc = 0; mc < mut_count; mc++) {
				int place = rand() % (*new_generation)[j].size();
				Photo* new_slide = &slides[rand() % slides.size()];
				(*new_generation)[j][place] = new_slide;
			}
			remove_same_photos((*new_generation)[j]);
			
		}
		scores_sum = 0;
		population = new_generation;
		for (int k = 0; k < population_size; k++) {
			scores[k]=total_score((*population)[k]);
			if (scores[k] > max_score) max_score = scores[k];
			scores_sum += scores[k];
		}
		
		cout << max_score <<'\n';
	}



}