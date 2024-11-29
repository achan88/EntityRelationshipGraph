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


// 
void Graph::path(string startId, string endId) {

    Node* startNode = searchForNode(startId);
    Node* endNode = searchForNode(endId);

    // If either of them is nullptr, node doesn't exist in the graph
    if (startNode == nullptr || endNode == nullptr) {
        cout << "failure" << endl;
        return;
    }

    // Disclaimer: The idea to use a maxHeap that stores the previous node (parent node), 
    // and the current path that led up to the node was produced by generative AI
    vector<tuple<double, Node*, Node*, vector<Node*>>> maxHeap;

    tuple<double, Node*, Node*, vector<Node*>> startOfPath = make_tuple(0, startNode, nullptr, vector<Node*>{startNode});
    maxHeap.push_back(startOfPath);

    vector<Node*> visited;

    make_heap(maxHeap.begin(), maxHeap.end());

    while(!maxHeap.empty()) {

        tuple<double, Node*, Node*, vector<Node*>> currentTuple = maxHeap.front();
        pop_heap(maxHeap.begin(), maxHeap.end());
        maxHeap.pop_back();

        Node* nodeToBeProcessed = get<1>(currentTuple);
        double nodeProcessingWeight = get<0>(currentTuple);
        vector<Node*> currentPath = get<3>(currentTuple);

        bool found = false;
        for (int i = 0; i < visited.size(); i++) {
            if (visited[i] == nodeToBeProcessed) {
                found = true;
                break;
            }
        }

        if (found) {
            continue;
        }

        visited.push_back(nodeToBeProcessed);

        if (nodeToBeProcessed == endNode) {
            for (int i = 0; i < currentPath.size(); i++) {
                cout << currentPath[i]->getId();

                if (i < currentPath.size() - 1) {
                    cout << " ";
                }
            }

            cout << endl;
            return;
        }

        vector<tuple<Node*, string, double>> adjacentNodes = nodeToBeProcessed->getAdjacentNodes();
        for (int i = 0; i < adjacentNodes.size(); i++) {

            Node* node = get<0>(adjacentNodes[i]);  
            double adjacentWeight = get<2>(adjacentNodes[i]);

            bool found = false;
            for (int j = 0; j < visited.size(); j++) {
                if (visited[j] == node) {
                    found = true;
                    break;
                }
            }

            if (found) {
                continue;
            }

            



        }





    }








}

        