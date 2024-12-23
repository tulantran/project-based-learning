#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef STACK
#define STACK

// Stack structure with a capacity, current top index, and an array of items
struct stack;

typedef struct stack {
    uint32_t capacity; // Maximum number of elements the stack can hold
    uint32_t top;      // Current position in the stack (index of the next free slot)
    uint32_t *items;   // Array holding stack elements
} Stack;

// Creates a stack with the given capacity
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = 0; // Initialize as empty
    s->items = calloc(s->capacity, sizeof(uint32_t)); // Allocate memory for items
    return s;
}

// Frees the memory used by the stack and ensures the pointer is set to NULL
void stack_free(Stack **sp) {
    if (sp != NULL && *sp != NULL) {
        if ((*sp)->items) {
            free((*sp)->items); // Free the array memory
            (*sp)->items = NULL;
        }
        free(*sp); // Free the stack structure memory
        *sp = NULL; // Nullify the pointer to prevent reuse
    }
}

// Pushes a value onto the stack, returns false if the stack is full
bool stack_push(Stack *s, uint32_t val) {
    if (s->top < s->capacity) {
        s->items[s->top] = val; // Add the value at the top position
        s->top++;               // Increment the top index
        return true;
    } else {
        return false; // Stack overflow
    }
}

// Pops a value from the stack, returns false if the stack is empty
bool stack_pop(Stack *s, uint32_t *val) {
    if (s->top > 0) {
        s->top--;               // Decrement the top index
        *val = s->items[s->top]; // Retrieve the top value
        return true;
    } else {
        return false; // Stack underflow
    }
}

// Peeks at the top value of the stack without removing it, returns false if empty
bool stack_peek(const Stack *s, uint32_t *val) {
    if (s->top > 0) {
        *val = s->items[s->top - 1]; // Get the value at the top
        return true;
    } else {
        return false; // Stack is empty
    }
}

// Checks if the stack is empty
bool stack_empty(const Stack *s) {
    return (s->top == 0);
}

// Checks if the stack is full
bool stack_full(const Stack *s) {
    return (s->top == s->capacity);
}

// Returns the current number of elements in the stack
uint32_t stack_size(const Stack *s) {
    return s->top;
}

// Copies the contents of one stack to another, assumes equal capacities
void stack_copy(Stack *dst, const Stack *src) {
    assert(dst->capacity == src->capacity); // Ensure capacities match
    dst->top = 0; // Reset destination stack
    for (uint32_t i = 0; i < src->top; ++i) {
        stack_push(dst, src->items[i]); // Push each item to the destination
    }
}

// Prints the contents of the stack as city names (requires a mapping array)
void stack_print(const Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i++) {
        fprintf(outfile, "%s\n", cities[s->items[i]]); // Print each city's name
    }
}

#endif

