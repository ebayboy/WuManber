
#include <iostream>
#include <fstream>
#include<time.h>

#include "WuManber.hpp"

using namespace std;

void test1()
{
	std::string Text = "my name is fanpf. what is your name ? my name is rose.";

	std::vector<std::string> patterns;
	std::vector<size_t> patterns_id;

	size_t id = 1000;
	patterns.push_back("name");
	patterns_id.push_back(id++);
	patterns.push_back("is");
	patterns_id.push_back(id++);

	WuManber wu;
	if (wu.Init(patterns_id, patterns) != 0) {
		std::cerr << "Error: init!" << std::endl;
		exit(-1);
	}

	cout << wu << endl;

#if 1
	vector<WMResult_t> results;
	int ret = wu.Search(Text, &results);
	cout << "hit_count:" << ret << endl;


	cout << "results:" << endl;
	for (auto & res : results) {
		cout << res.pattern_id << ":" <<  res.index  << endl;
	}
#else
	clock_t start,end;
	start = clock();

	size_t count = 100000;
	for (size_t i = 0;  i< count; i++) {
		int ret = wu.Search(Text, nullptr);
	}
	end = clock();
	//cout << "hit_count:" << ret << endl;

#endif

}

//1.68s
void test2() 
{
	std::string Text = "This is some text I made up.  This will be testing\n" 
		"multi-pattern matching from Wu/Manber's paper called\n"
		"'A Fast Algorithm for Multi-Pattern Searching'. Manber is\n"
		"currently at Google.";

	size_t id = 1000;
	std::vector<size_t> patterns_id;

	vector<std::string> patterns;
	patterns.push_back( "this is"  );
	patterns_id.push_back(id++);
	patterns.push_back( "pattern"  );
	patterns_id.push_back(id++);
	patterns.push_back( "google!"  );
	patterns_id.push_back(id++);
	patterns.push_back( "anber"  );
	patterns_id.push_back(id++);

	WuManber wu;
	if (wu.Init(patterns_id, patterns) != 0) {
		std::cerr << "Error: init!" << std::endl;
		exit(-1);
	}

	cout << "start..." << endl;
	clock_t end, start = clock();

	size_t count = 100000;
	for (size_t i = 0;  i< count; i++) {
		int ret = wu.Search(Text, nullptr);
	}

	end = clock();
	cout << "Totle Time : " <<(double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
}


int main()
{
	test1();
//	test2();

	return 0;
}
