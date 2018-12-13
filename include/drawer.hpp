#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <sstream>

#include "dfa.hpp"

template<typename T,typename V>
std::ostringstream dot_DFA(const DFA<T,V>& dfa){
	std::ostringstream os;
	os << "digraph dfa {" << std::endl;
	os << "\t\"\"[shape=none]" << std::endl;
	auto dfaFinals = dfa.getFinals();
	for(auto& tr : dfa.getDelta()){
		os << "\t\"" << tr.first << "\"[shape=";
		if(dfaFinals.count(tr.first) == 1)
			os << "doublecircle";
		else
			os << "circle";
		os << "]" << std::endl;
	}
	os << std::endl << "\t\"\" -> \"" << dfa.getInitial() <<
		"\"" <<std::endl;
	for(auto& tr : dfa.getDelta()){
		for(auto& letter : tr.second){
			os << "\t\"" << tr.first << "\" -> \""
					  << letter.second  << "\"[label=\""
					  << letter.first << "\"]" << std::endl;
		}
	}
	os << "}" << std::endl;
	return os;
}

template<typename T, typename V>
std::ostringstream dot_FSA(const FSA<T,V>& fsa){
	std::ostringstream os;
	os << "digraph dfa {" << std::endl;
	os << "\t\"\"[shape=none]" << std::endl;
	auto fsaFinals = fsa.getFinal();
	auto fsaInit = fsa.getInitial();
	auto trTable = fsa.getTransition();
	for(auto& tr : trTable){
		os << "\t\"" << tr.first << "\"[shape=";
		if(fsaFinals.count(tr.first) == 1)
			os << "doublecircle";
		else
			os << "circle";
		os << "]" << std::endl;
	}
	for(auto& ini : fsaInit){
		os << std::endl << "\t\"\" -> \"" << ini <<
			"\"" <<std::endl;
	}
	for(auto& tr : trTable){
		for(auto& letter : tr.second){
			for(auto& nextState : letter.second){
				os << "\t\"" << tr.first << "\" -> \""
					<< nextState  << "\"[label=\""
					<< letter.first << "\"]" << std::endl;
			}
		}
	}
	os << "}" << std::endl;
	return os;
}

#endif /* DRAWER_HPP */
