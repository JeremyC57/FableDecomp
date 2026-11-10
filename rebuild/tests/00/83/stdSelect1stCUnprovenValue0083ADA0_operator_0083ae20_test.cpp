// Behaviour test: _Select1st::operator() (0x0083AE20) returns the key, the pair's first member, by reference.
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
    Value v; v.first.k = 12; v.second.d = 1;
    Select1st s;
    bool ok = &s(v) == &v.first && s(v).k == 12;
    if (!ok) { std::printf("0083ae20_TEST FAIL\n"); return 1; }
    std::printf("0083ae20_TEST PASS\n");
    return 0;
}