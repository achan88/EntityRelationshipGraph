#include "Graph.h"
#include <iostream>
using namespace std;

Graph::~Graph() {
    
}
bool Graph::isValidId(string id) {
    for (int i = 0; i < id.length(); i++) {
        if (!((id[i] >= 'A' && id[i] <= 'Z') || (id[i] >= 'a' && id[i] <= 'z') || (id[i] >= '0' && id[i] <= '9'))) {
            return false; 
        }
    }
    return true;
}

Node* Graph::searchForNode(string id) {

    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            return entities[i];
        }
    }
    return nullptr;
}

void Graph::addRelationship(string sourceId, string label, string destinationId, double weight) {
    Node* sourceNode = searchForNode(sourceId);
    Node* destinationNode = searchForNode(destinationId);

    if (sourceNode == nullptr || destinationNode == nullptr) {
        cout << "failure" << endl;
        return;
    }

    sourceNode->addAdjacentNode(destinationNode, label, weight);
    destinationNode->addAdjacentNode(sourceNode, label, weight);
    cout << "success" << endl;
}

void Graph::addEntity(string id, string name, string type) {
    Node* node = searchForNode(id);

    if (node) {
        node->setName(name);
        node->setLabel(type);
        cout << "success" << endl;
        return;
    }

    node = new Node(type, id, name);
    entities.push_back(node);
    cout << "success" << endl;


}
void Graph::printAdjacent(string id) {
    Node* node = searchForNode(id);

    if (node == nullptr) {
        cout << "failure" << endl;
        return;
    }

    if (node->getAdjacentNodes().size() == 0) {
        cout << endl;
        return;
    }

    for (int i = 0; i < node->getAdjacentNodes().size(); i++) {
        cout << get<0>(node->getAdjacentNodes()[i])->getId() << " ";
    }

    cout << endl;
}

void Graph::deleteEntity(string id) {

    Node* node = searchForNode(id); 

    if (node == nullptr) {
        cout << "failure" << endl;
        return;
    }

    for (int i = 0; i < entities.size(); i++) {

        if (entities[i] != node) {
            entities[i]->removeAdjacentNode(node);
        }
    }

    int itAdd = 0;
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i] == node) {
            itAdd = i;
            break;
        }
    }

    delete node;

    entities.erase(entities.begin() + itAdd);
}


        