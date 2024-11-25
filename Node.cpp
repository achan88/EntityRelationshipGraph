#include "Node.h"

Node::Node(string label, string id, string name) {
    this->label = label;
    this->id = id;
    this->name = name;
}
        
string Node::getLabel() {

    return label;
}
void Node::setLabel(string newLabel) {
    label = newLabel;
}

string Node::getId() {
    return id;

}

string Node::getName() {
    return name;
}

void Node::setName(string newName) {
    name = newName;
}

