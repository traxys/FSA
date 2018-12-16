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
		std::map<T, std::set<V> > sameDest;
		for(auto& letter : tr.second){
			sameDest[letter.second].insert(letter.first);
		}
		for(auto& edge : sameDest){
			os << "\t\"" << tr.first << "\" -> \""
				<< edge.first  << "\"[label=\"";
			std::string labels = "";
			for(auto& letter : edge.second){
				std::ostringstream osp;
				osp << letter << ",";
				labels += osp.str();
			}
			labels.pop_back();
			os << labels;
			os << "\"]" << std::endl;
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
		std::map<T, std::set<V> > sameDest;
		for(auto& letter : tr.second){
			for(auto& nextState : letter.second){
				sameDest[nextState].insert(letter.first);
			}
		}
		for(auto& edge : sameDest){
			os << "\t\"" << tr.first << "\" -> \""
				<< edge.first  << "\"[label=\"";
			std::string labels = "";
			for(auto& letter : edge.second){
				std::ostringstream osp;
				osp << letter << ","; 
				labels += osp.str();
			}
			labels.pop_back();
			os << labels;
			os << "\"]" << std::endl;
		}
	}
	os << "}" << std::endl;
	return os;
}

#endif /* DRAWER_HPP */
