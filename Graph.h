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

        void load(string filename, string type);
        void addRelationship(string sourceId, string label, string destinationId, double weight, bool print);
        void addEntity(string id, string name, string type, bool print);
        void printAdjacent(string id);
        void deleteEntity(string id);
        void path(string startId, string endId);
        void highest();
        void findAll(string type, string string);

        
};