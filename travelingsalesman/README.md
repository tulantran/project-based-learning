# Traveling Salesperson Problem (TSP) Solver

This program solves the Traveling Salesperson Problem (TSP) for a given graph, finding the shortest path that visits every vertex exactly once and returns to the starting vertex. The graph can be directed or undirected, and the program reads input and output from files or standard streams.

## Features
- Supports directed or undirected graphs.
- Uses Depth-First Search (DFS) to find the shortest Hamiltonian cycle.
- Input and output via files or standard streams.
- Flexible configuration through command-line options.

## Usage
### Syntax
tsp [options]

### Options
- `-i <infile>`: Input file with the graph's vertices and edges. Defaults to `stdin` if not provided.
- `-o <outfile>`: Output file for results. Defaults to `stdout` if not provided.
- `-d`: Specifies the graph is directed. Defaults to undirected.
- `-h`: Displays help information and exits.

### Example
tsp -i graph.txt -o result.txt -d

## Input Format
1. **Number of vertices**: Integer specifying the number of vertices.
2. **Vertex names**: One name per line for each vertex.
3. **Number of edges**: Integer specifying the number of edges.
4. **Edges**: Three integers per line: `start_vertex`, `end_vertex`, and `weight`.

### Example Input
4  
A  
B  
C  
D  
5  
0 1 10  
1 2 15  
2 3 20  
3 0 25  
0 2 30  



