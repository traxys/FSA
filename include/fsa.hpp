#ifndef FSA_HPP
#define FSA_HPP

#include <unordered_map>
#include <map>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <iostream>
#include <variant>

/// \brief Class representing any type of Finite state automata
/// with states of type T over a vocabulary of type V
template< typename T, typename V >
class FSA{
	public:
		/// \brief A map representing the transitions from state to state
		using transitionTable = std::map<T, std::unordered_map<V, std::set<T> > >;
		/// \brief Create a FSA from a map(state -> map(letter -> nextState))
		FSA(const std::set<T>& initial_,const std::set<T>& final_,const transitionTable& trTable_) :
			initial(initial_),
			finalState(final_),
			trTable(trTable_) {}
	
		/// \brief Rename all states
		/// \param converter function used to transalate a type into the new type
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
	

		/// \brief Prints the transition table
		void printTransition() const{
			for(auto& state : trTable){
				for(auto& vocTr : state.second){
					std::cout << state.first << " : " << vocTr.first
						<< " -> " << vocTr.second << std::endl;
				}
			}
		}

		/// \brief Prints the FSA as a list of states
		void printFSA() const{
			std::cout << "Initials : " << initial << std::endl;
			std::cout << "Finals : " << finalState << std::endl;
			std::cout << "Transition : " << std::endl;
			printTransition();
		}
		
		/// \brief Determinates the FSA using an alphabet
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
		
		/// \brief Returns the initial states
		std::set<T> getInitial() const{
			return initial;
		}
		/// \brief Return the final states
		std::set<T> getFinal() const{
			return finalState;
		}
		/// \brief Returns the transition table
		transitionTable getTransition() const{
			return trTable;
		}

	private:
		std::set<T> initial;
		std::set<T> finalState;
		transitionTable trTable; 
};

/// \brief prints a set in JSON form ({elem1, ..., elemN})
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

/// \brief Epsilon value representing the empty word
struct epsilon{
	bool value;
	epsilon() : value(false) {}
};

/// \brief All empty words are equal
inline bool operator==(const epsilon&, const epsilon&){ return true; };


/// \brief All empty words are equal
inline bool operator<(const epsilon&, const epsilon&){ return false; };

/// \brief All empty words are same
inline std::ostream& operator<<(std::ostream& os,const epsilon&){
	os << "ε";
	return os;
}

/// \brief Alphabet of elements of V and the empty word
template<typename V>
using epsilonAlphabet = std::variant<V,epsilon>;

/// \brief Transforms each letter from the variant type to
/// the corresponding string representation
template<typename V>
inline std::ostream& operator<<(std::ostream& os,const epsilonAlphabet<V>& letter){
	if(std::holds_alternative<epsilon>(letter)){
		os << std::get<epsilon>(letter);
	}else{
		os << std::get<V>(letter);
	}
	return os;
}

/// \brief hash each variant with either
namespace std {
		template <typename V> struct hash<epsilonAlphabet<V>>{
			size_t operator()(const epsilonAlphabet<V>& a) const{
				if(std::holds_alternative<epsilon>(a)){
					return hash<string>()("krikatoktok");
				}else{
					return hash<V>()(std::get<V>(a));
				}
			}
		};
} // namespace std;

/// \brief Finite state automatas with epsilon transitions
template<typename T, typename V>
class FSA<T, epsilonAlphabet<V>>{
	public:
		/// \brief The alphabet type
		using alphabet = epsilonAlphabet<V>;
		/// \brief Transition table for the FSA
		using epsTransitionTable = std::map<T, std::unordered_map<alphabet, std::set<T>>>;
		
		/// \brief Construct the FSA from the inital and final states with the transitions
		FSA(const std::set<T>& initial_,const std::set<T>& final_,const epsTransitionTable& trTable_) :
			initial(initial_),
			finalState(final_),
			trTable(trTable_) {}
		
		/// \brief Accumulate the neighours by epsilon transition of `from` in `current`
		void accEps(std::set<T>& current, const T& from) const {
			for(auto& tr : trTable.at(from)){
				if(std::holds_alternative<epsilon>(tr.first)){
					for(auto& state : tr.second){
						if(current.count(state) == 0){
							current.insert(state);
							accEps(current, state);
						}
					}
				}
			}
		}
		
		/// \brief Returns an equivalent FSA without epsilon transition
		auto removeEpsilon() const{
			std::set<T> finals;
			typename FSA<T, V>::transitionTable newTrTable;
			for(auto& state : trTable){
				std::set<T> acc = {state.first};
				accEps(acc, state.first);
				for(auto& accState : acc){
					if(finalState.count(accState) == 1){
						finals.insert(accState);
					}
					for(auto& accTr : trTable.at(accState)){
						if(!std::holds_alternative<epsilon>(accTr.first)){
							newTrTable[state.first][std::get<V>(accTr.first)].insert(accTr.second.begin(), accTr.second.end());
						}
					}
				}
			}
			return FSA<T, V>(initial, finals, newTrTable);
		}
		
		/// \brief Print the transition table
		void printTransition() const{
			for(auto& state : trTable){
				for(auto& vocTr : state.second){
					std::cout << state.first << " : " << vocTr.first
						<< " -> " << vocTr.second << std::endl;
				}
			}
		}

		/// \brief Print the FSA
		void printFSA() const{
			std::cout << "Initials : " << initial << std::endl;
			std::cout << "Finals : " << finalState << std::endl;
			std::cout << "Transition : " << std::endl;
			printTransition();
		}
		
		/// \brief Rename the states with converter
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

		/// \brief First remove the epsilon transitions then determinate
		FSA<std::string, V> determinate(const std::set<V>& alph) const{
			return removeEpsilon().determinate(alph);
		}
		
		/// \brief Get the initial states
		std::set<T> getInitial() const{
			return initial;
		}
		/// \brief Get the final states
		std::set<T> getFinal() const{
			return finalState;
		}
		/// \brief Get the transtion table
		epsTransitionTable getTransition() const{
			return trTable;
		}
	private:
		std::set<T> initial;
		std::set<T> finalState;
		epsTransitionTable trTable; 
};

#endif /* FSA_HPP */
