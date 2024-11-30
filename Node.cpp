#include "Node.h"
#include <tuple>
#include <vector>

Node::Node(string label, string id, string name) {
    this->label = label;
    this->id = id;
    this->name = name;
}

// Trivial accessor and mutator methods
string Node::getLabel() {

    return label;
}
void Node::setLabel(string newLabel) {
    label = newLabel;
}

string Node::getId() {
    return id;

}

string Node::getName() {
    return name;
}

void Node::setName(string newName) {
    name = newName;
}

vector<tuple<Node*, string, double>>& Node::getAdjacentNodes() {
    return adjacentNodes;
}

void Node::addAdjacentNode(Node* node, string label, double weight) {
            
    // If this node already forms and edge, update its label and weight
    for (int i = 0; i < adjacentNodes.size(); i++) {
        if (get<0>(adjacentNodes[i]) == node) {
            get<1>(adjacentNodes[i]) = label;
            get<2>(adjacentNodes[i]) = weight;
            return;
        }
    }

    // Otherwise, make a new tuple, and add it to adjacency list
    tuple<Node*, string, double> newTuple = make_tuple(node, label, weight);
    adjacentNodes.push_back(newTuple);
}

void Node::removeAdjacentNode(Node* node) {

    // Check to see if it exists, if it does delete, if it doesn't, do nothing
    for (int i = 0; i < adjacentNodes.size(); i++) {
        if (get<0>(adjacentNodes[i]) == node) {
            adjacentNodes.erase(adjacentNodes.begin() + i);
            return;
        }
    }

}