// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <vector>
#include <string>
#include <queue>

// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
  int state;
  int next_state;
  std::string str;
};

struct FiniteStateAutomaton {
  // Make your own FSA struct here.
	std::vector<FSATableElement> elements;
	std::vector<int> accept_state;
};

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);

// 현재 state에 terminal x가 들어왔을때 어떤 state로 갈 수 있는 지 계산.
std::vector<int> makeDFA(const FiniteStateAutomaton& fsa, int current, char terminal);
std::vector<int> makeDFA(const FiniteStateAutomaton& fsa, std::vector<int> states, char terminal);

#endif //_PL_HOMEWORK_FSA_H_

