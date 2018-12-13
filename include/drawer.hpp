#ifndef DRAWER_HPP
#define DRAWER_HPP

#include "dfa.hpp"

template<typename T,typename V>
void dot_DFA(const DFA<T,V>& dfa){
	std::cout << "digraph dfa {" << std::endl;
	std::cout << "\t\"\"[shape=none]" << std::endl;
	auto dfaFinals = dfa.getFinals();
	for(auto& tr : dfa.getDelta()){
		std::cout << "\t\"" << tr.first << "\"[shape=";
		if(dfaFinals.count(tr.first) == 1)
			std::cout << "doublecircle";
		else
			std::cout << "circle";
		std::cout << "]" << std::endl;
	}
	std::cout << std::endl << "\t\"\" -> \"" << dfa.getInitial() <<
		"\"" <<std::endl;
	for(auto& tr : dfa.getDelta()){
		for(auto& letter : tr.second){
			std::cout << "\t\"" << tr.first << "\" -> \""
					  << letter.second  << "\"[label=\""
					  << letter.first << "\"]" << std::endl;
		}
	}
	std::cout << "}" << std::endl;
}

#endif /* DRAWER_HPP */
