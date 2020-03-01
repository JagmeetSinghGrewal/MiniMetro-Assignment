#include <float.h>
#include "graph.h"

/*
* Check Whether all vertices have been visited 
* Param int *visited, array of "booleans", 0 means false, 1 means true
* Param int loop, integer that tells how many times to loop through array
*/
int check_visited(int *visited, int loop) {
	int all_visited = 1; //variable that holds whether all vertices have been visited, initially set to true
	
	//if one vetex hasnt been visited, then set all_visited to false
	for (int i = 0; i < loop; i++ ) {
		//if vertex hasnt been visited, set all_visited to false
		if (visited[i]==0) {
			all_visited = 0;
		}
	}

	return all_visited;
}
/*
* Find the closest vertex, measured by some type of distance, whether that be actual distance or cost
* Param float *distances, array of distances, used to find the shortest/smallest distance
* Param int *visited, array of "booleans", 0 means false, 1 means true
* Param int loop, integer that tells how many times to loop through array
*/
int find_nearest(float *distances, int *visited,  int loop) {
	float nearest_distance = 1000001; //current closest distance, initially set to max 
	int nearest_vertex; //vertex that gave the closest distance
	for (int i = 0; i < loop; i++) {
		//if the vertex hasnt been visited, then check whether this vertex has a closer distance
		if (visited[i]==0) {
			//if the vertex has a closer distance, update to closer distance and store which vertex gave this distance
			if (nearest_distance > distances[i]) {
				nearest_distance = distances[i];
				nearest_vertex = i;
			}
		}
	}
	return nearest_vertex;
}

/*
* Find the distances to all vertices in a graph, from a starting vertex
* Param graph *, graph to get the distances for
* Param int start, which vertex to start from 
*/
float* dijkstra(Graph *self, int start) {
	/*******************************************************/
	int current_vertex; //vertex with the nearest distance
	float *distances = malloc(sizeof(float) * self->V); //array of distances, holds the closest distance from start node to a given vertex
	int *visited = malloc(sizeof(int)*self->V); //array of booleans, to check whether a given vertex has been visited yet

	//initialise distances to max distances and visited to false
	for (int i = 0; i < self->V; i++) {
		distances[i] = 1000000;
		visited[i] = 0;
	}


	//set the starting vertex's distance to 0
	distances[start] = 0;

	//find all closest distances until all vertices have been visited
	while (!check_visited(visited, self->V)) {
		current_vertex = find_nearest(distances,visited,self->V);	//find the current nearest vertex
		visited[current_vertex] = 1;	//set current nearest vertex to be visited

		//go through all edges out going from current vertex and update distances
		EdgeNodePtr current = self->edges[current_vertex].head;
		while (current != NULL) {
			float distance;	//array of distances to vertices
			int vertex = current->edge.to_vertex; //vertex at the end of the edge from current_vertex
			distance = distances[current_vertex] + current->edge.weight; //calculate distance to vertex
			
			//update distances if a shorter distance is found for a given vertex
			if (distance < distances[vertex]) {
				distances[vertex] = distance;
			}
			current = current->next; 
		}
		
	}
	//free dynamically allocated memory
	free(visited);
	return distances;
	/*******************************************************/
}

/*
* Find a MST from an inputted graph, connect the graph by the cheapest connection
* Param graph *, graph to get the new graph from
* Param int start, which vertex to start from
*/
Graph prims_mst(Graph *self, int start) {
	/*******************************************************/
	Graph g; //new graph
	int current_vertex;//vertex with the nearest distance
	float  *cost = malloc(sizeof(float) * self->V); //array of cheapest costs
	int *from = malloc(sizeof(int) * self->V); //array of vertices that give the cheapest cost to a given vertex, value stored is the vertex that gave the current cost to given vertex (given vertex is vertex used as index for the array)
	int *visited = malloc(sizeof(int) * self->V); //array of booleans indicating whether a given vertex has been visited

	//initialise the variables
	g.V = self->V;
	g.edges = malloc(sizeof(g.edges) * g.V);
	g.vertex_types = malloc(sizeof(int) * g.V);

	for (int i = 0; i < self->V; i++) {
		g.vertex_types[i] = self->vertex_types[i];
		from[i] = g.V; //special flag value is the number of vertices, as its not in the index range of the array
		g.edges[i].head = NULL;
		cost[i] = 1000000; 
		visited[i] = 0;
	}

	//starting vertex
	cost[start] = 0;

	//add vertices until all have been added
	while (!check_visited(visited, self->V)) {
		current_vertex = find_nearest(cost, visited, self->V); //find the cheapest vertex to connect
		visited[current_vertex] = 1; //set that vertex as visited
		
		//if this vertex's cost was given from another vertex, add a edge between the two vertices
		if (from[current_vertex] < g.V) {
			//add edge to the vertex that the newly visited vertix's cost came from to the bnewly visited vertex
			EdgeNodePtr new_node = malloc(sizeof * new_node);
			new_node->next = g.edges[from[current_vertex]].head;
			new_node->edge.to_vertex = current_vertex;
			new_node->edge.weight = cost[current_vertex];
			g.edges[from[current_vertex]].head = new_node;

			//add an edge to the newly visited vertex to where the newly visited vertix's cost came from
			EdgeNodePtr new_node_from = malloc(sizeof * new_node_from);
			new_node_from->next = g.edges[current_vertex].head;
			new_node_from->edge.to_vertex = from[current_vertex];
			new_node_from->edge.weight = cost[current_vertex];
			g.edges[current_vertex].head = new_node_from;
		}
		
		//update cost and update from which vertex these costs come from of all vertices tht havent been visited yet
		EdgeNodePtr current = self->edges[current_vertex].head;
		while (current != NULL) {
			//if cost to a vertex that hasnt been visited is less thatn the current cost of that vertex, update cost to that vertex and from which vertex gave that cost
			if (visited[current->edge.to_vertex] == 0 && cost[current->edge.to_vertex] > current->edge.weight) {
				cost[current->edge.to_vertex] = current->edge.weight;
				from[current->edge.to_vertex] = current_vertex;
			}
			current = current->next;
		}
	}

	//free dynamically allocated memory
	free(from);
	free(cost);
	free(visited);
	return g;
	/*******************************************************/
}

/*
* Find a MST from an inputted graph, connect the graph by the cheapest connection but each vertex is not connected to another vertex of the same type (as much as possible)
* Param graph *, graph to get the new graph from
*/
Graph my_solution(Graph* self) {
	/*******************************************************/
	Graph g; //new graph
	int current_vertex; //vertex with the nearest distance (in this case, smallest cost)
	float* cost = malloc(sizeof(float) * self->V); //array of cheapest costs, to given vertex
	int* from = malloc(sizeof(int) * self->V); //array of vertices that give the cheapest cost to a given vertex, value stored is the vertex that gave the current cost to given vertex (given vertex is vertex used as index for the array)
	int* visited = malloc(sizeof(int) * self->V);  //array of booleans indicating whether a given vertex has been visited
	int* next_to_type = malloc(sizeof(int) * self->V); //array of vertices that was given the cheapest cost from a vertex of the same type, 1 representing true, 0 -> false

	//initialise the variables
	g.V = self->V;
	g.edges = malloc(sizeof(g.edges) * g.V);
	g.vertex_types = malloc(sizeof(int) * g.V);

	//the following for loops can be added together
	for (int i = 0; i < self->V; i++) {
		g.vertex_types[i] = self->vertex_types[i];
		from[i] = g.V; //special flag value is the number of vertices, as its not in the index range of the array
		g.edges[i].head = NULL;
		cost[i] = 1000000; //flt_max
		visited[i] = 0;
		next_to_type[i] = 0;
	}

	//start at vertex 0
	cost[0] = 0;

	//add vertices until all have been added
	while (!check_visited(visited, self->V)) {
		current_vertex = find_nearest(cost, visited, self->V); //find the cheapest vertex to connect
		visited[current_vertex] = 1; //set that vertex as visited

		//if this vertex's cost was given from another vertex, add a edge between the two vertices, if both verteices arent the same type
		if (from[current_vertex] < g.V && g.vertex_types[current_vertex] != g.vertex_types[from[current_vertex]]) {
			
			//add edge to the vertex that the newly visited vertix's cost came from
			EdgeNodePtr new_node = malloc(sizeof * new_node);
			new_node->next = g.edges[from[current_vertex]].head;
			new_node->edge.to_vertex = current_vertex;
			new_node->edge.weight = cost[current_vertex];
			g.edges[from[current_vertex]].head = new_node;

			//add an edge to the newly visited vertex to where the newly visited vertix's cost came from
			EdgeNodePtr new_node_from = malloc(sizeof * new_node_from);
			new_node_from->next = g.edges[current_vertex].head;
			new_node_from->edge.to_vertex = from[current_vertex];
			new_node_from->edge.weight = cost[current_vertex];
			g.edges[current_vertex].head = new_node_from;
		}
		//if this vertex's cost was given from another vertex and they are the same type, mark next to type of that vertex as true
		else if (from[current_vertex] < g.V && g.vertex_types[current_vertex] == g.vertex_types[from[current_vertex]]) {
			next_to_type[current_vertex] = 1;
		}

		//if this vertex isnt conected to the same type vertex, the update cost and update from which vertex these costs come from of all vertices that havent been visited yet from this current vertex
		if (next_to_type[current_vertex] == 0) {
			EdgeNodePtr current = self->edges[current_vertex].head;
			while (current != NULL) {
				//if cost to a vertex that hasnt been visited is less thatn the current cost of that vertex, update cost to that vertex and from which vertex gave that cost
				if (visited[current->edge.to_vertex] == 0 && cost[current->edge.to_vertex] > current->edge.weight) {
					cost[current->edge.to_vertex] = current->edge.weight;
					from[current->edge.to_vertex] = current_vertex;
				}
				current = current->next;
			}
		}
	}

	//loops through next_to_type array twice, to make sure that graph is connected. 
	//this is done to avoid a vertex to be connected to a vertex that wasnt already on the main graph. 
	//what was heppening was on a small chance, a unconnected graph was being produced, as two vertices that were on the next_to_type array were being connected, without being connectd to main graph. 
	//so on first iteration, it makes sure aleast on of those is first connected to amin graph, then add the other vertex 
	for (int m = 0; m < 2; m++) {
		for (int i = 0; i < g.V; i++) {
			//if a vertex was about to be connected to the same type vertex
			if (next_to_type[i] == 1)
			{
				int found = 0; //variable that says a appropriate vertex was found
				int vertex; //vertex to connect the next_to_type vertex to.
				float cost = 1000000; //initial cost of new connection

				EdgeNodePtr current = self->edges[i].head;
				while (current != NULL) {
					//if the cost to a vertex, of a different type and that is already on the graph (was addedd to a vertex of different type), set found to true, and update the vertex to connect to and the cost that was found
					if (cost > current->edge.weight && g.vertex_types[i] != g.vertex_types[current->edge.to_vertex] && next_to_type[current->edge.to_vertex] == 0) {
						cost = current->edge.weight;
						vertex = current->edge.to_vertex;
						found = 1;
					}
					current = current->next;
				}

				//if a appropriate vertex was found, add the connection 
				if (found) {
					//add edge to the vertex that isnt connect to the graph yet
					EdgeNodePtr new_node = malloc(sizeof * new_node);
					new_node->edge.to_vertex = vertex;
					new_node->edge.weight = cost;
					new_node->next = g.edges[i].head;
					g.edges[i].head = new_node;

					//add an edge to the vertex that hasnt been added from 'vertex'
					EdgeNodePtr new_node_from = malloc(sizeof * new_node_from);
					new_node_from->edge.to_vertex = i;
					new_node_from->edge.weight = cost;
					new_node_from->next = g.edges[vertex].head;
					g.edges[vertex].head = new_node_from;

					//the vertex isnt connected to the same type vertex anymore
					next_to_type[i] = 0;
				}
			}
		}
	}

	//in the unlikely case that not all vertices that were going to be connected to the same type vertex (there is still a a vertex in the next_to_type array set to true), hasnt been added to the graph yet. add to the vertex that provides the cheapest cost on the graph
	if (!check_visited(next_to_type, g.V)) {
		for (int i = 0; i < g.V; i++) {
			if (next_to_type[i] == 1)
			{
				EdgeNodePtr current = self->edges[i].head;

				int vertex; //vertex to connect to
				float cost = 1000000; //initial cost of that connection

				//find the cheapest connection and which vertex gives that connection
				while (current != NULL) {
					if (cost > current->edge.weight && next_to_type[current->edge.to_vertex] == 0) {
						cost = current->edge.weight;
						vertex = current->edge.to_vertex;
					}
					current = current->next;
				}

				//add a connection between the two vertices
				EdgeNodePtr new_node = malloc(sizeof * new_node);
				new_node->edge.to_vertex = vertex;
				new_node->edge.weight = cost;
				new_node->next = g.edges[i].head;
				g.edges[i].head = new_node;

				EdgeNodePtr new_node_from = malloc(sizeof * new_node_from);
				new_node_from->edge.to_vertex = i;
				new_node_from->edge.weight = cost;
				new_node_from->next = g.edges[vertex].head;
				g.edges[vertex].head = new_node_from;

				next_to_type[i] = 0;
			}
		}
	}

	//free dynamically allocated memory
	free(next_to_type);
	free(from);
	free(cost);
	free(visited);
	return g;
	/*******************************************************/
}
