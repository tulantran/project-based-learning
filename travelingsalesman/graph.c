#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Graph structure definition
typedef struct graph {
    uint32_t vertices;       // Number of vertices in the graph
    bool directed;           // Boolean indicating if the graph is directed
    bool *visited;           // Array to track visited vertices
    char **names;            // Array of vertex names
    uint32_t **weights;      // Adjacency matrix for edge weights
} Graph;

// Function to create and initialize a graph
Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;

    // Allocate memory for visited array, initialized to false
    g->visited = calloc(vertices, sizeof(bool));

    // Allocate memory for names array, initialized to NULL
    g->names = calloc(vertices, sizeof(char *));

    // Allocate memory for adjacency matrix (weights array)
    g->weights = calloc(vertices, sizeof(g->weights[0]));
    for (uint32_t i = 0; i < vertices; ++i) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }
    return g;
}

// Function to free all resources associated with a graph
void graph_free(Graph **gp) {
    for (uint32_t i = 0; i < (*gp)->vertices; ++i) {
        free((*gp)->weights[i]);   // Free each row of the adjacency matrix
        (*gp)->weights[i] = NULL;

        free((*gp)->names[i]);     // Free each vertex name
        (*gp)->names[i] = NULL;
    }

    free((*gp)->weights);          // Free adjacency matrix pointer
    (*gp)->weights = NULL;

    free((*gp)->names);            // Free names array pointer
    (*gp)->names = NULL;

    free((*gp)->visited);          // Free visited array
    (*gp)->visited = NULL;

    free(*gp);                     // Free the graph structure itself
    *gp = NULL;
}

// Function to get the number of vertices in the graph
uint32_t graph_vertices(const Graph *g) {
    return g->vertices;
}

// Function to add an edge to the graph
void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    assert(end < g->vertices);     // Ensure end vertex is within bounds
    assert(start < g->vertices);  // Ensure start vertex is within bounds
    g->weights[start][end] = weight;

    // If the graph is undirected, make the edge symmetric
    if (!(g->directed)) {
        g->weights[end][start] = weight;
    }
}

// Function to get the weight of an edge
uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return g->weights[start][end];
}

// Function to mark a vertex as visited
void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

// Function to mark a vertex as unvisited
void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

// Function to check if a vertex has been visited
bool graph_visited(const Graph *g, uint32_t v) {
    return g->visited[v];
}

// Function to get the array of vertex names
char **graph_get_names(const Graph *g) {
    return g->names;
}

// Function to add or update a vertex name
void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    assert(v < g->vertices);       // Ensure vertex index is within bounds
    if (g->names[v])
        free(g->names[v]);         // Free previous name if it exists
    g->names[v] = strdup(name);    // Duplicate the new name
    g->visited[v] = false;         // Mark the vertex as unvisited
}

// Function to get the name of a vertex
const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    return g->names[v];
}

// Function to print the graph's adjacency matrix and vertex names
void graph_print(const Graph *g) {
    printf("Graph (Adjacency Matrix):\n");

    // Print vertex names
    printf("Vertex Names:\n");
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("%u: %s\n", i, g->names[i]);
    }
    printf("\n");

    // Print adjacency matrix
    printf("Adjacency Matrix:\n");
    printf("   ");
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("%u ", i);
    }
    printf("\n");
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("%u: ", i);
        for (uint32_t j = 0; j < g->vertices; j++) {
            printf("%u ", g->weights[i][j]);
        }
        printf("\n");
    }
}
