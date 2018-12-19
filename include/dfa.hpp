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
		
		template<typename NT>
		DFA< NT, V > rename( std::function<NT(T)> converter ) const{
			NT newIni = converter(initial);
			std::set<NT> newFinal;
			for(auto& fin : finals){
				newFinal.insert(converter(fin));
			}
			std::map<NT, 
				 	 std::unordered_map<V,NT>> newDelta;
			for(auto& stateTr : delta){
				std::unordered_map<V, NT > newStateTr;
				for(auto& vocTr : stateTr.second){
					newStateTr[vocTr.first] = converter(vocTr.second);
				}
				newDelta[converter(stateTr.first)] = newStateTr;
			}
			return DFA<NT, V>(newIni, newFinal, newDelta);
		}

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

		DFA<T, V> complete(const T& well, const std::set<V> alphabet) const{
			auto cInit = initial;
			auto cFinal = finals;
			auto cDelta = delta;
			bool addedState = false;
			for(auto& state : cDelta){
				for(auto& letter : alphabet){
					if(state.second.count(letter) == 0){
						state.second[letter] = well;
						addedState = true;
					}
				}
			}
			if(addedState){
				for(auto& letter : alphabet){
					cDelta[well][letter] = well;
				}
			}
			return DFA<T, V>(cInit, cFinal, cDelta);
		}

		DFA<T, V> minimalize(const T& well, const std::set<V>& alphabet) const{
			auto completed = complete(well, alphabet);
			std::set<std::set<T>> classes;
			classes.insert(finals);
			std::set<T> nonFinal;
			for(auto& state : delta){
				if(finals.count(state.first) == 0){
					nonFinal.insert(state.first);
				}
			}
			std::set<std::set<T>> newClass;
			typename DFA< std::set<T>, V>::deltaFunction newDelta;
			while(classes.size() != newClass.size()){
				newDelta = DFA< std::set<T>, V>::deltaFunction();
				for(auto& eqclass : classes){
					std::map<std::map< V, std::set<T>>, std::set<T>> trClasses;
					for(auto& state : eqclass){
						std::map<V, std::set<T>> tr;
						for(auto& nextState : delta.at(state)){
							for(auto& destClass : classes){
								if(destClass.count(nextState.second) == 1){
									tr[nextState.count.first] = destClass;
								}
							}
						}
						trClasses[tr].insert(state);
						newDelta[eqclass] = tr;
					}
					for(auto& flwSet : trClasses){
						newClass.insert(flwSet.second);
					}
				}
				classes = newClass;
				newClass = {};
			}
			std::set<std::set<T>> newFinal;
			for(auto& eqclass : classes){
				bool isFinal = false;
				for(auto& elem : eqclass){
					if(finals.count(elem) == 1){
						isFinal = true;
						break;
					}
				}
				if(isFinal){
					newFinal.insert(classes);
				}
			}
			std::set<T> newInitial;
			for(auto& eqclass : classes){
				for(auto& elem : eqclass){
					if(elem == initial){
						newInitial = eqclass;
						break;
					}
				}
				if(!newInitial.empty()){
					break;
				}
			}
			DFA<std::set<T>, V> minDFA(newInitial, newFinal, newDelta);
			return minDFA.rename([](std::set<T> s){ return (*s.begin()); });
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

		T getInitial() const{
			return initial;
		}
		std::set<T> getFinals() const{
			return finals;
		}
		deltaFunction getDelta() const{
			return delta;
		}

	private:
		T initial;
		std::set<T> finals;
		deltaFunction delta;
};

#endif /* DFA_HPP */
