#include "dfa.hpp"
#include "fsa.hpp"
#include "drawer.hpp"

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
	DFA<std::string, bool> testDFA(testFSA, {0,1});
	//std::cout << dot_FSA(testFSA).str();

	using epsFSAt = FSA<char, epsilonAlphabet<bool>>;
	std::unordered_map<epsFSAt::alphabet, std::set<char>> truc;
	auto eps = epsilon();
	epsFSAt::epsTransitionTable maps;
	maps['a'][eps] = {'b'};
	maps['a'][1] = {'a'};
	maps['a'][0] = {'c'};
	maps['b'][0] = {'a'};
	maps['b'][1] = {'c'};
	maps['c'][0] = {'a'};
	maps['c'][1] = {'c'};
	epsFSAt FSAeps({'a'}, {'c'}, maps);
	//FSAeps.printFSA();
	std::cout << dot_FSA(FSAeps).str() << std::endl;
}
