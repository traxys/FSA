#include "fsa.hpp"

#include <functional>
#include <set>
#include <unordered_map>

int main(){
	FSA<std::set<char>,bool>::deltaFunction delta;
	delta[{'a','c'}][0] = {{'a','c'},{'b'}};
	delta[{'a','c'}][1] = {{'b'}};
	delta[{'b'}][0] = {{'b'}};
	delta[{'b'}][1] = {{'a','c'}};
	FSA<std::set<char>, bool> testFSA({{'a','c'}},{{'b'}},delta);
	testFSA.printTransition();
}
