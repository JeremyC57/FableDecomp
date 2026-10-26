// behaviour test for 0x004ABA70 std::_Rb_tree::_M_erase [set<CCharString>](x): every node of the subtree is visited once, its value destroyed once before its node is freed, right subtrees before the node itself, and a null root does nothing
#include <cstdio>
#include <cstdlib>
static int gDestroyed = 0;
static int gFreed = 0;
static int gOrder[16];
struct Value {                                 // stands for the value type: a destructor with an observable effect
    int Tag;
    ~Value() { gOrder[gDestroyed & 15] = Tag; ++gDestroyed; }
};
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value value; };
static void FreeNode(void* p) { ++gFreed; std::free(p); }
struct Tree {
    NodeBase* header; unsigned int count;
    void Erase(Node* x)                        // the function under test (STLport 4.6 _M_erase)
    {
        while (x != 0) {
            Erase((Node*)x->right);
            Node* y = (Node*)x->left;
            x->value.~Value();
            FreeNode(x);
            x = y;
        }
    }
};
static Node* Make(int tag, Node* l, Node* r)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->color = false; n->parent = 0; n->left = l; n->right = r; n->value.Tag = tag;
    return n;
}
int main()
{
    int fail = 0;
    Tree t; t.header = 0; t.count = 0;
    t.Erase(0);
    if (gDestroyed != 0 || gFreed != 0) fail |= 1;
    Node* root = Make(4, Make(2, Make(1, 0, 0), Make(3, 0, 0)), Make(6, Make(5, 0, 0), Make(7, 0, 0)));
    t.Erase(root);
    if (gDestroyed != 7 || gFreed != 7) fail |= 2;
    static const int want[7] = { 7, 6, 5, 4, 3, 2, 1 };   // right subtree first, then the node, then down the left spine
    for (int i = 0; i < 7; ++i)
        if (gOrder[i] != want[i]) fail |= 4;
    if (fail) { std::printf("004aba70_TEST FAIL %d\n", fail); return 1; }
    std::printf("004aba70_TEST PASS\n");
    return 0;
}