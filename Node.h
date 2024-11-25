#include <string>
#include <vector>
#include <tuple>
using namespace std;


class Node {
    private:
        string label;
        string id;
        string name;

        vector<tuple<Node*, string, double>> adjacentNodes;

    public:
        Node(string label, string id, string name);

        vector<tuple<Node*, string, double>>& getAdjacentNodes();

        string getLabel();
        void setLabel(string newLabel);

        string getId();

        string getName();
        void setName(string newName);



};