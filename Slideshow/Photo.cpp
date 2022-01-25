#include "Photo.h"

Photo::Photo(char tp, int nt, vector<string> tg) {
	count++;
	id = count;
	type = tp;
	numberOfTags = nt;
	for (int i = 0; i < numberOfTags; i++) {
		tags.insert(tg[i]);
	}
}
Photo::Photo(Photo* p1, Photo* p2) {
	count++;
	id = count;
	type = 'H';
	set_union(p1->tags.begin(), p1->tags.end(),
		p2->tags.begin(), p2->tags.end(),
		inserter(tags, tags.begin()));
	numberOfTags = tags.size();
}
int Photo::get_id() {
	return this->id;
}
char Photo::get_type() {
	return this->type;
}
int Photo::get_numberOfTags() {
	return this->numberOfTags;
}
set<string> Photo::get_tags() {
	return this->tags;
}