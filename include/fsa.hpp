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
		using deltaFunction = std::map<T, std::unordered_map<V, std::set<T> > >;
		FSA(const std::set<T>& initial_,const std::set<T>& final_,const deltaFunction& delta_) :
			initial(initial_),
			finalState(final_),
			delta(delta_) {}

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
												  newDelta;
			for(auto& stateTr : delta){
				std::unordered_map<V, std::set<NT> > newStateTr;
				for(auto& vocTr : stateTr.second){
					std::set<NT> newFollowingSet;
					for(auto& followingState : vocTr.second){
						newFollowingSet.insert(converter(followingState));
					}
					newStateTr[vocTr.first] = newFollowingSet;
				}
				newDelta[converter(stateTr.first)] = newStateTr;
			}
			return FSA<NT, V>(newIni, newFinal, newDelta);
		}

		void printTransition() const{
			for(auto& state : delta){
				for(auto& vocTr : state.second){
					std::ostringstream os;
					std::cout << state.first << " : " << vocTr.first << " -> " << vocTr.second << std::endl;
				}
			}
		}

		FSA<std::string, V> determinate(){
			std::map< std::set<T> , std::unordered_map< V, std::set< std::set<T> > > > partsDelta;
			std::set< std::set<T> > todo;
			todo.insert(initial);
		};
	private:
		T currentState;
		std::set<T> initial;
		std::set<T> finalState;
		deltaFunction delta; 
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
