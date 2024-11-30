#include "Node.h"

Node::Node(string label, string id, string name) {
    this->label = label;
    this->id = id;
    this->name = name;
}
        
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
            
    for (int i = 0; i < adjacentNodes.size(); i++) {
        if (get<0>(adjacentNodes[i]) == node) {
            get<1>(adjacentNodes[i]) = label;
            get<2>(adjacentNodes[i]) = weight;
            return;
        }
    }

    tuple<Node*, string, double> myTuple = make_tuple(node, label, weight);
    adjacentNodes.push_back(myTuple);
}

void Node::removeAdjacentNode(Node* node) {

    for (int i = 0; i < adjacentNodes.size(); i++) {
        if (get<0>(adjacentNodes[i]) == node) {
            adjacentNodes.erase(adjacentNodes.begin() + i);
            return;
        }
    }

}

bool Node::isAdjacentNode(Node* node) {
    for (int i = 0; i < adjacentNodes.size(); i++) {
        if (get<0>(adjacentNodes[i]) == node) {
            return true;
        }
    }
    return false;
}