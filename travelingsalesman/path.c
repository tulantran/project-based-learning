#include "graph.h"  // Include header for graph-related functions and structures
#include "stack.h"  // Include header for stack-related functions and structures

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef PATH
#define PATH

// Path structure definition
struct path;
typedef struct path {
    uint32_t total_weight;  // Total weight (distance) of the path
    Stack *vertices;        // Stack containing the vertices in the path
} Path;

// Function to create and initialize a Path
Path *path_create(uint32_t capacity) {
    Stack *pstack = stack_create(capacity);  // Create a stack for vertices
    Path *p = (Path *) malloc(sizeof(Path)); // Allocate memory for the Path
    p->total_weight = 0;                     // Initialize total weight to 0
    p->vertices = pstack;                    // Assign the created stack
    return p;
}

// Function to free all resources associated with a Path
void path_free(Path **pp) {
    stack_free(&((*pp)->vertices));  // Free the stack of vertices
    free(*pp);                       // Free the Path structure itself
    *pp = NULL;                      // Set the pointer to NULL
}

// Function to get the number of vertices in the path
uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);  // Return the size of the stack
}

// Function to get the total weight (distance) of the path
uint32_t path_distance(const Path *p) {
    return p->total_weight;
}

// Function to add a vertex to the path and update the total weight
void path_add(Path *p, uint32_t val, const Graph *g) {
    uint32_t topvert;
    if (stack_peek(p->vertices, &topvert)) {  // Get the top vertex in the stack
        // Ensure there is a valid edge weight between the vertices
        // assert(graph_get_weight(g, topvert, val) != 0);
        p->total_weight += graph_get_weight(g, topvert, val);  // Update weight
    }
    stack_push(p->vertices, val);  // Push the new vertex onto the stack
}

// Function to remove the most recent vertex from the path and adjust the weight
void path_remove(Path *p, const Graph *g) {
    uint32_t removedv;
    if (stack_pop(p->vertices, &removedv)) {  // Pop the top vertex from the stack
        uint32_t topv;
        if (stack_peek(p->vertices, &topv)) { // Get the new top vertex
            p->total_weight -= graph_get_weight(g, topv, removedv); // Adjust weight
        }
    }
}

// Function to copy one Path to another
void path_copy(Path *dst, const Path *src) {
    stack_copy(dst->vertices, src->vertices);  // Copy the stack of vertices
    dst->total_weight = src->total_weight;     // Copy the total weight
}

// Function to print the path and its details to a file or standard output
void path_print(const Path *p, FILE *f, const Graph *g) {
    Stack *reversed = stack_create(stack_size(p->vertices));  // Create a stack to reverse the path
    uint32_t count = stack_size(p->vertices);                 // Get the number of vertices in the path

    // Reverse the stack of vertices for printing
    for (uint32_t i = 0; i < count; i++) {
        uint32_t popped;
        assert(stack_pop(p->vertices, &popped));  // Pop from the original stack
        assert(stack_push(reversed, popped));     // Push onto the reversed stack
    }

    fprintf(f, "Path:\n");
    for (uint32_t i = 0; i < count; i++) {
        uint32_t popped;
        stack_pop(reversed, &popped);            // Pop from the reversed stack
        fprintf(f, "%s\n", graph_get_vertex_name(g, popped));  // Print vertex name
    }
    stack_free(&reversed);                       // Free the reversed stack

    fprintf(f, "Total Distance: %u\n", p->total_weight);  // Print total weight
}

#endif
