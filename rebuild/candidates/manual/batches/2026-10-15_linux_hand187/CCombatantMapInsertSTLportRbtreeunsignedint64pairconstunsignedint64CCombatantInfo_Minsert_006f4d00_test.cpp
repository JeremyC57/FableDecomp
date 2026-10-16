#include <cstdio>
#include <cstdlib>
// Behaviour test for the STLport _M_insert of CCombatantMap @ 0x006F4D00 (links, leftmost/rightmost, count).
struct Value { unsigned long long first; int info; };
struct NodeBase { long color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value v; };
static int g_rebalances = 0;
static void Rebalance(NodeBase*, NodeBase*&) { ++g_rebalances; }
struct Map {
    NodeBase* header; unsigned long count;
    Node* Create(const Value& v) { Node* t = (Node*)std::malloc(sizeof(Node)); t->v = v; return t; }
    Node* Insert(NodeBase* x_, NodeBase* y_, const Value& v, NodeBase* w_)
    {
        Node* w = (Node*)w_; Node* y = (Node*)y_; Node* x = (Node*)x_; Node* z;
        if (y == header || (w == 0 && (x != 0 || v.first < y->v.first))) {
            z = Create(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = Create(v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};
static int Fail() { std::printf("006f4d00_TEST FAIL\n"); return 1; }
int main()
{
    NodeBase h = {0, 0, 0, 0}; h.left = &h; h.right = &h;
    Map m = {&h, 0};
    Value a = {10, 1}, b = {5, 2}, c = {20, 3};
    Node* ra = m.Insert(0, &h, a, 0);
    if (h.parent != ra || h.left != ra || h.right != ra || ra->parent != &h || m.count != 1) return Fail();
    Node* rb = m.Insert(0, ra, b, 0);          // 5 < 10: left of root, new leftmost
    if (ra->left != rb || h.left != rb || h.right != ra) return Fail();
    Node* rc = m.Insert(0, ra, c, 0);          // 20 > 10: right of root, new rightmost
    if (ra->right != rc || h.right != rc || m.count != 3 || g_rebalances != 3 || rc->v.info != 3) return Fail();
    std::free(ra); std::free(rb); std::free(rc);
    std::printf("006f4d00_TEST PASS\n");
    return 0;
}
