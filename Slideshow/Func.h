#pragma once
#include "Photo.h"
#include <numeric>

int score(Photo* p1, Photo* p2);

int total_score(vector<Photo*>& slideshow);


vector<Photo> random_vertical_pairs(vector<Photo>& photos, vector<int> vertical);

void remove_same_photos(vector<Photo*>& slideshow);

vector<Photo*> generate_random_solution(vector<Photo>& slides, int sol_size);

int duel(vector<vector<Photo*>*>& parents);