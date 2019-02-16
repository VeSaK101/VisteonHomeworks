//============================================================================
// Name        : AStar_fixme.cpp
// Author      : ivaylo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fcntl.h>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <cmath>

class Node {
public:
	Node(Node* parent, int row, int col, int g, int h) 
	: parent(parent), row(row), col(col), g(g), h(h), f(g + h) {		
	}
	
	Node* const parent;
	const int row;
	const int col;
	const int g;
	const int h;
	const int f; 
};


std::list<Node*> findShortestPath(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<int>> map, int w, int h) {
	auto comp = [](Node* n1, Node* n2) {return n1->f > n2->f;};
	std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> openNodes(comp);
	std::set<Node*> closedNodes;
	
	// Push the start node into the queue; heuristics is Manhattan distance to the end 
	openNodes.push(new Node(nullptr, fromX, fromY, 0, std::abs(fromX - toX) + std::abs(fromY - toY)));
	
	while(!openNodes.empty()) {
		// Get the node with least (f = cost + h) and remove it from the queue
		Node* currentNode = openNodes.top();
		openNodes.pop();
		
		closedNodes.insert(currentNode);
		
		// Get the neighbors of currentNode
		std::vector<Node*> neighbors;
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (currentNode->row + i >= 0 && currentNode->row + i < w && currentNode->col + j >= 0 && currentNode->col + j < h && ((i == 0 && j != 0) || (i != 0 && j == 0)) && map[currentNode->row + i][currentNode->col + j] == 0) {
					neighbors.push_back(new Node(currentNode, currentNode->row + i, currentNode->col + j, currentNode->g + 1, std::abs(currentNode->row + i - toX) + std::abs(currentNode->col + j - toY)));
				}
			}
		}
		
		// For each neighbor:
		// If it is the end node, return the path
		// If it is not in closed set, add it to open queue
		for (size_t i = 0; i < neighbors.size(); ++i) {
			if (neighbors[i]->row == toX && neighbors[i]->col == toY) {
				// Reconstruct the path because neighbor[i] is the last node
				Node* node = neighbors[i];
				std::list<Node*> route;
				while (node != nullptr) {
					route.push_front(node);
					node = node->parent;
				}
				return route;
			}
			
			if (closedNodes.find(neighbors[i]) != closedNodes.end()) {
				continue;
			}
			
			openNodes.push(neighbors[i]);		
		}
	}
	
	return std::list<Node*>();	// return empty list
}

int main() {
	std::list<Node*> route = findShortestPath(0, 0, 6, 6, {
			{0, 1, 1, 0, 0, 0, 0},
			{0, 1, 1, 0, 1, 1, 0},
			{0, 0, 0, 0, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 1, 1, 0},
			{0, 1, 1, 1, 0, 0, 0},
			{0, 0, 0, 0, 0, 1, 0},
	}, 7, 7);
	
	// Print the path
	if (route.size() == 0) {
		std::cout << "No route to target!" << std::endl;
	} else {
		for (std::list<Node*>::iterator nodeIt = route.begin(); nodeIt != route.end(); ++nodeIt) {
			std::cout << "(" << (*nodeIt)->row << ", " << (*nodeIt)->col << ")->";
		}
	}
	return 0;
}