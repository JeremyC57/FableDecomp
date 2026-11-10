// Behaviour test: the tree's static _S_value (0x0083AE10) returns the value field of a node.
#include <cstdio>
struct Key { long k; };
struct Data { long d; };
struct Value { Key first; Data second; };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value value; };
struct Select1st { const Key& operator()(const Value& v) const { return v.first; } };
struct iterator {
    NodeBase* node;
    iterator() { node = 0; }
    iterator(Node* x) { node = x; }
    Value& operator*() const { return ((Node*)node)->value; }
};
struct const_iterator {
    NodeBase* node;
    const_iterator(Node* x) { node = x; }
};
struct Tree {
    Node* header;
    unsigned int count;
    iterator end() { return iterator(header); }
    static Value& S_value(Node* x) { return x->value; }
};
int main()
{
    Node n; n.value.second.d = 3;
    bool ok = &Tree::S_value(&n) == &n.value && Tree::S_value(&n).second.d == 3;
    if (!ok) { std::printf("0083ae10_TEST FAIL\n"); return 1; }
    std::printf("0083ae10_TEST PASS\n");
    return 0;
}