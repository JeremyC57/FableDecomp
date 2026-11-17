// Behaviour test for 0x00777790 _Rb_tree<long, pair<const long, CShopItem>>::_M_insert(x, y, v, w)
#include <cstdio>
#include <cstdlib>

class CShopItem { public: float Quantity, PriceMultiplier, MaxStock; long RestockPeriod; float A, B, C; };
struct Value { long first; CShopItem second; };
struct NodeBase { long Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
struct Node : NodeBase { Value V; };

static int g_rebalances = 0;
static void Rebalance(NodeBase*, NodeBase*&) { ++g_rebalances; }   // 0x0042971D stand-in (colouring not under test)

struct Tree {
    NodeBase* Header;
    unsigned int Count;
    Node* Create(const Value& v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->V = v; return n; }
    NodeBase* Insert(NodeBase* x, NodeBase* yb, const Value& v, NodeBase* w)
    {
        Node* y = (Node*)yb;
        Node* z;
        if (yb == Header || (w == 0 && (x != 0 || v.first < y->V.first))) {
            z = Create(v);
            y->Left = z;
            if (yb == Header) { Header->Parent = z; Header->Right = z; }
            else if (yb == Header->Left) Header->Left = z;
        }
        else {
            z = Create(v);
            y->Right = z;
            if (yb == Header->Right) Header->Right = z;
        }
        z->Parent = y; z->Left = 0; z->Right = 0;
        Rebalance(z, Header->Parent);
        ++Count;
        return z;
    }
};

int main()
{
    NodeBase header = { 0, 0, 0, 0 };
    header.Left = &header; header.Right = &header;
    Tree t = { &header, 0 };
    Value v10 = { 10, { 1.0f, 2.0f, 3.0f, 4, 5.0f, 6.0f, 7.0f } };
    Node* root = (Node*)t.Insert(0, &header, v10, 0);                 // empty tree: root, leftmost and rightmost
    if (header.Parent != root || header.Left != root || header.Right != root || t.Count != 1 || root->Parent != &header) { std::printf("00777790_TEST FAIL\n"); return 1; }
    if (root->V.first != 10 || root->V.second.RestockPeriod != 4 || root->V.second.C != 7.0f) { std::printf("00777790_TEST FAIL\n"); return 1; }
    Value v5 = v10; v5.first = 5;
    Node* left = (Node*)t.Insert(0, root, v5, 0);                     // key below the parent: new leftmost
    if (root->Left != left || header.Left != left || header.Right != root || t.Count != 2) { std::printf("00777790_TEST FAIL\n"); return 1; }
    Value v20 = v10; v20.first = 20;
    Node* right = (Node*)t.Insert(0, root, v20, 0);                   // key above the parent: new rightmost
    if (root->Right != right || header.Right != right || header.Left != left || right->Left != 0 || t.Count != 3) { std::printf("00777790_TEST FAIL\n"); return 1; }
    Value v1 = v10; v1.first = 1;
    Node* forced = (Node*)t.Insert(0, right, v1, right);              // w != 0 forces the right link whatever the key
    if (right->Right != forced || header.Right != forced || g_rebalances != 4) { std::printf("00777790_TEST FAIL\n"); return 1; }
    std::printf("00777790_TEST PASS\n");
    return 0;
}