#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "obj.h"
#include "value.h"
#include "vm.h"
#include "table.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
	Obj* object = (Obj*)reallocate(NULL, 0, size);
	object->type = type;
	object->next = vm.objects;
	vm.objects = object;
	return object;
}

static ObjString* allocateString(int length, uint32_t hash) {
	ObjString* string = (ObjString*)allocateObject(sizeof(ObjString), OBJ_STRING);
	string->chars = ALLOCATE(char, length + 1);
	string->length = length;
	string->hash = hash;
	tableSet(&vm.strings, string, NIL_VAL);
	return string;
}

static uint32_t hashString(const char* key, int length) {
	uint32_t hash = 2166136261u;
	for (int i = 0; i < length; i++) {
		hash ^= (uint8_t)key[i];
		hash *= 16777619;
	}
	return hash;
}

ObjString* objString(const char* chars, int length) {
	uint32_t hash = hashString(chars, length);
	ObjString* interned = tableFindString(&vm.strings, chars, length,
		hash);
	if (interned != NULL) return interned;
	ObjString* string = allocateString(length, hash);
	memcpy(string->chars, chars, length);
	string->chars[length] = '\0';
	return string;
}


void printObject(Value value)
{
	switch (OBJ_TYPE(value)) {
	case OBJ_STRING:
		printf("%s", AS_CSTRING(value));
		break;
	}
}

