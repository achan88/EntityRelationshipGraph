#include "Graph.h"
#include <string>
#include <iostream>

int main() {

    Graph graph;
    string command;
    

    while (cin >> command) {

        if (command == "LOAD") {
           cout << "TODO" << endl;
        } else if (command == "RELATIONSHIP") {
            string sourceId;
            string label;
            string destinationId;
            double weight;

            cin >> sourceId >> label >> destinationId >> weight;
            graph.addRelationship(sourceId, label, destinationId, weight);
        }
    }
}