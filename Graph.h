#include "Node.h"
#include <vector>
#include <string>

class Graph {

    private:
        vector<Node*> entities;

        bool isValidId(string id);
        Node* searchForNode(string id);
    
    public:
        Graph() = default;
        ~Graph();

        void addRelationship(string sourceId, string label, string destinationId, double weight);
        void addEntity(string id, string name, string type);
        void printAdjacent(string id);
        void deleteEntity(string id);
        
};