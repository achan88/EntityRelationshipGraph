#include "Graph.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "illegal_exception.h"
using namespace std;

Graph::~Graph() {
    for (int i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
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

void Graph::load(string filename, string type) {

    ifstream file(filename);
    string line;

    if (type == "entities") {

        while (getline(file, line)) {
            istringstream stringstream(line);
            string id;
            string name;
            string newType;

            stringstream >> id >> name >> newType;

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

            addRelationship(sourceId, label, destinationId, weight, false);
        }
    }

    cout << "success" << endl;

    file.close();
}

void Graph::addRelationship(string sourceId, string label, string destinationId, double weight, bool print) {
    Node* sourceNode = searchForNode(sourceId);
    Node* destinationNode = searchForNode(destinationId);

    if (sourceNode == nullptr || destinationNode == nullptr) {

        if (print) {
            cout << "failure" << endl;
        }
        return;
    }

    sourceNode->addAdjacentNode(destinationNode, label, weight);
    destinationNode->addAdjacentNode(sourceNode, label, weight);

    if (print) {
        cout << "success" << endl;
    }
}

void Graph::addEntity(string id, string name, string type, bool print) {
    Node* node = searchForNode(id);

    if (node) {
        node->setName(name);
        node->setLabel(type);
        if (print) {
            cout << "success" << endl;
        }
        
        return;
    }

    node = new Node(type, id, name);
    entities.push_back(node);

    if (print) {
        cout << "success" << endl;
    }

}

void Graph::printAdjacent(string id) {

    try {
        if (!isValidId(id)) {
            throw illegal_exception();
        }
    } catch (const illegal_exception&) {
        cout << "illegal argument" << endl;
        return;
    }

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

    try {
        if (!isValidId(id)) {
            throw illegal_exception();
        }
    } catch (const illegal_exception&) {
        cout << "illegal argument" << endl;
        return;
    }

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

    cout << "success" << endl;
}

void Graph::path(string startId, string endId) {

    try {
        if (!isValidId(startId) || !isValidId(endId)) {
            throw illegal_exception();
        }
    } catch (const illegal_exception&) {
        cout << "illegal argument" << endl;
        return;
    }

    Node* startNode = searchForNode(startId);
    Node* endNode = searchForNode(endId);

    // If either of them is nullptr, node doesn't exist in the graph
    if (startNode == nullptr || endNode == nullptr) {
        cout << "failure" << endl;
        return;
    }

    // Total path weight, current node, vector of node path (path taken all way up to and including that particular node). 
    // Double comes first because that's how the heap algorithm sorts tuples
    vector<tuple<double, Node*, vector<Node*>>> maxHeap;

    tuple<double, Node*, vector<Node*>> startOfPath = make_tuple(0, startNode, vector<Node*>{startNode});
    maxHeap.push_back(startOfPath);

    vector<Node*> processed;

    make_heap(maxHeap.begin(), maxHeap.end());

    while(!maxHeap.empty()) {

        tuple<double, Node*, vector<Node*>> currentTuple = maxHeap.front(); // This gets the top most element of the heap (neighbouring node with the highest weighted edge)
        pop_heap(maxHeap.begin(), maxHeap.end()); // pop_heap only pushes element to the back
        maxHeap.pop_back(); // Remove it from the actual vector

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

        // In order for a node to be marked processed, we want to add all neighbouring nodes for exploration
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

            // Node has already been visited, we don't want to re-add it to process again
            if (found) {
                continue;
            }
            
            vector<Node*> newPath = currentPath;
            newPath.push_back(node);

            maxHeap.push_back(make_tuple(nodeProcessingWeight + adjacentWeight, node, newPath));
            push_heap(maxHeap.begin(), maxHeap.end());

        }   
    }
    cout << "failure" << endl;
}

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
    return make_tuple(0, nullptr, nullptr);
}

void Graph::highest() {    

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

    if (hasAnEdge == false) {
        cout << "failure" << endl;
        return;
    }

    tuple<double, Node*, Node*>  maxWeightedPath = make_tuple(0, nullptr, nullptr);

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

        for (int i = 0; i < entities.size(); i++) {
            if (entities[i]->getName() == string) {
                cout << entities[i]->getId();

                if (i != entities.size() - 1) {
                    cout << " ";
                }
            }
        }
    } else if (type == "type") {
        for (int i = 0; i < entities.size(); i++) {
            if (entities[i]->getLabel() == string) {
                cout << entities[i]->getId();

                if (i != entities.size() - 1) {
                    cout << " ";
                }
            }
        }
    }

    cout << endl;
}
