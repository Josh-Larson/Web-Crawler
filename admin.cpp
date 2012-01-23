#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

string domain_find(string url);

int main(int argc, char *argv[]) {
	ifstream stream("url_list.txt");
	vector <string> url_list;
	int urls = 0;
	int domains = 0;
	int found = 0;
	string line = "";
	while (stream.good()) {
		getline(stream, line);
		found = 0;
		for (int i = 0; i < url_list.size(); i++) {
			if (domain_find(line) == url_list[i]) {
				found = 1;
				break;
			}
		}
		if (found == 0) {
			url_list.push_back(domain_find(line));
			domains++;
		}
		urls++;
	}
	for (int i = 0; i < url_list.size(); i++) {
		cout << url_list[i] << "\n";
	}
	cout << "Found a total of " << urls << " urls.\n";
	cout << "Domains Found: " << domains << "\n";
	return 0;
}

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
