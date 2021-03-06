// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

#include "fsa.h"

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
	FiniteStateAutomaton fsa;
	int numState;
};


void makeState(int start, int end, std::string subexp, RegExpMatcher* regexp_matcher);
// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

