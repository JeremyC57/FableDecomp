// Behaviour test: _Rb_tree<u64,pair<const u64,CThingPosAndOrientation>>::_M_insert -- link a new node under y, keep leftmost/rightmost, rebalance and count
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
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C3DAngles { float X, Y, Z; };
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
static int g_rebalanced = 0;
static void Rebalance(NodeBase*, NodeBase*&) { ++g_rebalanced; }   // stand-in for _Rb_global<bool>::_Rebalance 0x0042971D

struct CTree {                                          // header pointer, count (the compare is empty)
    NodeBase*    header;
    unsigned int count;
    NodeBase*& root() const { return header->parent; }
    NodeBase*& leftmost() const { return header->left; }
    NodeBase*& rightmost() const { return header->right; }
    static u64 Key(const SValue& v) { return v.first; }
    static u64 KeyOf(NodeBase* n) { return ((Node*)n)->value_key(); }
    Node* CreateNode(const SValue& v)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        new (&n->value) SValue(v);
        return n;
    }
    NodeBase* Insert(NodeBase* x, NodeBase* y, const SValue& v, NodeBase* w)   // STLport _M_insert
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || Key(v) < KeyOf(y)))) {
            z = CreateNode(v);
            y->left = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            }
            else if (y == leftmost())
                leftmost() = z;
        }
        else {
            z = CreateNode(v);
            y->right = z;
            if (y == rightmost())
                rightmost() = z;
        }
        z->parent = y;
        z->left = 0;
        z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};

int main()
{
    NodeBase head;
    head.color = false;
    head.parent = 0;
    head.left = head.right = &head;
    CTree t;
    t.header = &head;
    t.count = 0;
    SValue a; a.first = 50; a.second.Position.X = a.second.Position.Y = a.second.Position.Z = 1.0f; a.second.FacingAngles.X = a.second.FacingAngles.Y = a.second.FacingAngles.Z = 0.0f;
    NodeBase* n50 = t.Insert(0, &head, a, 0);                        // into the empty tree: root, leftmost, rightmost
    if (t.root() != n50 || t.leftmost() != n50 || t.rightmost() != n50 || n50->parent != &head || t.count != 1)
        { std::printf("0082f7b0_TEST FAIL\n"); return 1; }
    SValue b; b.first = 20; b.second.Position.X = b.second.Position.Y = b.second.Position.Z = 1.0f; b.second.FacingAngles.X = b.second.FacingAngles.Y = b.second.FacingAngles.Z = 0.0f;
    NodeBase* n20 = t.Insert(0, n50, b, 0);                          // smaller key: left child, new leftmost
    SValue c; c.first = 90; c.second.Position.X = c.second.Position.Y = c.second.Position.Z = 1.0f; c.second.FacingAngles.X = c.second.FacingAngles.Y = c.second.FacingAngles.Z = 0.0f;
    NodeBase* n90 = t.Insert(0, n50, c, 0);                          // larger key: right child, new rightmost
    if (n50->left != n20 || n50->right != n90 || t.leftmost() != n20 || t.rightmost() != n90 || n20->parent != n50 || t.count != 3)
        { std::printf("0082f7b0_TEST FAIL\n"); return 1; }
    SValue d; d.first = 30; d.second.Position.X = d.second.Position.Y = d.second.Position.Z = 1.0f; d.second.FacingAngles.X = d.second.FacingAngles.Y = d.second.FacingAngles.Z = 0.0f;
    NodeBase* n30 = t.Insert(0, n20, d, n20);                        // a hint w forces the right side
    SValue e; e.first = 25; e.second.Position.X = e.second.Position.Y = e.second.Position.Z = 1.0f; e.second.FacingAngles.X = e.second.FacingAngles.Y = e.second.FacingAngles.Z = 0.0f;
    NodeBase* n25 = t.Insert(n30, n30, e, 0);                        // x != 0 forces the left side
    if (n20->right != n30 || n30->left != n25 || t.leftmost() != n20 || t.count != 5 || g_rebalanced != 5
        || ((Node*)n25)->value_key() != 25 || n25->left != 0 || n25->right != 0)
        { std::printf("0082f7b0_TEST FAIL\n"); return 1; }
    if (((Node*)n90)->value.second.Position.Z != 1.0f)
        { std::printf("0082f7b0_TEST FAIL\n"); return 1; }
    std::printf("0082f7b0_TEST PASS\n");
    return 0;
}