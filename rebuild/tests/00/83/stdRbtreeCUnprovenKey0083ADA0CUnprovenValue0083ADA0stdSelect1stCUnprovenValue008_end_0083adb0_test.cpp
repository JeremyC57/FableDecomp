// Behaviour test: the tree's end() (0x0083ADB0) is an iterator on the header node.
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
    Node header; Tree t; t.header = &header; t.count = 0;
    bool ok = t.end().node == &header;
    if (!ok) { std::printf("0083adb0_TEST FAIL\n"); return 1; }
    std::printf("0083adb0_TEST PASS\n");
    return 0;
}