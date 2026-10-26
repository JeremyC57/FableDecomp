// Behaviour test: _Rb_tree<u64,pair<const u64,CThingPosAndOrientation>>::_M_clone_node -- a fresh node with the value and colour, no children
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C3DAngles { public: float X, Y, Z; };
struct CThingPosAndOrientation {
    C3DVector Position;
    C3DAngles FacingAngles;
};
struct SValue {                                         // pair<const u64, CThingPosAndOrientation>
    u64 first;
    CThingPosAndOrientation second;
};
struct Node : NodeBase {
    SValue value;
    u64 value_key() const { return value.first; }
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
    u64 key = ((u64)0x12345 << 16);
    SValue v; v.first = key; v.second.Position.X = (float)key; v.second.Position.Y = 1.0f; v.second.Position.Z = 2.0f; v.second.FacingAngles.X = v.second.FacingAngles.Y = v.second.FacingAngles.Z = 0.5f;
    Node* x = CreateNode(v);
    x->color = true;
    x->parent = x->left = x->right = x;                  // the clone must not inherit the links
    Node* c = CloneNode(x);
    if (c == x || !c->color || c->left != 0 || c->right != 0 || c->value_key() != key)
        { std::printf("00825a80_TEST FAIL\n"); return 1; }
    if (c->value.second.Position.X != x->value.second.Position.X || c->value.second.FacingAngles.Y != 0.5f)
        { std::printf("00825a80_TEST FAIL\n"); return 1; }
    std::printf("00825a80_TEST PASS\n");
    return 0;
}