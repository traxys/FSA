#ifndef FSA_HPP
#define FSA_HPP

#include <unordered_map>
#include <map>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <iostream>

template< typename T, typename V >
class FSA{
	public:
		using transitionTable = std::map<T, std::unordered_map<V, std::set<T> > >;
		FSA(const std::set<T>& initial_,const std::set<T>& final_,const transitionTable& trTable_) :
			initial(initial_),
			finalState(final_),
			trTable(trTable_) {}

		template<typename NT>
		FSA< NT, V > rename( std::function<NT(T)> converter ) const{
			std::set<NT> newIni;
			std::set<NT> newFinal;
			for(auto& ini : initial){
				newIni.insert(converter(ini));
			}
			for(auto& fin : finalState){
				newFinal.insert(converter(fin));
			}
			std::map<NT, 
				 	 std::unordered_map<V, 
				                       std::set<NT> > > 
												  newTrs;
			for(auto& stateTr : trTable){
				std::unordered_map<V, std::set<NT> > newStateTr;
				for(auto& vocTr : stateTr.second){
					std::set<NT> newFollowingSet;
					for(auto& followingState : vocTr.second){
						newFollowingSet.insert(converter(followingState));
					}
					newStateTr[vocTr.first] = newFollowingSet;
				}
				newTrs[converter(stateTr.first)] = newStateTr;
			}
			return FSA<NT, V>(newIni, newFinal, newTrs);
		}

		void printTransition() const{
			for(auto& state : trTable){
				for(auto& vocTr : state.second){
					std::cout << state.first << " : " << vocTr.first
						<< " -> " << vocTr.second << std::endl;
				}
			}
		}

		void printFSA() const{
			std::cout << "Initials : " << initial << std::endl;
			std::cout << "Finals : " << finalState << std::endl;
			std::cout << "Transition : " << std::endl;
			printTransition();
		}

		FSA<std::string, V> determinate(const std::set<V>& alphabet)
			const {
			std::map< std::set<T> , 
				      std::unordered_map<V,std::set<std::set<T> > > >
						  partsTrTable;
			std::set< std::set<T> > initials = {initial};
			std::set< std::set<T> > finals;
			std::set< std::set<T> > todo;
			todo.insert(initial);
			while(!todo.empty()){
				auto currentSet = (*todo.begin());
				todo.erase(todo.begin());
				std::unordered_map< V, std::set< std::set<T> > >
					transition;
				for(auto& letter : alphabet){
					bool isFinal = false;
					std::set<T> afterLetter;
					for(auto& setElem : currentSet){
						for(auto& dest : trTable.at(setElem).at(letter)){
							if(finalState.count(dest) == 1){
								isFinal = true;
							}
							afterLetter.insert(dest);
						}
					}
					if(partsTrTable.count(afterLetter) == 0){
						todo.insert(afterLetter);
					}
					if(isFinal){
						finals.insert(afterLetter);
					}
					transition[letter] = {afterLetter};
				}
				partsTrTable[ currentSet ] = transition;
			}
			std::map<std::set<T>, std::string> names;
			int i = 0;
			for(auto& stateTr : partsTrTable){
				names[stateTr.first] = "r" + std::to_string(i);
				i++;
			}

			std::function<std::string(std::set<T>)> conv =
				[&names](const std::set<T>& part){ 
					return names[part];
				};
			return FSA<std::set<T>,V>(initials,finals,partsTrTable)\
				.rename(conv);
		};

		std::set<T> getInitial() const{
			return initial;
		}
		std::set<T> getFinal() const{
			return finalState;
		}
		transitionTable getTransition() const{
			return trTable;
		}

	private:
		std::set<T> initial;
		std::set<T> finalState;
		transitionTable trTable; 
};

template<typename T>
inline std::ostream& operator<<(std::ostream& os,const std::set<T>&
		elementSet){
	os << "{ ";
	for(auto& elem: elementSet){
		os << elem << ",";
	}
	os << " }";
	return os;
}

#endif /* FSA_HPP */
