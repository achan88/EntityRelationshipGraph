#include "Graph.h"

~Graph::Graph() {
    for (int i = 0; i < vertices)
}
bool Graph::isValidId(string id);



Node* Graph::searchForNode(string id);

void Graph::addRelationship(string sourceId, string label, string destinationId, double weight);
void Graph::addEntity(string id, string name, string type);
void Graph::printAdjacent(string id);
void Graph::deleteEntity(string id);
        