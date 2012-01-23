#include <curl/curl.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
/*
-----
==========
			Here Starts the Tags Class
==========
-----
*/
class tag {
	public:
	string full;
	vector <string> attributes;
	tag(string t);
	tag();
	void seperate();
};

tag::tag(string t) {
	tag::full = t;
	tag::seperate();
}

tag::tag() {
	tag::full = "";
}

void tag::seperate() {
	
}

/*
-----
==========
			Here Starts the Web Class
==========
-----
*/


char *code;


class source {
	public:
	string code;
	vector <string> lines;
	/* Very Basic Constructor */
	source();
	source(string url);
	void getSource(string url);
	void dump(string file);
};

int handle_data(char *data, size_t size, size_t nmemb, string *buffer) {
	int result = 0;
	if (buffer != NULL) {
		buffer->append(data, size * nmemb);
		result = size * nmemb;
	}
	return result;
}

void source::dump(string file) {
	ofstream stream(file.c_str());
	for (int i = 0; i < lines.size(); i++) {
		stream << lines[i] << "\n";
	}
	stream.close();
}

void source::getSource(string url) {
	char *urlcpy = (char *)url.c_str();
	string buffer = "";
	CURL* curl = curl_easy_init();
	// Tell libcurl the URL
	curl_easy_setopt(curl,CURLOPT_URL, (char *)url.c_str());
	// Tell libcurl what function to call when it has data
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	// Do it!
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	source::code = buffer;
	int lastNL = 0;
	for (int i = 0; i < buffer.length(); i++) {
		if (buffer[i] == '\n') {
			source::lines.push_back(buffer.substr(lastNL, i - lastNL));
			lastNL = i+1;
		}
	}
	return;
}

source::source() {
	source::code = "";
	return ;
}

source::source(string url) {
	char *urlcpy = (char *)url.c_str();
	string buffer = "";
	CURL* curl = curl_easy_init();
	// Tell libcurl the URL
	curl_easy_setopt(curl,CURLOPT_URL, (char *)url.c_str());
	// Tell libcurl what function to call when it has data
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	// Do it!
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	source::code = buffer;
	int lastNL = 0;
	for (int i = 0; i < buffer.length(); i++) {
		if (buffer[i] == '\n') {
			source::lines.push_back(buffer.substr(lastNL, i - lastNL));
			lastNL = i+1;
		}
	}
	return;
}
