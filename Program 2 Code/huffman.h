#include <string>
#include <vector>
#include <map>

class huffman{

struct node {
    int freq;
    char name;
    node* left;
    node* right;
    bool isLeaf=false;
} ;

// struct leaf: public node {
//     leaf(char nodename,int frequency){
//         freq=frequency;
//         name = nodename;
//     }
//     int freq = 0;
//     node* parent=nullptr;
//     char name;
//     bool isLeaf = true;
// };

// struct parent:node {
//     int freq = 0;
//     node* left;
//     node* right;
//     bool isLeaf = false;
//     parent(int frequency, node* leftnode,node* rightnode){
//         freq = frequency;
//         left = leftnode;
//         right = rightnode;
//     }
// };

public:
    huffman();
    void load(std::string&);
    void construct();
    void encode(std::string&);
    void decode(std::string&);
    void clear();
    ~huffman();

    std::string info;

private:
    void encodeNode(node*,std::string);
    std::map<char,int> frequencyMap;
    std::vector<std::pair<char,int>> frequencyVector;
    std::vector<node*> list;
    // std::vector<node*> children;
    std::map<char,std::string> code;
};
