#include <iostream>
#include <vector>
#include <algorithm>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

void aggregate(vector<vector<uint32_t>>& entries) {
	for(int len=32; len > 0; len--){
		if(entries[len].size() < 2){
			continue;
		}

		sort(entries.begin(), entries.end());

		for(unsigned int i=0; i < entries[len].size()-1; i++){

			uint32_t& first = entries[len][i];
			uint32_t& second = entries[len][i+1];

			if(first == 0xffffffff){
				continue;
			}

			if((first ^ second) == (((uint32_t) 1) << (32-len))){
				uint32_t new_entry = first & (~(1 << (32-len)));
				entries[len-1].push_back(new_entry);
				first = 0xffffffff;
				second = 0xffffffff;
			}
		}
	}
};

int main(int argc, char** argv){
	(void) argc;
	(void) argv;

	vector<vector<uint32_t>> entries;
	entries.resize(33);
	string ip;
	while(cin >> ip){
		in_addr_t num = inet_addr(ip.c_str());
		entries[32].push_back(htonl(num));
	}
	aggregate(entries);

	for(int len=32; len > 0; len--){
		for(uint32_t ip : entries[len]){
			if(ip != 0xffffffff){
				struct in_addr help;
				help.s_addr = ntohl(ip);
				cout << inet_ntoa(help) << "/" << len << endl;
			}
		}
	}
	return 0;
};
