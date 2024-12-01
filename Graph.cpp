#include "Graph.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "illegal_exception.h"
using namespace std;

// Normal destructor
Graph::~Graph() {
    for (int i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
}

// Checks if id is within these three character ranges, if it is not, it contains an invalid character, and we return false
bool Graph::isValidId(string id) {
    for (int i = 0; i < id.length(); i++) {
        if (!((id[i] >= 'A' && id[i] <= 'Z') || (id[i] >= 'a' && id[i] <= 'z') || (id[i] >= '0' && id[i] <= '9'))) {
            return false; 
        }
    }
    return true;
}

// Linearly traverse the vector of entities to find if a corresponding id is in the graph, if not, return nullptr for indication
Node* Graph::searchForNode(string id) {

    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            return entities[i];
        }
    }
    return nullptr;
}

void Graph::load(string filename, string type) {

    // set up file stream reader
    ifstream file(filename);
    string line;

    if (type == "entities") {

        // reading a single line from the file
        while (getline(file, line)) {
            istringstream stringstream(line);
            string id;
            string name;
            string newType;

            stringstream >> id >> name >> newType;

            // print flag set to false, load should only output success once 
            addEntity(id, name, newType, false);
        }



    } else if (type == "relationships") {
        while (getline(file, line)) {
            istringstream stringstream(line);
            string sourceId;
            string label;
            string destinationId;
            double weight;

            stringstream >> sourceId >> label >> destinationId >> weight;

            // print flag set to false, load should only output success once 
            addRelationship(sourceId, label, destinationId, weight, false);
        }
    }

    cout << "success" << endl;

    file.close();
}

void Graph::addRelationship(string sourceId, string label, string destinationId, double weight, bool print) {

    // Get corresponding id nodes in the graph
    Node* sourceNode = searchForNode(sourceId);
    Node* destinationNode = searchForNode(destinationId);
    
    // Ensure that these nodes actually exist, otherwise, failure
    if (sourceNode == nullptr || destinationNode == nullptr) {

        // Control output for load
        if (print) {
            cout << "failure" << endl;
        }
        return;
    }

    // Graph is undirected, so both source and destination and can be interchanged. Add to both nodes' adjacency lists
    sourceNode->addAdjacentNode(destinationNode, label, weight);
    destinationNode->addAdjacentNode(sourceNode, label, weight);

    // Control output for load
    if (print) {
        cout << "success" << endl;
    }
}

void Graph::addEntity(string id, string name, string type, bool print) {

    // Search for corresponding node with given id
    Node* node = searchForNode(id);

    // Node exists, then update its values
    if (node) {
        node->setName(name);
        node->setLabel(type);
        if (print) {
            cout << "success" << endl;
        }
        
        return;
    }

    // Otherwise, we create a new node, and add it to the list of vertices
    node = new Node(type, id, name);
    entities.push_back(node);

    if (print) {
        cout << "success" << endl;
    }

}

void Graph::printAdjacent(string id) {

    // Illegal argument checker
    try {
        if (!isValidId(id)) {
            throw illegal_exception();
        }
    } catch (const illegal_exception&) {
        cout << "illegal argument" << endl;
        return;
    }
    
    // Node searching 
    Node* node = searchForNode(id);

    // Invalid node, failure
    if (node == nullptr) {
        cout << "failure" << endl;
        return;
    }

    // No adjacent nodes
    if (node->getAdjacentNodes().size() == 0) {
        cout << endl;
        return;
    }

    // Print out all adjacent nodes id
    for (int i = 0; i < node->getAdjacentNodes().size(); i++) {
        cout << get<0>(node->getAdjacentNodes()[i])->getId() << " ";
    }

    cout << endl;
}


void Graph::deleteEntity(string id) {

    try {
        if (!isValidId(id)) {
            throw illegal_exception();
        }
    } catch (const illegal_exception&) {
        cout << "illegal argument" << endl;
        return;
    }

    Node* node = searchForNode(id); 

    // Node doesn't exist, cannot delete, failure
    if (node == nullptr) {
        cout << "failure" << endl;
        return;
    }
    
    // Remove the node from all other nodes' adjacency lists
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

    // Deallocate memory
    delete node;

    // Erase from entitiy vector, and shifts elements left to close the space
    entities.erase(entities.begin() + itAdd);

    cout << "success" << endl;
}

void Graph::path(string startId, string endId) {

    // Input validation
    try {
        if (!isValidId(startId) || !isValidId(endId)) {
            throw illegal_exception();
        }
    } catch (const illegal_exception&) {
        cout << "illegal argument" << endl;
        return;
    }
    
    // For the corresponding nodes
    Node* startNode = searchForNode(startId);
    Node* endNode = searchForNode(endId);

    // If either of them is nullptr, node doesn't exist in the graph
    if (startNode == nullptr || endNode == nullptr) {
        cout << "failure" << endl;
        return;
    }

    // Total path weight, current node, node path (path taken all way up to and including that particular node). 
    // Max heap to get highest weighted edge in efficient time O(1)
    vector<tuple<double, Node*, vector<Node*>>> maxHeap;

    // Starting node starts off our path
    tuple<double, Node*, vector<Node*>> startOfPath = make_tuple(0, startNode, vector<Node*>{startNode});
    maxHeap.push_back(startOfPath);

    // Keep track of nodes that have been visited already
    vector<Node*> processed;

    make_heap(maxHeap.begin(), maxHeap.end());

    while(!maxHeap.empty()) {

        tuple<double, Node*, vector<Node*>> currentTuple = maxHeap.front(); // This gets the top most element of the heap (neighbouring node with the highest weighted edge)
        pop_heap(maxHeap.begin(), maxHeap.end()); // pop_heap only pushes element to the back
        maxHeap.pop_back(); // Remove it from the actual vector

        // Extract required data fromthe current tuple (node, weight, path)
        Node* nodeToBeProcessed = get<1>(currentTuple);
        double nodeProcessingWeight = get<0>(currentTuple);
        vector<Node*> currentPath = get<2>(currentTuple);

        processed.push_back(nodeToBeProcessed);

        // Since there's only one unique path, if we see the end, immediately return
        if (nodeToBeProcessed == endNode) {
            for (int i = 0; i < currentPath.size(); i++) {
                cout << currentPath[i]->getId();

                if (i != currentPath.size() - 1) {
                    cout << " ";
                }
            }

            cout << " " << nodeProcessingWeight << endl;
            return;
        }

        // In order for a node to be marked as processed, we add all neighbouring nodes for exploration into binary max heap
        vector<tuple<Node*, string, double>> adjacentNodes = nodeToBeProcessed->getAdjacentNodes();
        for (int i = 0; i < adjacentNodes.size(); i++) {

            Node* node = get<0>(adjacentNodes[i]);  
            double adjacentWeight = get<2>(adjacentNodes[i]);

            // Check if this node has already been visited previously
            bool found = false;
            for (int j = 0; j < processed.size(); j++) {
                if (processed[j] == node) {
                    found = true;
                    break;
                }
            }

            // Node has already been visited, we don't want to re-add it to process again
            if (found) {
                continue;
            }
            
            // Form the new path for the adjacent nodes
            vector<Node*> newPath = currentPath;
            newPath.push_back(node);

            maxHeap.push_back(make_tuple(nodeProcessingWeight + adjacentWeight, node, newPath));

            // This makes heapifies the vector again to maintain max heap properties
            push_heap(maxHeap.begin(), maxHeap.end());

        }   
    }

    // No path is found, path fails (disconnected graph)
    cout << "failure" << endl;
}

// Same logic as path function, but with a slight modification, comments are the same as path function
tuple<double, Node*, Node*> Graph::pathHelper(Node* startNode, Node* endNode) {

    vector<tuple<double, Node*, vector<Node*>>> maxHeap;

    tuple<double, Node*, vector<Node*>> startOfPath = make_tuple(0, startNode, vector<Node*>{startNode});
    maxHeap.push_back(startOfPath);

    vector<Node*> processed;

    make_heap(maxHeap.begin(), maxHeap.end());

    while(!maxHeap.empty()) {

        tuple<double, Node*, vector<Node*>> currentTuple = maxHeap.front();
        pop_heap(maxHeap.begin(), maxHeap.end());
        maxHeap.pop_back();

        Node* nodeToBeProcessed = get<1>(currentTuple);
        double nodeProcessingWeight = get<0>(currentTuple);
        vector<Node*> currentPath = get<2>(currentTuple);

        processed.push_back(nodeToBeProcessed);

        if (nodeToBeProcessed == endNode) {
            
            // If we reach the end, return the weight, start, and end, as a tuple for comparison in highest()
            return make_tuple(nodeProcessingWeight, startNode, endNode);
        }

        vector<tuple<Node*, string, double>> adjacentNodes = nodeToBeProcessed->getAdjacentNodes();
        for (int i = 0; i < adjacentNodes.size(); i++) {

            Node* node = get<0>(adjacentNodes[i]);  
            double adjacentWeight = get<2>(adjacentNodes[i]);

            bool found = false;
            for (int j = 0; j < processed.size(); j++) {
                if (processed[j] == node) {
                    found = true;
                    break;
                }
            }

            if (found) {
                continue;
            }
            
            vector<Node*> newPath = currentPath;
            newPath.push_back(node);

            maxHeap.push_back(make_tuple(nodeProcessingWeight + adjacentWeight, node, newPath));
            push_heap(maxHeap.begin(), maxHeap.end());

        }   
    }

    // No path found, just send to default values
    return make_tuple(0, nullptr, nullptr);
}

void Graph::highest() {    

    // No vertices, failure
    if (entities.size() == 0) {
        cout << "failure" << endl;
        return;
    }

    bool hasAnEdge = false;
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->getAdjacentNodes().size() > 0) {
            hasAnEdge = true;
        }
    }

    // Graph is disconnected, failure
    if (hasAnEdge == false) {
        cout << "failure" << endl;
        return;
    }

    tuple<double, Node*, Node*>  maxWeightedPath = make_tuple(0, nullptr, nullptr);

    // Compare every pair of entities in the graph to find the highest weighted path
    for (int i = 0; i < entities.size(); i++) {
        for (int j = i + 1; j < entities.size(); j++) {
            tuple<double, Node*, Node*> comparisonPath = pathHelper(entities[i], entities[j]); 
            if (get<0>(comparisonPath) > get<0>(maxWeightedPath)) {

                if (get<1>(comparisonPath) != nullptr) {
                    maxWeightedPath = comparisonPath;
                }

            }
        }
    }

    cout << get<1>(maxWeightedPath)->getId() << " " << get<2>(maxWeightedPath)->getId() << " " << get<0>(maxWeightedPath) << endl;

}

void Graph::findAll(string type, string string) {

    if (type == "name") {

        // Flag to see if there has been at least once match
        bool atLeastOne = false;
        for (int i = 0; i < entities.size(); i++) {
            if (entities[i]->getName() == string) {
                atLeastOne = true;
                cout << entities[i]->getId();

                if (i != entities.size() - 1) {
                    cout << " ";
                }
            }
        }

        // No matches exist, failure
        if (atLeastOne == false) {
            cout << "failure" << endl;
        } else {
            cout << endl;
        }
    } else if (type == "type") {

        // Flag to see if there is a match
        bool atLeastOne = false;
        for (int i = 0; i < entities.size(); i++) {
            if (entities[i]->getLabel() == string) {
                cout << entities[i]->getId();
                atLeastOne = true;

                if (i != entities.size() - 1) {
                    cout << " ";
                }
            }
        }

        // No matches, therefore no nodes in the graph contain the input type
        if (atLeastOne == false) {
            cout << "failure" << endl;
        } else {
            cout << endl;
        }
    }
}
