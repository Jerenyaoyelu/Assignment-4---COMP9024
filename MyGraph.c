#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// A vertex is a 2D point
typedef struct Vertex { 
	int x; // x-coordinate
	int y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex *p1; // first end point
	Vertex *p2; // second end point
} Edge;
// A vertex node stores a vertex and other information, and you need to expand this type
typedef struct VertexNode {
	Vertex *v;
	struct VertexNode *next;
	// for disdinguish if it is a isolated vertex
	int degree;
} VertexNode;
typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
	VertexNode **vertices; // an array of vertices or a linked list of vertices  
	int nV; // #vertices
	int nE; // #edges
} GraphRep;


//The above types serve as a starting point only. You need to expand them and add more types. 
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment    

VertexNode *NewVertexNode(Vertex *V){
	VertexNode *newNode = (VertexNode *)malloc(sizeof(VertexNode));
	assert(newNode!=NULL);
	newNode->degree = 0;
	newNode->next = NULL;
	newNode->v = (Vertex *)malloc(sizeof(Vertex));
	assert(newNode->v !=NULL);
	newNode->v = V;
	return newNode;
}

// Add the time complexity analysis of CreateEmptyGraph() here
//assume the miaximum size of array is 100
Graph CreateEmptyGraph()
{
	Graph newg = (GraphRep *)malloc(sizeof(GraphRep));
	assert(newg != NULL);
	newg->vertices =(VertexNode **)malloc(100*sizeof(VertexNode *));
	assert(newg->vertices!=NULL);
	newg->nV = 0;
	newg->nE = 0;
	// for(int i = 0; i< 100;i++){
	// 	printf("(tt)\n");
	// 	newg->vertices[i] = NULL;
	// }
	return newg;
}
// Add the time complexity analysis of InsertEdge() here
int InsertEdge(Graph g, Edge *e)
{
	// //create new graph nodes
	// VertexNode *new1 = NewVertexNode(e->p1);
	// VertexNode *new2 = NewVertexNode(e->p2);
	// //update the degree
	// new1->degree++;
	// new2->degree++;
	//it is an empty graph
	if(g->nE == 0){
		//insert the first vertexNode
		g->vertices[0] = NewVertexNode(e->p1);
		g->vertices[0]->degree++;
		//if use the new2 directly, it eventually will create a circle after this if statement
		g->vertices[0]->next = NewVertexNode(e->p2);
		g->vertices[0]->next->degree++;
		g->vertices[1] = NewVertexNode(e->p2);
		g->vertices[1]->degree++;
		//if use the new1 directly, it eventually will create a circle after this if statement
		g->vertices[1]->next = NewVertexNode(e->p1);
		g->vertices[1]->next->degree++;
		g->nE ++;
		g->nV = g->nV + 2;
		// printf("ver %d: eg %d\n",g->nV,g->nE);
		return 1;
	}
	int j = 0;
	VertexNode *crt;
	VertexNode *tail;
	//label to show that if links are added in both vertice side and also indicates the three cases below.
	//{1,1} means both vertices are existing ones but the edge is new, and it has been added during the while loop, return 1
	//{1,0}/{0,1} means only one vertex is new and the edge is new, the new vertex and new edge(the link on the new vertex side) have to be added after the while loop, return 1 
	//{0,0}:
		//if the function does not end up within the while loop, this means both are new vertices, the vertices and edge are needed to be added after the loop,return 1
		//if the function ends up within the while loop, this means both are existing vertices and so is the edge,return 0
	int isExisting[2] = {0,0};
	//both vertices are existing, if this function ends in this loop
	while(g->vertices[j] != NULL){
		// printf("(%d,%d)-(%d,%d)\n",g->vertices[j]->v->x,g->vertices[j]->v->y,g->vertices[j]->next->v->x,g->vertices[j]->next->v->y);
		//add link on p1 side
		if(g->vertices[j]->v->x == e->p1->x && g->vertices[j]->v->y == e->p1->y){
			crt = g->vertices[j];
			//to see if it exists or not
			while(crt != NULL){
				//the edge is existing in the graph
				if(crt->v->x == e->p2->x && crt->v->y == e->p2->y){return 0;}
				//assign a pointer to the tail of the adjacent list
				if(crt->next == NULL){
					tail = crt;
				}
				crt = crt->next;
			}
			//it is a new edge
			//how to deal with the degree of this newnode??
			tail->next = NewVertexNode(e->p2);
			isExisting[0] = 1;
		}
		//add link on p2 side
		if(g->vertices[j]->v->x == e->p2->x && g->vertices[j]->v->y == e->p2->y){
			crt = g->vertices[j];
			while(crt != NULL){
				//the edge is existing in the graph
				if(crt->v->x == e->p1->x && crt->v->y == e->p1->y){return 0;}
				//assign a pointer to the tail of the adjacent list
				if(crt->next == NULL){
					tail = crt;
				}
				crt = crt->next;
			}
			//it is a new edge
			tail->next = NewVertexNode(e->p1);
			isExisting[1] = 1;
			// printf("mm(%d,%d)-(%d,%d)\n",g->vertices[j]->v->x,g->vertices[j]->v->y,tail->next->v->x,tail->next->v->y);
		}
		//when the links are added in both side, there is no need to continue to go through the loop.
		if(isExisting[0] == 1 && isExisting[1] == 1){
			g->nE++;
			// printf("no ver %d: eg %d\n",g->nV,g->nE);
			return 1;
		}
		j++;
	}
	//one vertices is existing, and another one is new which needed to be added into the array
	if(isExisting[0] == 1 && isExisting[1] == 0){
		g->vertices[g->nV] = NewVertexNode(e->p2);
		g->vertices[g->nV]->degree++;
		g->vertices[g->nV]->next = NewVertexNode(e->p1);
		g->nV++;
	}else if(isExisting[0] == 0 && isExisting[1] == 1){
		g->vertices[g->nV] = NewVertexNode(e->p1);
		g->vertices[g->nV]->degree++;
		g->vertices[g->nV]->next = NewVertexNode(e->p2);
		g->nV++;
	}else {//both are new vertices
		g->vertices[g->nV] = NewVertexNode(e->p1);
		g->vertices[g->nV]->degree++;
		g->vertices[g->nV]->next = NewVertexNode(e->p2);
		g->vertices[g->nV+1] = NewVertexNode(e->p2);
		g->vertices[g->nV+1]->degree++;
		g->vertices[g->nV+1]->next = NewVertexNode(e->p1);
		g->nV = g->nV + 2;
	}
	g->nE++;
	// printf("ver %d: eg %d\n",g->nV,g->nE);
	return 1;
}

// Add the time complexity analysis of DeleteEdge() here
void DeleteEdge(Graph g, Edge *e)
{
	
}

// Add the time complexity analysis of ReachableVertices() here
void ReachableVertices(Graph g, Vertex *v)
{
	
}

// Add the time complexity analysis of ShortestPath() here
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
	
}

// Add the time complexity analysis of FreeGraph() here
void FreeGraph(Graph g)
{
	
}

// Add the time complexity analysis of ShowGraph() here
void ShowGraph(Graph g)
{
	int j = 0;
	while(g->vertices[j]!= NULL){
		printf("(%d,%d)",g->vertices[j]->v->x,g->vertices[j]->v->y);
		j++;
	}
	printf("\n");
}

int main() //sample main for testing 
{ 
 Graph g1;
 Edge *e_ptr; 
 Vertex *v1, *v2;
  
 // Create an empty graph g1;
 g1=CreateEmptyGraph();
  
 // Create first connected component 
 // Insert edge (0,0)-(0,10)
 e_ptr =(Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=0;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (0,0)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (0, 10)-(10, 10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=10;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (0,10)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,0)-(5,4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5, 4)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=4;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5,6)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=6;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10,10)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=10;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10, 6)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=6;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Create second connected component
 // Insert edge (20,4)-(20,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=4;
 v2->x=20;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (20,10)-(30,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=10;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (25,5)-(30,10) 	
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=25;
 v1->y=5;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n"); 
 ShowGraph(g1);
 
//  //Display graph g1
//  ShowGraph(g1);
	
//  // Find the shortest path between (0,0) and (10,6) 
//  v1=(Vertex*) malloc(sizeof(Vertex));
//  assert(v1 != NULL);
//  v2=(Vertex *) malloc(sizeof(Vertex));
//  assert(v2 != NULL);
//  v1->x=0;
//  v1->y=0;
//  v2->x=10;
//  v2->y=6;
//  ShortestPath(g1, v1, v2);
//  free(v1);
//  free(v2);
	  
//  // Delete edge (0,0)-(5, 6)
//  e_ptr = (Edge*) malloc(sizeof(Edge));
//  assert(e_ptr != NULL);
//  v1=(Vertex*) malloc(sizeof(Vertex));
//  assert(v1 != NULL);
//  v2=(Vertex *) malloc(sizeof(Vertex));
//  assert(v2 != NULL);
//  v1->x=0;
//  v1->y=0;
//  v2->x=5;
//  v2->y=6;
//  e_ptr->p1=v1;
//  e_ptr->p2=v2; 	 
//  DeleteEdge(g1, e_ptr);
//  free(e_ptr);
//  free(v1);
//  free(v2);
 	 
//  // Display graph g1
//  ShowGraph(g1);
	
//  // Find the shortest path between (0,0) and (10,6) 
//  v1=(Vertex*) malloc(sizeof(Vertex));
//  assert(v1 != NULL);
//  v2=(Vertex *) malloc(sizeof(Vertex));
//  assert(v2 != NULL);
//  v1->x=0;
//  v1->y=0;
//  v2->x=10;
//  v2->y=6; 
//  ShortestPath(g1, v1, v2);
//  free(v1);
//  free(v2);
 
//  // Find the shortest path between (0,0) and (25,5)
//  v1=(Vertex*) malloc(sizeof(Vertex));
//  assert(v1 != NULL);
//  v2=(Vertex *) malloc(sizeof(Vertex));
//  assert(v2 != NULL);
//  v1->x=0;
//  v1->y=0;
//  v2->x=25;
//  v2->y=5;
//  ShortestPath(g1, v1, v2);
//  free(v1);
//  free(v2);	
 
//  // Find reachable vertices of (0,0)
//  v1=(Vertex*) malloc(sizeof(Vertex));
//  assert(v1 != NULL);
//  v1->x=0;
//  v1->y=0;
//  ReachableVertices(g1, v1);
//  free(v1);
 
//  // Find reachable vertices of (20,4)
//  v1=(Vertex*) malloc(sizeof(Vertex));
//  assert(v1 != NULL);
//  v1->x=20;
//  v1->y=4;
//  ReachableVertices(g1, v1);
//  free(v1);
 
//  // Free graph g1
//  FreeGraph(g1);
 
 return 0; 
}
