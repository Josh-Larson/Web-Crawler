/*
 * Author: Josh Larson
 * Program Started: January 22, 2012 01:32:30
 */
#include <iostream>
#include "tag_sep.h"
#include <time.h>
#include "source.h"
#include <string.h>
using namespace std;

string domain_find(string url);
string recursive_crawl(string url);
string strtolower(string str);
string validate_url(string url, string start);
bool is_found(string url, string opt = "");

int main(int argc, char *argv[]) {
	// Starting URL
	string start = "www.google.com";
	if (argc > 1) {
		start = argv[1];
	}
	recursive_crawl(start);
	return 0;
}

string recursive_crawl(string start) {
	// Creates file, class of source which is defined in source.h
	source file(start);
	file.dump("file"); // Dump the lines into the file: file
	vector <vector <vector <string> > > elements;
	elements = loadHTML("file"); // A little XML Parser I made, it'll dump html elements from "file"
	string domain = "";
	int output_to_file = 0;
	int found_file = 0;
	if (is_found(start)) found_file = 1;
	cout << "Found File: " << start << "\n";
	for (int a = 0; a < elements.size(); a++) {
		if (elements[a].size() > 0) {
			// If not outputted this file, and it finds the html or body tag, then save the url
			if (output_to_file == 0 && is_found(start) == false && (found_file == 0 || found_file == 1)
			 && (strtolower(elements[a][0][0]) == "html" || strtolower(elements[a][0][0]) == "body")) {
				output_to_file = 1;
				ofstream stream("url_list.txt", ios::out | ios::app);
				stream << start << "\n";
				stream.close();
			}
		}
		for (int b = 0; b < elements[a].size(); b++) {
			if (strtolower(elements[a][b][0]) == "href" && strtolower(elements[a][0][0]) != "img") {
				string url = elements[a][b][1];
				url = validate_url(url, start);
				if (url == start) continue;
				// Check if url is already logged
				if (is_found(url)) {
					continue;
				}
				if (is_found(url, start)) {
					continue;
				}
				if (url[0] == '#') continue; // If it's an ID
				// Limit a few file extensions so that
				// we don't get a bunch of crap
				if (url.substr(url.length()-4) == ".css") {
					continue;
				}
				if (url.substr(url.length()-4) == ".ico") {
					continue;
				}
				if (url.substr(0, 11) == "javascript:") {
					continue;
				}
				if (url.substr(url.length()-4) == ".exe") {
					continue;
				}
				// Continue on with the recursive crawl
				recursive_crawl(url);
			}
		}
	}
	return "";
}

/*
 * Isolate the domain from the
 * supplied url and then returns
 * it
 */
string domain_find(string url) {
	string domain = "";
	int found_double = 0;
	for (int i = 0; i < url.length(); i++) {
		if (i < url.length()-1) {
			if (url[i] == '/' && url[i+1] == '/') {
				found_double = 1;
				domain.push_back(url[i]);
				domain.push_back(url[i+1]);
				i++;
				continue;
			}
		}
		if (url[i] == '.' && found_double == 0) {
			found_double = 1;
			domain.push_back(url[i]);
			continue;
		}
		if (url[i] == '/' && found_double == 1) {
			break;
		}
		domain.push_back(url[i]);
	}
	if (domain[domain.length()-2] == '/' && domain[domain.length()-1] == '/') {
		domain = domain.substr(0, domain.length()-2);
	}
	return domain;
}

/*
 * Check the file "url_list.txt"
 * to see if the url has already
 * been recorded
 */
bool is_found(string url, string opt) {
	if (opt != "") {
		if (url.find("?") != string::npos && opt.find("?") != string::npos) {
			if (opt.substr(0, opt.find("?")) == url.substr(0, url.find("?"))) {
				return true;
			} else if (opt.substr(0, opt.find("?")) == url) {
				return true;
			} else if (url.substr(0, url.find("?")) == opt) {
				return true;
			}
		} else {
			if (opt.substr(0, url.length()) == url) {
				return true;
			}
		}
	}
	ifstream stream("url_list.txt");
	string line = "";
	while (stream.good()) {
		getline(stream, line);
		if (url.find("?") != string::npos && line.find("?") != string::npos) {
			if (line.substr(0, line.find("?")) == url.substr(0, url.find("?"))) {
				return true;
			} else if (line.substr(0, line.find("?")) == url) {
				return true;
			} else if (url.substr(0, url.find("?")) == line) {
				return true;
			}
		} else {
			if (line.substr(0, url.length()) == url) {
				return true;
			}
		}
	}
	return false;
}

string strtolower(string str) {
	for (int i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

/*
 * Change a few urls so that they
 * can be used by the crawler
 */
string validate_url(string url, string start) {
	if (url[0] == 39 || url[0] == 34) {
		url = url.substr(1);
	}
	if (url[url.length()-1] == 39 || url[url.length()-1] == 34) {
		url = url.substr(0, url.length()-1);
	}
	if (url[0] == '/' && url[1] != '/') {
		url.insert(0, domain_find(start).c_str());
	} else if (url[0] == '/' && url[1] == '/') {
		url = url.substr(1);
		url.insert(0, domain_find(start).c_str());
	}
	if (url.find("www.") == string::npos
	 && (url.find("http://") == string::npos && url.find("https://") == string::npos)) {
	 	url.insert(0, "/");
		url.insert(0, domain_find(start).c_str());
	}
	if (url.find(".") == string::npos
	 && url.find("http://") == string::npos
	 && url.find("https://") == string::npos
	 && url.find("www.") == string::npos) {
	 	url.insert(0, "/");
	 	url.insert(0, domain_find(start).c_str());
	 }
	return url;
}
