#pragma once

#include "chunk.h"
#include "value.h"
#include "table.h"
#include "obj.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
	ObjFunction* function;
	uint8_t* ip;
	Value* slots;
} CallFrame;


typedef struct {
	Chunk* chunk;
	CallFrame frames[FRAMES_MAX];
	int frameCount;
	Value stack[STACK_MAX];
	Value* stackTop;
	Table globals;
	Table strings;
	Obj* objects;
} VM;

extern VM vm;

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;


void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();
