// Behaviour test: the tree iterator's operator* (0x0083ADA0) returns the value field of the node it is on.
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
    Node n; n.value.first.k = 4; n.value.second.d = 9;
    iterator it(&n);
    bool ok = &*it == &n.value && (*it).second.d == 9;
    (*it).second.d = 11;
    ok = ok && n.value.second.d == 11;
    if (!ok) { std::printf("0083ada0_TEST FAIL\n"); return 1; }
    std::printf("0083ada0_TEST PASS\n");
    return 0;
}