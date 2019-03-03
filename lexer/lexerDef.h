#ifndef _LEXERDEF
#define _LEXERDEF
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_SIZE 256
#define MAX_TOKEN_SIZE 150

#include<stdbool.h>

struct State{
	int val;
	int is_final_state;
	int is_retracting_state;
};

struct Transition{
	int next_state;
	int current_state;
	char character;
};

struct Token{
	char* val;
	int state;
	int len;
};



typedef struct Token Token;
typedef struct State State;
typedef struct Transition Transition;

struct DFA{
	State* states;
	Transition** transitions;
	char ** token_names;
};
typedef struct DFA DFA;
#endif
