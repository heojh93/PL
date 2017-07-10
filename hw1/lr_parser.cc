// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
#include <vector>
#include <cstdbool>

#include "lr_parser.h"

#define DISABLE_LOG false
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser) {
	lr_parser->elements = elements;
	lr_parser->rules = rules;
	return true;

  //return false;
}

bool RunLRParser(const LRParser& lr_parser, const std::string& str) {
	string input = str;
	LRTableElement selectedTrans;
	vector<LRTableElement>::const_iterator it;

	vector<int>LRStack;
	LRStack.push_back(0);

	int stackTop;
	char inputTop;
	int action;

	while (true) {
		stackTop = LRStack.back();
		inputTop = input[0];
		
		//printf("stackTop : %d, inputTop %c\n", stackTop, inputTop);
		bool find = false;
		for (it = lr_parser.elements.begin(); it != lr_parser.elements.end(); ++it) {
			if (it->state == stackTop && it->symbol == inputTop) {
				selectedTrans = *it;
				find = true;
				break;
			}
		}
		action = (find) ? selectedTrans.action : INVALID;

		int gotoRule;
		int cnt;
		switch (action) {

		case INVALID:
			printf("Build Table has fault! : INVALID\n");
			return false;

		case SHIFT:
			LRStack.push_back(selectedTrans.next_state);
			input.erase(input.begin());
			break;

		case REDUCE:
			gotoRule = selectedTrans.next_state - 1;
			cnt = lr_parser.rules[gotoRule].num_rhs;
			//printf("reduce : goto Rule %d, cnt : %d\n", gotoRule, cnt);
			while (cnt--) LRStack.pop_back();

			find = false;
			for (it = lr_parser.elements.begin(); it != lr_parser.elements.end(); ++it) {
				if (it->state == LRStack.back() && it->symbol == lr_parser.rules[gotoRule].lhs_symbol) {
					selectedTrans = *it;
					find = true;
					break;
				}
			}
			if (find) {
				LRStack.push_back(selectedTrans.next_state);
			}
			else {
				printf("Build Table has fault! : GOTO\n");
				return false;
			}
			break;

		case ACCEPT:
			return true;
		}
		/*
		printf("stack : ");
		for (int i = 0; i < LRStack.size(); i++) {
			printf("%d ", LRStack[i]);
		}printf("\n");
		*/
	}
	
}

