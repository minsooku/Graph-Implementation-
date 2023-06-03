#if !defined(STRINGGRAPHDATASTRUCTURE_H)
#define STRINGGRAPHDATASTRUCTURE_H

typedef struct edge {
  int weight;
  struct vertex *destination_vertex;
  struct edge *next_edge;
} Edge;

typedef struct vertex {
  Edge *edge_list;
  char *name;
} Vertex;

typedef struct string_graph {
  int num_vertices;
  Vertex** vertex_array;
} String_graph;

#endif
