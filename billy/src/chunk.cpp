#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

static void initLineArray(LineArray* array)
{
    array->capacity = 0;
    array->count = 0;
    array->lines = NULL;
}

static Line* previusLine(LineArray* array)
{
    if (array->count == 0)
        return &array->lines[0];
    else
        return &array->lines[array->count - 1];
}

void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
    initLineArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    LineArray* lines = &chunk->lines;
    
    if (lines->capacity < lines->count + 1)
    {
        int oldCapacity = lines->capacity;
        lines->capacity = GROW_CAPACITY(oldCapacity);
        lines->lines = GROW_ARRAY(Line, lines->lines, oldCapacity, lines->capacity);
    }
    

    if (previusLine(lines)->value == line)
    {
        previusLine(lines)->count++;
    }
    else {
        lines->lines[lines->count].value= line;
        lines->lines[lines->count].count = 1;
        lines->count++;
    }
}

int addConstant(Chunk* chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}


void freeChunk(Chunk* chunk) {
    reallocate(chunk->code, sizeof(uint8_t) * chunk->capacity, 0);
    freeValueArray(&chunk->constants);
    FREE_ARRAY(Line, chunk->lines.lines, chunk->lines.capacity);
    initChunk(chunk);
}

int getLine(Chunk* chunk, int index)
{
    int counter = 0;
    for (int i = 0; i < chunk->lines.count; i++)
    {
        for (int j = 0; j < chunk->lines.lines[i].count; j++)
        {
            if (counter == index)
                return chunk->lines.lines[i].value;
            counter++;
        }
    }
    return -1;
}
