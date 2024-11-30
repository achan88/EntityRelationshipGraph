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
        } else if (command == "ENTITY") {
            string id;
            string name;
            string type;

            cin >> id >> name >> type;

            graph.addEntity(id, name, type);
        } else if (command == "PRINT") {
            string id;
            cin >> id;

            graph.printAdjacent(id);
        } else if (command == "DELETE") {
            string id;
            cin >> id;

            graph.deleteEntity(id);
        } else if (command == "PATH") {
            string id1;
            string id2;
            cin >> id1 >> id2;

            graph.path(id1, id2);
        } else if (command == "HIGHEST") {

            cout << "TODO" << endl;
        } else if (command == "FINDALL") {
            string type;
            string string;

            graph.findAll(type, string);
        } else if (command == "EXIT") {
            break;
        }
    }
}