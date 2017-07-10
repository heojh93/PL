// PL homework: hw1
// fsa.cc

#include <iostream>

#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

// BuildFSA
queue<int> bfsQueue;
vector<int> possibleState;

vector<int> makeDFA(const FiniteStateAutomaton& fsa, int current, char terminal) {

	vector<int> states;
	vector<int> epsilonStack;
	vector<int> traceStack;	// epsilon�� �պ��ϴ� ��츦 ����.

	vector<FSATableElement>::const_iterator it,it2;
	vector<int>::const_iterator it3;

	epsilonStack.push_back(current);

	// epsilon���� ���°�찡 �ִ� ���� loop
	int mediateState;
	while (!epsilonStack.empty()) {
		mediateState = epsilonStack.back();
		epsilonStack.pop_back();
		for (it = fsa.elements.begin(); it != fsa.elements.end(); ++it) {
			if (it->state == mediateState) {
				// �ѹ��� ���� state�� �� ���.
				if ((it->str).find(terminal) != string::npos) {
					states.push_back(it->next_state);
				}
				else if (it->str == "") {
					bool find = false;
					for (int j = 0; j < traceStack.size(); j++) {
						if (it->next_state == traceStack[j]) {
							find = true;
							break;
						}
					}if (!find) {
						epsilonStack.push_back(it->next_state);
						traceStack.push_back(it->next_state);
					}
				}
			}
		}
	}
	
	traceStack.clear();
	// a##### �̷���츦 ����.				
	for (int i = 0; i < states.size(); i++) {
		epsilonStack.push_back(states[i]);
		while (!epsilonStack.empty()) {
			mediateState = epsilonStack.back();
			epsilonStack.pop_back();
			for (it = fsa.elements.begin(); it != fsa.elements.end(); ++it) {
				
				if(it->state == mediateState) {
					if (it->str == "") {

						// states�� �������� push_back
						bool find = false;
						for (int j = 0; j < traceStack.size(); j++) {
							if (it->next_state == traceStack[j]) {
								find = true;
								break;
							}
						}

						if (!find) {
							states.push_back(it->next_state);
							epsilonStack.push_back(it->next_state);
							traceStack.push_back(it->next_state);
						}
					}
				}
			}
		}
	}
/*
	for (int i = 0; i < states.size(); i++) {
		printf("%d ", states[i]);
	}
	*/
	
	return states;

}

std::vector<int> makeDFA(const FiniteStateAutomaton& fsa, std::vector<int> states, char terminal) {
	vector<int> newState;
	vector<int>::const_iterator it;
	for (it = states.begin(); it != states.end(); ++it) {
		vector<int> v = makeDFA(fsa, *it, terminal);
		newState.insert(newState.end(), v.begin(), v.end());
	}
	return newState;
}

bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
  // Implement this function.
	vector<int> v = makeDFA(fsa, 1, str[0]);

	for (int i = 1; i < str.size(); i++) {
		v = makeDFA(fsa, v, str[i]);
	}

	vector<int>::const_iterator it, it2;
	for (it = fsa.accept_state.begin(); it != fsa.accept_state.end(); ++it) {
		for (it2 = v.begin(); it2 != v.end(); ++it2) {
			if (*it == *it2) return true;
		}
	}

  return false;
}

bool BuildFSA(const std::vector<FSATableElement>& elements,
	const std::vector<int>& accept_states,
	FiniteStateAutomaton* fsa) {
	// Implement this function.
	vector<FSATableElement>::const_iterator it;
	vector<int>::iterator it2;
	vector<int>::const_iterator it3;
	bfsQueue.push(1);

	// bfs ���� ���ް����� state ��� --> possibleState.
	while (!bfsQueue.empty()) {
		int popQueue = bfsQueue.front();
		bfsQueue.pop();

		bool find = false;
		for (it2 = possibleState.begin(); it2 != possibleState.end(); ++it2) {
			if (*it2 == popQueue) {
				find = true;
			}
		}
		// possibleState�� ������ push��, bfsQueue�� next_state�� push��.
		if (!find) {
			possibleState.push_back(popQueue);

			for (it = elements.begin(); it != elements.end(); ++it) {
				if (it->state == popQueue) {
					bfsQueue.push(it->next_state);
				}
			}
		}
	}

	// build ���� ���� --> final state�� ���ް����Ѱ�?
	bool build = false;
	for (it3 = accept_states.begin(); it3 != accept_states.end(); ++it3){
		for (it2 = possibleState.begin(); it2 != possibleState.end(); ++it2) {
			if (*it3 == *it2) {
				build = true;
				break;
			}
		}
	}


	// build���� �� ��� fsa���� �� true return, �ȵǸ� false�� return.
	LOG << "num_elements: " << elements.size()
		<< ", accept_states: " << accept_states.size() << endl;
	if (build) {
		for (it = elements.begin(); it != elements.end(); ++it) {
			fsa->elements.push_back(*it);
		}
		for (it3 = accept_states.begin(); it3 != accept_states.end(); ++it3) {
			fsa->accept_state.push_back(*it3);
		}
		return true; 
	}
	else { return false; }
}

