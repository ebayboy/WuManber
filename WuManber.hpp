#pragma once

#include <string>
#include <unordered_map> 
#include <algorithm> 
#include <list>
#include <vector>
#include <cstddef> 
#include <iostream>
#include <cstdio>

#include <string>

struct WMResult_t {
	std::size_t pattern_id;
	std::size_t index;
};

class WuManber {
	public:
		// constructor
		WuManber(): initialized(false), B(2), m(0), patterns(), patterns_id(), shift_table(), aux_shift_table(), hash_table() {};

		int Init(const std::vector<size_t>& exprs_id, const std::vector<std::string>& exprs)
		{
			if (exprs_id.size() !=  exprs.size()) {
				std::cout << "Error: exprs.size:" << exprs.size() << " exprs_id.size:" << exprs_id.size() << std::endl;
				return -1;
			}

			patterns.assign(exprs.begin(), exprs.end());
			patterns_id.assign(exprs_id.begin(), exprs_id.end());

			std::cout << "patterns.size:" << patterns.size() << std::endl;;
			std::cout << "patterns_id.size:" << patterns_id.size() << std::endl;

			//Init of m
			for (auto iter = patterns.begin(); iter != patterns.end(); ++iter) {
				m = (m == 0) ? iter->length() : std::min(iter->length(), m);
			}

			//Init of tables for each block in each pattern
			std::size_t pattern_index = 0;
			for (auto pattern_iter = patterns.begin();
					pattern_iter != patterns.end();
					++pattern_iter, ++pattern_index) {
				for (std::size_t char_index = 0; char_index < m - B + 1; ++char_index) {
					std::string block = pattern_iter->substr(char_index, B);
					std::size_t block_pos = m - char_index - B;
					std::size_t last_shift = (block_pos == 0) ? m - B + 1 : block_pos;

					//Init of SHIFT table
					if (shift_table.count(block)) {
						last_shift = shift_table[block];
						shift_table[block] = std::min(shift_table[block], block_pos);
					}
					else {
						shift_table[block] = block_pos;
					}

					//Init of HASH table
					if (block_pos == 0) {
						hash_table[block].push_back(pattern_index);
						if (last_shift != 0) {
							if (aux_shift_table.count(block))
								aux_shift_table[block] = std::min(aux_shift_table[block], last_shift);
							else
								aux_shift_table[block] = last_shift;
						}
					}
				}
			}

			initialized = true;
			return 0;
		}

	//contructor end

	//destructor
		virtual ~WuManber() {};

		//FUNCTION:
		//	Search text with patterns;
		//@text:
		//	intput text
		//@wmresults : 
		//	not nullptr: output result_set
		//	nullptr: not output result_set
		//RETURN:
		//	error: -1
		//  ok : hit count 
		int Search(const std::string & text, std::vector<WMResult_t> *wmresults)
		{
			int hit_count = 0;

			if (!initialized) {
				std::cerr << "Error: not initialized!!!" << std::endl;
				return -1;
			}

			for (size_t char_index = m - B; char_index < text.length(); ++char_index) {
				std::string block = text.substr(char_index, B);
				auto shift_value = shift_table.find(block);
				if (shift_value != shift_table.end()) {
					if (shift_value->second == 0) {//Possible match
						auto possibles = hash_table.at(block);
						for (size_t j = 0; j < patterns.size(); j++) {
							std::string pattern = patterns[j];
							std::size_t char_pos = char_index - m + B;
							std::size_t i;
							for (i = 0; i < pattern.length(); ++i) {
								if (pattern.at(i) != text.at(char_pos + i)) {
									break;
								}
							}
							if (i == pattern.length()) {
								hit_count++;
								if (wmresults) {
									WMResult_t r{patterns_id.at(j), char_pos};
									wmresults->emplace_back(r);
								}
							}
						}
						char_index += aux_shift_table.at(block) - 1;//Shift according to ASHIFT[block]
					}
					else
						char_index += shift_value->second - 1;//Shift according to SHIFT[block]
				}
				else {
					char_index += m - B; //Shift the maximum possible to the right
				}
			}

			return hit_count;
		}

		friend std::ostream &operator << (std::ostream &output, const WuManber &b) {
			std::cout << "initialized:" << b.initialized << std::endl;
			if (b.patterns.size() != b.patterns_id.size()) {
				std::cerr <<  "Error: patterns.size:" << b.patterns.size() << " patterns_id.size:" << b.patterns_id.size() << std::endl;
			}

			std::cout << "Patterns:" << std::endl;
			for (size_t i = 0; i < b.patterns.size(); i++ ) {
				std::cout << "\t" << b.patterns_id[i] << ":" << b.patterns[i] << std::endl;
			}
			return output;
		}

	private:
		bool initialized;

		//Block size
		std::size_t B;

		//Patterns minimum length
		std::size_t m;

		std::vector<std::string> patterns;
		std::vector<size_t> patterns_id;

		std::unordered_map<std::string, std::size_t> shift_table;
		std::unordered_map<std::string, std::size_t> aux_shift_table;
		std::unordered_map<std::string, std::list<std::size_t> > hash_table;

};

