#include "dfa.hpp"

#include <functional>
#include <set>
#include <unordered_map>

int main(){
	FSA<char,bool>::transitionTable trTable;
	trTable['a'][0] = {'a','b'};
	trTable['a'][1] = {'b'};
	trTable['b'][0] = {'b'};
	trTable['b'][1] = {'a'};
	FSA<char, bool> testFSA({'a'},{'b'},trTable);
	testFSA.printFSA();
	//testFSA.determinate( {0,1} ).printFSA();
	DFA<std::string, bool> testDFA(testFSA, {0,1});
	testDFA.print();
}
