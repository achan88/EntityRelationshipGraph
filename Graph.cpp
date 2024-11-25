#include "Graph.h"

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

void Graph::addRelationship(string sourceId, string label, string destinationId, double weight);
void Graph::addEntity(string id, string name, string type);
void Graph::printAdjacent(string id);
void Graph::deleteEntity(string id);
        