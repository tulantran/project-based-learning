#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <assert.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPT_ERR "tsp: unknown or poorly formatted option -%c\n"

Path *best;   // Global pointer for the best path
Path *current; // Global pointer for the current path

void dfs(uint32_t vertex, Graph *g);

int main(int argc, char **argv) {
    // HANDLE OPTIONS AND FILE IO
    bool directed = false; // Flag to check if the graph is directed
    FILE *infile = stdin;  // Default input file is stdin
    FILE *outfile = stdout; // Default output file is stdout
    int opt;

    // Process command-line arguments
    while ((opt = getopt(argc, argv, "i:o:dh")) != -1) {
        switch (opt) {
        case 'i':
            // Open the specified input file
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                fprintf(stderr, OPT_ERR, opt);
                exit(1);
            }
            break;

        case 'o':
            // Open the specified output file
            outfile = fopen(optarg, "w");
            if (outfile == NULL) {
                fprintf(stderr, OPT_ERR, opt);
                exit(1);
            }
            break;
        case 'd': 
            directed = true; // Set graph to directed
            break;
        case 'h':
            // Print help message and exit
            printf("Usage: tsp [options]\n\n"
                   "-i infile    Specify the input file path containing the cities and edges\n"
                   "             of a graph. If not specified, the default input should be\n"
                   "             set as stdin.\n\n"
                   "-o outfile   Specify the output file path to print to. If not specified,\n"
                   "             the default output should be set as stdout.\n\n"
                   "-d           Specifies the graph to be directed.\n\n"
                   "-h           Prints out a help message describing the purpose of the\n"
                   "             graph and the command-line options it accepts, exiting the\n"
                   "             program afterwards.\n");
            exit(0);
        default:
            // Handle unknown or malformed options
            fprintf(stderr, OPT_ERR, optopt);
            exit(1);
            break;
        }
    }

    // READ IN FROM FILE
    uint32_t num_vertices, num_edges;

    // READ VERTICES
    if (fscanf(infile, "%u\n", &num_vertices) != 1) {
        // Error handling for vertex count
        fprintf(stderr, "tsp: error reading number of vertices\n");
        exit(1);
    }

    // Create the graph with specified number of vertices and directionality
    Graph *gr = graph_create(num_vertices, directed);

    // Read vertex names and add them to the graph
    for (uint32_t i = 0; i < num_vertices; i++) {
        char vertexname[100];
        fgets(vertexname, sizeof(vertexname), infile);
        vertexname[strlen(vertexname) - 1] = '\0'; // Remove newline character
        graph_add_vertex(gr, vertexname, i);
    }

    // READ EDGES
    if (fscanf(infile, "%u", &num_edges) != 1) {
        // Error handling for edge count
        fprintf(stderr, "tsp: must provide number of edges\n");
        exit(1);
    } else {
        // Read and add edges to the graph
        for (uint32_t i = 0; i < num_edges; i++) {
            uint32_t start, end, weight;
            if (fscanf(infile, "%u%u%u", &start, &end, &weight) != 3) {
                fprintf(stderr, "tsp: error reading edge\n");
                exit(1);
            }
            graph_add_edge(gr, start, end, weight);
        }
    }

    // Close the input file if it is not stdin
    if (infile != stdin) {
        fclose(infile);
    }

    // Initialize paths for tracking best and current paths
    best = path_create(num_vertices + 1);
    current = path_create(num_vertices + 1);

    // Start depth-first search from the start vertex
    dfs(START_VERTEX, gr);

    // If no valid path is found, output an error message
    if (path_distance(best) == 0) {
        fprintf(outfile, "No path found!\n");
    } else {
        // Print the best path found
        path_print(best, outfile, gr);
    }

    // Close the output file if it is not stdout
    if (outfile != stdout) {
        fclose(outfile);
    }

    // Free dynamically allocated memory
    graph_free(&gr);
    path_free(&best);
    path_free(&current);
}

// Depth-First Search (DFS) to explore paths
void dfs(uint32_t vertex, Graph *g) {
    graph_visit_vertex(g, vertex); // Mark vertex as visited
    path_add(current, vertex, g);  // Add vertex to the current path

    // If the current path is shorter than the best path or no best path exists
    if (path_distance(current) < path_distance(best) || path_distance(best) == 0) {
        // If all vertices are visited and there's an edge back to the start vertex
        if (path_vertices(current) == graph_vertices(g)
            && graph_get_weight(g, vertex, START_VERTEX) > 0) {
            // Complete the cycle by adding the start vertex
            path_add(current, START_VERTEX, g);

            // Update the best path if necessary
            if (path_distance(current) < path_distance(best) || path_distance(best) == 0) {
                path_copy(best, current);
            }

            // Remove the start vertex from the current path
            path_remove(current, g);
        }

        // Explore adjacent vertices
        for (uint32_t next_vertex = 0; next_vertex < graph_vertices(g); next_vertex++) {
            if (!graph_visited(g, next_vertex) // If the vertex has not been visited
                && graph_get_weight(g, vertex, next_vertex) > 0) { // And an edge exists
                dfs(next_vertex, g); // Recursive call to visit the next vertex
            }
        }
    }

    // Backtrack: unvisit the vertex and remove it from the current path
    graph_unvisit_vertex(g, vertex);
    path_remove(current, g);
}
