
#include <iostream>
#include <fstream>

#include "WuManber.hpp"

using namespace std;

int main() 
{
	std::string Text = "my name is fanpf. what is your name ? my name is rose.";

	std::vector<std::string> patterns;
	std::vector<size_t> patterns_id;

	patterns.push_back("name");
	patterns_id.push_back(1001);
    
	WuManber wu;
	if (wu.Init(patterns_id, patterns) != 0) {
		std::cerr << "Error: init!" << std::endl;
		exit(-1);
	}

	cout << wu << endl;
 
#if 0
	vector<WMResult_t> results;
	int ret = wu.Search(Text, &results);
	cout << "hit_count:" << ret << endl;


	cout << "results:" << endl;
	for (auto & res : results) {
		cout << res.pattern_id << ":" <<  res.index  << endl;
	}
#else
	int ret = wu.Search(Text, nullptr);
	cout << "hit_count:" << ret << endl;

#endif

	return 0;
}
