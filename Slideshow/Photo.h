#pragma once
#include <set>
#include<string>
#include<vector>
#include <algorithm>
#include<cstdlib>
#include <fstream>

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

	Photo(char tp, int nt, vector<string> tg);

	Photo(Photo* p1, Photo* p2);

	int get_id();
	char get_type();
	int get_numberOfTags();
	set<string> get_tags();
};

