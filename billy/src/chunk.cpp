#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
}

void writeChunk(Chunk* chunk, uint8_t byte) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = (uint8_t*)reallocate(chunk->code, sizeof(uint8_t) * oldCapacity, sizeof(uint8_t) * chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}


void freeChunk(Chunk* chunk) {
    reallocate(chunk->code, sizeof(uint8_t) * chunk->capacity, 0);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}