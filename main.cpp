#include "graph.hpp"
#include "randomgraph.hpp"
#include <iostream>
#include <time.h>
#include <cstring>

// Convenience. If you don't understand just ignore ;)
using namespace DHBW;

// What do I do?
// The '&' in  front of g ensures that we do not copy g,
// it will be exactly the same Graph g as outside of this function
VertexId example_function1(Graph &g)
{
	// This method is written unneccessary compliceted to give a better feeling of the use of this class
	// EdgeId and GraphId are internally just the same as unsigned int's. I prefer using the aliases for
	// better readability.
	EdgeId a;
	for (VertexId i = 0; i < g.num_vertices(); ++i)
	{
		// Get the i-th vertex
		Vertex &v = g.vertex(i);
		EdgeId b = 0;
		for (EdgeId e = 0; e < v.num_edges(); ++e)
		{
			++b;
		}
		if (b > a)
		{
			a = b;
		}
	}
	return a;
}

// A vector is a kind of an dynamic_array. std::vector<VertexId> is a vector of VertexIds
// Random path starting at s and ending in t
std::vector<VertexId> random_path(Graph &g, VertexId s, VertexId t)
{
	// Create empty vector
	std::vector<VertexId> path;
	// add first vertex, i.e. s, (represented by its id)
	path.push_back(s);

	// random walk :D
	VertexId current = s;
	while (current != t)
	{
		// Get vertex from VertexId
		Vertex &v = g.vertex(current);
		// Ask for number of edges
		EdgeId num_edges = v.num_edges();
		// compute random integer in [0,num_edges-1]
		EdgeId rand_id = rand() % num_edges;
		// Take edge (first take id from v, than get edge from g)
		Edge next_edge = g.edge(v.edge(rand_id));
		// Compute next vertex
		current = next_edge.other_vertex(v.id());
		path.push_back(current);
	}
	return path;
}

// used for findConnectedComponents()
// works in general like visit1()
void visit(Graph &g, std::vector<bool> &visited, std::vector<VertexId> &connComponent, VertexId v_id)
{
	Vertex &currVertex = g.vertex(v_id);
	visited[v_id] = 1;
	for (EdgeId j = 0; j < currVertex.num_edges(); ++j)
	{
		Edge e = g.edge(currVertex.edge(j));
		// if edge is outgoing continue
		VertexId neighbour_id = e.other_vertex(currVertex.id());
		if (0 == visited[neighbour_id])
		{
			visit(g, visited, connComponent, neighbour_id);
		}
		else
		{
			continue;
		}
	}
	// add vertex to the connected component
	connComponent.push_back(v_id);
}

// recursive way for finding connected components
void findConnectedComponents(Graph &g, bool printToStdOut = false)
{
	std::vector<bool> visited(g.num_vertices(), 0);
	// to store connected components
	std::vector<std::vector<VertexId>> connectedComps;

	for (VertexId i = 0; i < g.num_vertices(); ++i)
	{
		if (1 != visited[i])
		{
			connectedComps.push_back(std::vector<EdgeId>());
			visit(g, visited, connectedComps.back(), i);
		}
		else
		{
			continue;
		}
	}

	std::cout << connectedComps.size();
	if (printToStdOut != false)
	{
		for (VertexId row = 0; row <= connectedComps.size(); row++)
		{
			std::cout << "\n";
			for (VertexId col = 0; col < connectedComps[row].size(); col++)
			{
				std::cout << connectedComps[row][col] << " ";
			}
		}
	}
}

// vertices are visited in correct edge direction and numbered
void visit1(Graph &g, std::vector<bool> &visited, VertexId &vertexCounter, std::vector<VertexId> &numberedVertices, VertexId v_id)
{
	Vertex &currVertex = g.vertex(v_id);
	visited[v_id] = 1;
	for (EdgeId j = 0; j < currVertex.num_edges(); ++j)
	{
		Edge e = g.edge(currVertex.edge(j));
		// if edge is incoming continue
		if (currVertex.id() == e.vertex2())
		{
			continue;
		}
		else
		{
			VertexId neighbour_id = e.other_vertex(currVertex.id());
			if (0 == visited[neighbour_id])
			{
				visit1(g, visited, vertexCounter, numberedVertices, neighbour_id);
			}
			else
			{
				continue;
			}
		}
	}
	numberedVertices[vertexCounter] = v_id;
	++vertexCounter;
}

// vertices are visited in incorrect edge direction and added to the component
void visit2(Graph &g, std::vector<bool> &visited, std::vector<VertexId> &connComponent, VertexId v_id)
{
	Vertex &currVertex = g.vertex(v_id);
	visited[v_id] = 1;
	for (EdgeId j = 0; j < currVertex.num_edges(); ++j)
	{
		Edge e = g.edge(currVertex.edge(j));
		// if edge is outgoing continue
		if (currVertex.id() == e.vertex1())
		{
			continue;
		}
		else
		{
			VertexId neighbour_id = e.other_vertex(currVertex.id());
			if (0 == visited[neighbour_id])
			{
				visit2(g, visited, connComponent, neighbour_id);
			}
			else
			{
				continue;
			}
		}
	}
	// add vertex to the connected component
	connComponent.push_back(v_id);
}

void tarjans(Graph &g, bool printToStdOut = false)
{
	std::vector<bool> visited(g.num_vertices(), 0);
	// counter to number vertices in first run
	VertexId vertexCounter = 0;
	// to store vertexIds ordered after their number from the first run
	std::vector<VertexId> numberedVertices(g.num_vertices(), -1);
	// to store strongly connected components in the second run
	std::vector<std::vector<VertexId>> connectedComps;

	for (VertexId i = 0; i < g.num_vertices(); ++i)
	{
		if (1 != visited[i])
		{
			visit1(g, visited, vertexCounter, numberedVertices, i);
		}
		else
		{
			continue;
		}
	}

	// second run

	//set back visited vertices
	std::fill(visited.begin(), visited.end(), false);
	for (VertexId j = visited.size() - 1; j >= 0 && j < visited.size(); --j)
	{
		VertexId v_id = numberedVertices[j];
		if (1 != visited[v_id])
		{
			connectedComps.push_back(std::vector<EdgeId>());
			visit2(g, visited, connectedComps.back(), v_id);
		}
		else
		{
			continue;
		}
	}

	std::cout << connectedComps.size();
	if (printToStdOut != false)
	{
		for (VertexId row = 0; row <= connectedComps.size(); row++)
		{
			std::cout << "\n";
			for (VertexId col = 0; col < connectedComps[row].size(); col++)
			{
				std::cout << connectedComps[row][col] << " ";
			}
		}
	}
}

// first argument (required): input file (contains graph)
// second argument (optional): 'true', if resulting components should be printed to stdout
// third argument (optional): 'false', if input graph is undirected (or only connected components are searched)
int main(int argc, char *argv[])
{
	bool print = false;
	bool digraph = true;
	// check if input file is given as argument
	if (argc < 2 || argv[1] == nullptr)
	{
		std::cerr << "No input file given!";
	}
	// check if result should be printed
	if (argc > 2 && strcmp(argv[2], "true") == 0)
	{
		print = true;
	}
	// check if edges should be seen as undirected (find connected components)
	if (argc > 3 && strcmp(argv[3], "false") == 0)
	{
		digraph = false;
	}

	// for measure of duration
	clock_t start, end;
	start = clock();
	// Read graph from input file
	Graph g(argv[1]);
	if (digraph == true)
	{
		//find strongly connected components
		tarjans(g, print);
	}
	else
	{
		findConnectedComponents(g, print);
	}

	end = clock();
	std::cout << "\nTime: " << end - start;

	return 0;
}