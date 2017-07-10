// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"

void makeState(int start, int end, std::string subexp, RegExpMatcher* regexp_matcher) {
	int state = start * 10;
	int saveState;
	std::string str = subexp;

	// For "or" regular expression, it needs to devide state number..
	std::vector<FSATableElement>::const_iterator it;
	for (it = regexp_matcher->fsa.elements.begin(); it != regexp_matcher->fsa.elements.end(); ++it) {
		if (it->state == state) {
			state++;
		}
		state++;
	}

	FSATableElement q;
	q.state = start;
	q.str = "";
	q.next_state = state;
	regexp_matcher->fsa.elements.push_back(q);

	for (int i = 0; i < subexp.size(); i++) {
		// case of " character "
		if (isalnum(subexp[i]) || subexp[i] == '_' || subexp[i] == '.') {
			saveState = state;
			FSATableElement t;
			t.state = state;
			if (subexp[i] == '.') {
				t.str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
			} else {
				t.str = subexp[i];
			}
			t.next_state = ++state;
			regexp_matcher->fsa.elements.push_back(t);
		}
		// case of " * " : add epsilon to nfa.
		else if (subexp[i] == '*') {
			FSATableElement t;
			t.state = saveState;
			t.str = "";
			t.next_state = state;
			regexp_matcher->fsa.elements.push_back(t);
			t.state = state;
			t.next_state = saveState;
			regexp_matcher->fsa.elements.push_back(t);
		}
		// case of " ( ) " : recursion
		else if (subexp[i] == '(') {
			saveState = state;
			int pStack = 1;	// For matching ( )
			int close;
			for (int j = i+1; j < sizeof(subexp); j++) {
				if (subexp[j] == '(') {
					pStack++;
				}
				if (subexp[j] == ')') {
					pStack--;
					if (pStack == 0) {
						state++;
						close = j;
						break;
					}
				}
			}
			makeState(saveState, state, str.substr(i + 1, close - i - 1), regexp_matcher);
			i = close;
		}
		// case of " [ ] "
		else if (subexp[i] == '[') {
			saveState = state;
			int pStack = 1;
			int close;
			for (int j = i + 1; j < sizeof(subexp); j++) {
				if (subexp[j] == '[') {
					pStack++;
				}
				if (subexp[j] == ']') {
					pStack--;
					if (pStack == 0) {
						state++;
						close = j;
						break;
					}
				}
			}
			FSATableElement t;
			t.state = saveState;
			t.str = str.substr(i+1,close-i-1);
			t.next_state = state;
			regexp_matcher->fsa.elements.push_back(t);
			i = close;
		}
		// case of " | "
		else if (subexp[i] == '|') {
			makeState(start, end, str.substr(i + 1, str.size() - i), regexp_matcher);
			i = str.size() - 1;
		}
	}
	
	q.state = state;
	q.str = "";
	q.next_state = end;
	regexp_matcher->fsa.elements.push_back(q);
}

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
	std::string exp = regexp;
	makeState(1, 2, exp, regexp_matcher);
	regexp_matcher->fsa.accept_state.push_back(2);

	std::vector <FSATableElement> v = regexp_matcher->fsa.elements;
	/*
	for (int i = 0; i < v.size(); i++) {
		printf("state : %d, next : %d, str : %s\n", v[i].state, v[i].next_state, v[i].str.c_str());
	}*/
	
  return true;
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
  return RunFSA(regexp_matcher.fsa, str);
}

