// behaviour test for 0x0051A2F0 std::_Rb_tree::erase(iterator) [map<CThingBuilding*, list<CThingAICreature*> >]: the unlink helper gets the node and the header's root / leftmost / rightmost links, the returned node's value is destroyed once and the node freed, and the count drops by one
#include <cstdio>
#include <cstdlib>
static int gDestroyed = 0, gFreed = 0;
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Value { int Tag; ~Value() { ++gDestroyed; } };
struct Node : NodeBase { Value value; };
static NodeBase* gSeenZ = 0; static NodeBase** gSeenRoot = 0; static NodeBase** gSeenLeft = 0; static NodeBase** gSeenRight = 0;
static NodeBase* RebalanceForErase(NodeBase* z, NodeBase*& root, NodeBase*& leftmost, NodeBase*& rightmost)
{
    gSeenZ = z; gSeenRoot = &root; gSeenLeft = &leftmost; gSeenRight = &rightmost;
    root = 0; leftmost = 0; rightmost = 0;                    // the only node goes: the tree becomes empty
    return z;
}
struct Tree {
    NodeBase* header; unsigned int count;
    void Erase(NodeBase* position)                           // the function under test (STLport 4.6)
    {
        Node* y = (Node*)RebalanceForErase(position, header->parent, header->left, header->right);
        y->value.~Value();
        if (y != 0) { ++gFreed; std::free(y); }
        --count;
    }
};
int main()
{
    int fail = 0;
    NodeBase h; h.color = false;
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->parent = &h; n->left = 0; n->right = 0; n->value.Tag = 5;
    h.parent = n; h.left = n; h.right = n;
    Tree t; t.header = &h; t.count = 1;
    t.Erase(n);
    if (gSeenZ != n || gSeenRoot != &h.parent || gSeenLeft != &h.left || gSeenRight != &h.right) fail |= 1;
    if (gDestroyed != 1 || gFreed != 1 || t.count != 0) fail |= 2;
    if (fail) { std::printf("0051a2f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("0051a2f0_TEST PASS\n");
    return 0;
}