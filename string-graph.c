#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string-graph.h"

/*This whole program will greate a graph with vertices and edges that will 
  connect each vertices with a certain weight. This program will store graphs 
  with dynamically-allocated memory. It also store functions that allow 
  removing components of graphs as well as an entire graph. Since this program 
  dynamically-allocated memory when created a graph, it will also free any
  allocated memory that is no longer needed. */ 

/* This function will initialize its graph parameter with no vertices or edges,
   which is an empty graph. */
void graph_init(String_graph *const graph) {
  if(graph != NULL) {
    graph->num_vertices = 0;
    graph->vertex_array = NULL;
  }
}

/* Check if there's a vertex with the parameter name already exists by
   using string library function. Return 1 if the function finds name, 
   otherwise, return 0. */
static int compare_vertex_name(const String_graph *const graph, 
                               const char parameter_name[]) {
  int i, validity = 0;

  if (graph!= NULL && parameter_name != NULL) {
    for (i = 0; i < graph->num_vertices; i++) {
      if (strcmp(graph->vertex_array[i]->name, parameter_name) == 0) {
        validity = 1;
      }
    }
  }

  return validity;
}

/* This function adds a new vertex with name new_vertex to its parammeter graph
   that has no edges. If the vertex with name new_vertex already exist in 
   the graph, return 0 without modifying anything, otherwise return 1 after 
   adding vertex to the graph */
short add_vertex_to_graph(String_graph *const graph, const char new_vertex[]) {
  short validity= 1;

  /* Check if there's a vertex with the parameter name already exists by
     using helper function */
  if (graph!= NULL && new_vertex != NULL) {
    if (compare_vertex_name(graph, new_vertex)) {
      validity = 0;
    }

    /* Create a new vertex with its name to a newly-created 
       dynamically-allocated memory, if the parameter vertex name does not 
       exist in the graph */
    if (validity) {
      Vertex *new_ver = malloc(sizeof(Vertex));
      /* Create one more space at the end to add new vertex into the graph */
      new_ver->name = malloc(strlen(new_vertex) + 1);
      strcpy(new_ver->name, new_vertex);
      /* The vertex should not have any edges yet, can be added later */
      new_ver->edge_list = NULL;
      /* If the memory location is empty */
      if (graph->vertex_array == NULL) {
        graph->vertex_array = malloc(sizeof(Vertex *));
      }
      /* Resize the vertex_array, if the memory location is occupied */ 
      else {
        graph->vertex_array = realloc(graph->vertex_array, 
                                      (graph->num_vertices + 1) * sizeof(Vertex *));
      }

      /* Insert a new vertex to a graph with newly-created dynamically 
         allocated memory, and increase its number of vertices */
      graph->vertex_array[graph->num_vertices] = new_ver;
      graph->num_vertices++;
    }
  } else {
    validity = 0;
  }

  return validity;
}

/* This function returns the number of vertices that are currently in the 
   parameter graph */
short vertex_count(const String_graph *const graph) {
  short num_vertices = 0;

  /* Get the number of vertices that parameter graph contains */
  if (graph != NULL) {
    num_vertices = graph->num_vertices;
  }

  return num_vertices;
}

/* This function returns 1 if its parameger graph contains a vertex with name
   name, and 0 if not */
short isa_vertex(const String_graph *const graph, const char name[]) {
  return compare_vertex_name(graph, name);
}

/* This function should return a newly-created dynamically-allocated array, of
   pointers to characters, where the pointers point to the copies of the names 
   of the vertices in the graph. The names pointed to by the pointers must
   be in an alphabetical order */
char **get_vertex_list(const String_graph *const graph) {
  char **ch;
  int i, sorted = 0;
  Vertex *temp;

  if (graph != NULL) {
    /* Allocate memory for the pointers that point to vertex names */
    ch = malloc((graph->num_vertices + 1) * sizeof *ch);

    /* Sort the vertex array until it is completely sorted */
    while (!sorted) {
      sorted = 1;
      /*  Using graph->num_vertices-1 to not go beyond the array */
      for (i = 0; i < graph->num_vertices - 1; i++) {
        if(strcmp(graph->vertex_array[i]->name, 
                  graph->vertex_array[i+1]->name) > 0) {
          temp = graph->vertex_array[i];
          graph->vertex_array[i] = graph->vertex_array[i+1];
          graph->vertex_array[i+1] = temp;
          sorted = 0;
        }  
      }
    }

    for (i = 0; i < graph->num_vertices; i++) {
      /* Allocate memory for the name string */
      ch[i] = malloc(strlen(graph->vertex_array[i]->name) + 1);   
      /* Copy the name string into the new memory location */
      strcpy(ch[i], graph->vertex_array[i]->name);
    }

    /* Make the last element of the array to NULL */
    ch[graph->num_vertices] = NULL;
  }

  return ch;
}

/* It is a helper function that finds the index of given vertex in parameter 
   graph */
static int find_index_of_vertex(const String_graph *const graph,
                                const char target[]) {
  int i, target_index = -1;

  /* Loops through the vertices to find the index of a target vertex */
  for (i = 0; i < graph->num_vertices; i++) {
    if (strcmp(graph->vertex_array[i]->name, target) == 0) {
      target_index = i;
    }
  }

  return target_index;
}

/* This function add edges to its parameter graph with weight that goes
   from vertex source to dest. If the edge is added, return 1. */
short create_graph_edge(String_graph *const graph, const char source[],
                        const char dest[], int weight) {
  short validity = 0;
  int source_index = -1, dest_index = -1;
  Vertex *dest_vertex, *source_vertex;
  Edge *edge;

  /* If the weight is 0 or any of the vertices or parameter graph are null,
     return 0 */
  if (graph != NULL && source != NULL && dest != NULL && weight >= 0) {
    /* Find the index of source and dest vertices and assign values to 
       each vertices using helper function */
    source_index = find_index_of_vertex(graph, source);

    /* If the vertex names source does not exist in the graph, create one */
    if (source_index < 0) {
      add_vertex_to_graph(graph, source);
      source_index = graph->num_vertices - 1;
      source_vertex = graph->vertex_array[source_index];
    } else {
      source_vertex = graph->vertex_array[find_index_of_vertex(graph, source)];
    }
    
    dest_index = find_index_of_vertex(graph, dest);

    /* If the vertex names dest does not exist in the graph, create one */
    if (dest_index < 0) {
      add_vertex_to_graph(graph, dest);
      dest_index = graph->num_vertices - 1;
      dest_vertex = graph->vertex_array[dest_index];
    } else {
      dest_vertex = graph->vertex_array[find_index_of_vertex(graph, dest)];
    }
   
    /* Check if an edge already exists between source and dest */
    edge = source_vertex->edge_list;
    while (edge != NULL) {
      if (edge->destination_vertex == dest_vertex) {
        edge->weight = weight;
        validity = 1;
      }
      edge = edge->next_edge;
    }

    /* If the edge does not exist, create a new edge in the graph */
    if (!validity) {
      edge = malloc(sizeof(Edge));
      edge->weight = weight;
      edge->destination_vertex = dest_vertex;
      /* Set the next edge of 'edge' to the current first edge in the 
         edge list */
      edge->next_edge = source_vertex->edge_list;
      /* Update the edge list of 'source_vertex' to make 'edge' the first edge
         in the edge list */
      source_vertex->edge_list = edge;
      validity = 1;
    }
  }

  return validity;
}

/* This function should return the weight of the edge in its parameter graph
   that goes from vertex source to dest. It should return -1 in special 
   cases, such as parameter vertices do not exist or there is no edge between
   vertices, or if any parameters are null */
short get_graph_edge_weight(const String_graph *const graph,
                            const char source[], const char dest[]) {
  short weight = -1;
  int validity = 0, source_index = -1, dest_index = -1;
  Vertex *source_vertex= NULL;
  Edge *edge;

  /* Check that graph, source, and dest are not null */
  if (graph != NULL && source != NULL && dest != NULL) {
    /* Find the indices of the source and dest vertices */
    source_index = find_index_of_vertex(graph, source);
    dest_index = find_index_of_vertex(graph, dest);

    /* If both source and dest are found, initialize vertices */
    if (source_index >= 0 && dest_index >= 0) {
      source_vertex = graph->vertex_array[source_index];
      validity = 1;
    }

    /* If both vertices are valid, loop through the edges of the source vertex
       to find the edge that connects to the dest vertex */
    if (validity) {
      edge = source_vertex->edge_list;
      while (edge != NULL && validity == 1) {
        /* If an edge connecting source to dest is found, set weight and
           validity, and exit the loop */
        if (strcmp(edge->destination_vertex->name, dest) == 0) {
          weight = edge->weight;
          validity = 0;
        }
        edge = edge->next_edge;
      }
    }
  }

  /* Return weight (-1 if no edge found) */
  return weight;
}

/* This function should return the number of neighbors of vertex in its 
   parameter graph. Meaning return the number of its outgoing edges. Return -1
   if there is no vertex with name parameter vertex or any parameter is NULL */
short neighbor_count(const String_graph *const graph, const char vertex[]) {
  short num_neighbors = -1;
  int vertex_index = -1;
  Vertex *temp_vertex;
  Edge *edge;

  if (graph != NULL && vertex != NULL) {
    vertex_index = find_index_of_vertex(graph, vertex);

    /* If the vertex exists, loop through its list of edges and count them 
       all to return the number of its neighbors */
    if (vertex_index != -1) {
      temp_vertex = graph->vertex_array[vertex_index];
      edge = temp_vertex->edge_list;
      num_neighbors = 0;
      while (edge != NULL) {
        num_neighbors++;
        edge = edge->next_edge;
      }
    }
  }

  return num_neighbors;
}

/* This is a helper function that frees all the dynamically-allocated memory of
   the strings in the array names, then free the array names itself. */
void free_vertex_name_list(char **const names) { 
  int i;

  if (names != NULL) {
    for (i = 0; names[i] != NULL ; i++) {
      /* Free memory for all names string */
      free(names[i]);   
    }
    free(names);
  }
} 

/* This function return a newly-created dynamically-allocated array of pointers
   to characters, where the pointers point to copies of the names of neighbors
   of the parameter vertex. If the graph is NULL or the parameter vertex does 
   not exist in the graph, the function just return NULL. */
char **get_neighbor_names
(const String_graph *const graph, const char vertex[]) {
  char  *temp, **new_arr = NULL;
  int count, i, vertex_index, sorted = 0;
  Vertex *neighbor;
  Edge *edge;

  if (graph != NULL && vertex != NULL) {
    vertex_index = find_index_of_vertex(graph, vertex);

    if (vertex_index != -1) {
      /* Each name is stored in a newly-created dynamically-allocated character
         array */
      count = neighbor_count(graph, vertex);
      new_arr = malloc((count + 1) * sizeof(char *));

      if (new_arr != NULL) {
        /* Loop through the vertex's edge list and 
           add the neighbor names to new_arr */
        edge = graph->vertex_array[vertex_index]->edge_list;
        i = 0;

        while (edge != NULL) {
          neighbor = edge->destination_vertex;
          new_arr[i] = malloc(sizeof(char) * (strlen(neighbor->name) + 1));
          strcpy(new_arr[i], neighbor->name);
          i++;
          edge = edge->next_edge;
        }

        /* Sort the vertex array until it is completely sorted */
        while (!sorted) {
          sorted = 1;
          /*  Using graph->num_vertices-1 to not go beyond the array */
          for (i = 0; i < count - 1; i++) {
            if(strcmp(new_arr[i], new_arr[i+1]) > 0) {
              temp = new_arr[i];
              new_arr[i] = new_arr[i+1];
              new_arr[i+1] = temp;
              sorted = 0;
            }  
          }
        }
        /* Setting the last element of vertex array NULL */
        new_arr[count] = NULL;
      }      
    }
  }

  if (new_arr == NULL) {
    return NULL;
  }
  return new_arr;
}

/* This function removes the edges going from source vertex to dest vertex in
   its parameter graph. If either the vertices are not part of the graph, or
   if there is no edge going from source to dest, the function just return 0,
   otherwise 1. */
int remove_graph_edge(String_graph *const graph, const char source[], 
                      const char dest[]) {
  int found = 0, source_index, removed = 0;
  Edge *curr, *prev;

  if (graph != NULL && source != NULL && dest != NULL) {
    /* Find the source vertex */
    source_index = find_index_of_vertex(graph, source);

    if (source_index != -1) {
      /* Assign values to two edges after we know that the source vertex exist */
      prev = NULL;
      curr = graph->vertex_array[source_index]->edge_list;

      /* This loop removes edge from source to dest */
      while (curr != NULL && !removed) {
        if (strcmp(curr->destination_vertex->name, dest) == 0) {
          /* Dest edge was found, then remove it */
          if (prev == NULL) { /* When the first element of the list is dest */
            graph->vertex_array[source_index]->edge_list = curr->next_edge;
          } else { /* Using linked list way to remove elements */
            prev->next_edge = curr->next_edge;
          }

          /* Freeing memory that function does not need anymore */
          free(curr);
          found = 1;
          /* If we removed the parameter edge, we exit the while loop */
          removed = 1;
        } else { /* If we haven't find the edge, keep searching for one */
          prev = curr;
          curr = curr->next_edge;
        }
      }
    }
  }

  return found;
}

/* This function removes vertex from the graph and return 1. It also removes
   all the outgoing and incoming edges. Otherwise, return 0 without changing
   anything. */
int remove_vertex_from_graph(String_graph *const graph, const char vertex[]) {
  int vertex_index, found = 0, i, removed = 0;
  Edge *curr, *prev = NULL;

  if (graph != NULL && vertex != NULL) {
    vertex_index = find_index_of_vertex(graph, vertex);

    if (vertex_index != -1) {
      curr = graph->vertex_array[vertex_index]->edge_list;

      /* Remove all outgoing edges from the vertex */
      while (curr != NULL) {
        prev = curr;
        curr = curr->next_edge;
        free(prev);
      }

      /* Remove all incoming edges to the vertex */
      for (i = 0; i < graph->num_vertices; i++) {
        /* Exclude the parameter vertex itself, since we already removed all
           the outgoing edges */
        if (i != vertex_index) {
          prev = NULL;
          curr = graph->vertex_array[i]->edge_list;
          while (curr != NULL && !removed) {
            if (strcmp(curr->destination_vertex->name, vertex) == 0) {
              if (prev == NULL) {
                graph->vertex_array[i]->edge_list = curr->next_edge;
              } else {
                prev->next_edge = curr->next_edge;
              }
              
              free(curr);
              /* If we removed the parameter edge, we exit the while loop */
              removed = 1;
            } else { /* If we haven't find the edge, keep searching for one */
              prev = curr;
              curr = curr->next_edge;
            }
          }
        }
      }
      
      /* Remove the vertex and free all the dynamically-allocated memory used
         by parameter vertex */
      free(graph->vertex_array[vertex_index]->name);
      free(graph->vertex_array[vertex_index]);
      graph->vertex_array[vertex_index] = NULL;
      
      /* Shift all the vertices to the left, after removed vertex */
      for (i = vertex_index; i < graph->num_vertices -1; i++) {
        graph->vertex_array[i] = graph->vertex_array[i + 1];
      }
      
      graph->vertex_array[graph->num_vertices - 1] = NULL;
      
      /* Decrease the number of vertics in the graph */
      graph->num_vertices--;
      found = 1;
    }
  }
  
  return found; 
}

/* This function deallocate all dynamically-allocated memory that is used in
   the parameter graph and release the graph and all its data in the process. */
void graph_delete(String_graph *const graph) {
  int i;
  Edge *curr, *temp;

  if (graph != NULL) {
    /* Free all dynamically-allocated memory used by the parameter graph */
    for (i = 0; i < graph->num_vertices; i++) {
      if (graph->vertex_array[i] != NULL) {
        curr = graph->vertex_array[i]->edge_list;
      
        /* Deallocate all edges */
        while (curr != NULL) {
          temp = curr;
          curr = curr->next_edge;
          free(temp);
        }
      
        /* Deallocate all vertices */
        free(graph->vertex_array[i]->name);
        free(graph->vertex_array[i]);
        graph->vertex_array[i] = NULL;
      }
    }

    /* Deallocate the vertex array in graph */
    free(graph->vertex_array);
    graph->vertex_array = NULL;
  }
}
