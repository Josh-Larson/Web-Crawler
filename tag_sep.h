#ifndef MOCK_H
#define MOCK_H
#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;

vector <vector <vector <string> > > loadHTML(string f) {
	ifstream stream;
	stream.open(f.c_str());
	string line = "";
	int type = 0; // 0 = Not In Element ;; 1 = In Element
	int side = 0; // 0 = No Element ;; 1 = Left Side ;; 2 = Right Side
	string left_buf = "";
	string right_buf = "";
	vector <vector <vector <string> > > elements;
	vector <vector <string> > test;
	vector <string> t;
	t.push_back("");
	t.push_back("");
	test.push_back(t);
	int pos = -1;
	int equal = 0;
	while (stream.good()) {
		getline(stream, line);
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == '<') {
				type = 1;
				elements.push_back(test);
				pos++;
				elements[pos].erase(elements[pos].begin(), elements[pos].end());
				continue;
			} else if (line[i] == '>') {
				if (left_buf != "") {
					t[0] = left_buf;
					t[1] = right_buf;
					elements[pos].push_back(t);
					left_buf = "";
					right_buf = "";
					equal = 0;
				}
				left_buf = "";
				right_buf = "";
				type = 0;
				continue;
			}
			if (type == 0) continue;
			if (line[i] == ' ') {
				t[0] = left_buf;
				t[1] = right_buf;
				elements[pos].push_back(t);
				equal = 0;
				left_buf = "";
				right_buf = "";
				side = 1;
				continue;
			}
			if (line[i] == '=') {
				side = 2;
				equal = 1;
				continue;
			}
			if (line[i] == '<' || line[i] == '>' || line[i] == ' ') continue;
			if (side == 0) side = 1;
			if (side == 1) {
				left_buf.push_back(line[i]);
			} else if (side == 2) {
				right_buf.push_back(line[i]);
			}
		}
	}
	return elements;
}


#endif
