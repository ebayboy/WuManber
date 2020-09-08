#include "WuManber.hpp"
#include <iostream>
#include <fstream>

int main() 
{
	std::string Text = "my name is fanpf. what is your name ? my name is rose.";

	std::vector<std::string> patterns;
	patterns.push_back("name");
    
    //Map holding the times that every pattern appeared
    std::unordered_map<std::string, std::size_t> counters;
    
    for (auto& pattern : patterns)
        counters[pattern] = 0;
    
    //List with the detail of every match found
    std::list<Occurrence> r;
    
	WuManber wu;
	wu.initialize(patterns);
 
    std::size_t lineCounter = 1;
	
	r = wu.search(Text, lineCounter);

	//cout result
	for (auto& res : r) {
		std::cout << res.index << " " << res.pattern << " " << res.line << std::endl;
		counters[res.pattern]++;
	}

	return 0;
}
