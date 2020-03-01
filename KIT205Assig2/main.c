#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "graph.h"

int main() {
	Stop* stops = get_stops(20);
	
	/*********************************************/
	Graph G;
	G.V = 20; //equal to the number of stops
	G.edges = malloc(sizeof(G.edges) * G.V); //edde list
	G.vertex_types = malloc(sizeof(int) * G.V); //array for the type each vertex has
	
	//initialise edge list
	for (int i = 0; i < G.V; i++) {
		G.edges[i].head = NULL;
	}

	//add an edge to each vertex except one to itself
	for (int i = 0; i < G.V; i++) {
		for (int j = 0; j < G.V; j++) {
			//if the vertex are the same, dont add an edge
			if (i != j) {
				//add edge to j from i
				EdgeNodePtr new_node = malloc(sizeof *new_node); 
				new_node->next = G.edges[i].head; 
				new_node->edge.to_vertex = j;
				new_node->edge.weight = distance(stops[i].x, stops[i].y, stops[j].x, stops[j].y);
				G.edges[i].head = new_node;
			}
		}
		//add type to vertex_types
		G.vertex_types[i] = stops[i].type;
	}
	/*********************************************/

	//Uncomment this line once you have completed Parts A and B
	printf("base score: %f\n", get_score(&G));
	
	// Uncomment these lines once you have completed Part C
	Graph P = prims_mst(&G, 0);
	printf("mst score: %f\n", get_score(&P));

	//Uncomment these lines once you have completed Part D
	Graph M = my_solution(&G);
	printf("my score: %f\n", get_score(&M));
	
	

	//Free dynamically allocated memory
	
	//Free memory for G, free all edge nodes, edge list and vertex_type array
	//free each edge
	for (int i = 0; i < G.V; i++) {
		EdgeNodePtr current = G.edges[i].head;
		while (current !=NULL) {
			EdgeNodePtr to_free = current;
			current = current->next;
			free(to_free);
		}
	}
	//free list and array
	free(G.edges);
	free(G.vertex_types);

	//Free memory for P, free all edge nodes, edge list and vertex_type array
	//free each edge
	for (int i = 0; i < P.V; i++) {
		EdgeNodePtr current = P.edges[i].head;
		while (current != NULL) {
			EdgeNodePtr to_free = current;
			current = current->next;
			free(to_free);
		}
	}
	//free list and array
	free(P.edges);
	free(P.vertex_types);

	//Free memory for M, free all edge nodes, edge list and vertex_type array
	for (int i = 0; i < M.V; i++) {
		//free each edge
		EdgeNodePtr current =M.edges[i].head;
		while (current != NULL) {
			EdgeNodePtr to_free = current;
			current = current->next;
			free(to_free);
		}
	}
	//free list and array
	free(M.edges);
	free(M.vertex_types);

	//free dynamically allocated memory for stops
	free(stops);

	printf("\npress enter to exit\n");
	getchar();
	return 0;
}