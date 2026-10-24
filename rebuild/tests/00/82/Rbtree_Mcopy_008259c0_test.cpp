// Behaviour test: _Rb_tree<u64,u64>::_M_copy -- clone a subtree: same keys, colours and shape, fresh nodes linked to the new parent
#include <cstdio>
#include <cstdlib>
#include <new>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct NodeBase {                                       // STLport _Rb_tree_node_base
    bool      color;
    NodeBase* parent;
    NodeBase* left;
    NodeBase* right;
};
typedef u64 SValue;
struct Node : NodeBase {
    SValue value;
    u64 value_key() const { return value; }
};

static int g_nodes = 0;
static Node* CreateNode(const SValue& v)                 // _M_create_node: malloc + placement copy
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    new (&n->value) SValue(v);
    ++g_nodes;
    return n;
}
static Node* CloneNode(Node* x)                          // STLport _M_clone_node
{
    Node* tmp = CreateNode(x->value);
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
}
static Node* Copy(Node* x, Node* p)                      // STLport _M_copy
{
    Node* top = CloneNode(x);
    top->parent = p;
    if (x->right)
        top->right = Copy((Node*)x->right, top);
    p = top;
    x = (Node*)x->left;
    while (x != 0) {
        Node* y = CloneNode(x);
        p->left = y;
        y->parent = p;
        if (x->right)
            y->right = Copy((Node*)x->right, y);
        p = y;
        x = (Node*)x->left;
    }
    return top;
}
static bool SameShape(const NodeBase* a, const NodeBase* b, const NodeBase* pa, const NodeBase* pb)
{
    if (!a || !b)
        return a == b;
    if (a == b || a->color != b->color || a->parent != pa || b->parent != pb)
        return false;
    if (((const Node*)a)->value_key() != ((const Node*)b)->value_key())
        return false;
    return SameShape(a->left, b->left, a, b) && SameShape(a->right, b->right, a, b);
}

int main()
{
    // a hand-built tree: 50 (black) with 30 (red: 20, 40) on the left and 70 on the right
    NodeBase header;
    Node* n[6];
    u64 keys[6] = { 50, 30, 70, 20, 40, 60 };
    for (int i = 0; i < 6; ++i) {
        u64 key = keys[i];
        SValue v = key;
        n[i] = CreateNode(v);
        n[i]->color = (i == 1);
        n[i]->left = n[i]->right = 0;
    }
    n[0]->parent = &header; n[0]->left = n[1]; n[0]->right = n[2];
    n[1]->parent = n[0]; n[1]->left = n[3]; n[1]->right = n[4];
    n[2]->parent = n[0]; n[2]->left = n[5];
    n[3]->parent = n[1]; n[4]->parent = n[1]; n[5]->parent = n[2];
    g_nodes = 0;
    NodeBase header2;
    Node* root = Copy(n[0], (Node*)&header2);
    if (g_nodes != 6 || !SameShape(n[0], root, &header, &header2))
        { std::printf("008259c0_TEST FAIL\n"); return 1; }
    std::printf("008259c0_TEST PASS\n");
    return 0;
}