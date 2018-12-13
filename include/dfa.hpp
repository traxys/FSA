#ifndef DFA_HPP
#define DFA_HPP

#include "fsa.hpp"

#include <map>
#include <set>
#include <unordered_map>
#include <forward_list>

template<typename T,typename V>
class DFA{
	public:
		using deltaFunction = std::map<T, std::unordered_map<V,T> >;
		DFA(const T& initial_,
			const std::set<T>& finals_,
			const deltaFunction& delta_) :
			initial(initial_),
			finals(finals_),
			delta(delta_) {}
		
		template<typename FSAT>
		DFA(const FSA<FSAT,V>& fsa,
			const std::set<V>& alphabet,
			typename std::enable_if<std::is_same<std::string,T>::value>::type * = nullptr) {
			const FSA<std::string, V>& fsa_d = fsa.determinate(alphabet);
			initial = *fsa_d.getInitial().begin();
			finals = fsa_d.getFinal();
			for(auto& state : fsa_d.getTransition()){
				for(auto& tr : state.second){
					delta[state.first][tr.first] = 
						*tr.second.begin();
				}
			}
		}
		
		void print(){
			std::cout << "Inital : " << initial << std::endl;
			std::cout << "Finals : " << finals << std::endl;
			for(auto& state : delta){
				for(auto& tr : state.second){
					std::cout << state.first << " : " << tr.first
						  	  << " -> " << tr.second << std::endl;
				}
			}
		}

		bool validate(const std::forward_list<V>& word){
			T currentState = initial;
			for(auto& letter: word){
				if(delta[currentState].count(letter) == 0)
					return false;
				currentState = delta[currentState][letter];
			}
			return finals.count(currentState) == 1;
		}

	private:
		T initial;
		std::set<T> finals;
		deltaFunction delta;
};

#endif /* DFA_HPP */
