#include <cstdio>
// Behaviour test for the CCombatantMap range erase @ 0x006F4E40 (successor walk, per-node destroy, clear path).
struct Node { long color; Node* parent; Node* left; Node* right; int destroyed; };
static int g_freed = 0, g_cleared = 0;
static Node* Unlink(Node* n) { return n; }   // rebalance-for-erase stand-in: the victim itself
struct Map {
    Node* header; unsigned long size;
    static Node* Successor(Node* node)
    {
        if (node->right) { node = node->right; while (node->left) node = node->left; }
        else { Node* y = node->parent; while (node == y->right) { node = y; y = y->parent; } if (node->right != y) node = y; }
        return node;
    }
    void Erase(Node*) { ++g_cleared; }
    void EraseRange(Node* first, Node* last)
    {
        if (first == header->left && last == header) {
            if (size) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; size = 0; }
        } else {
            while (first != last) { Node* v = first; first = Successor(first); Node* n = Unlink(v); ++n->destroyed; if (n) ++g_freed; --size; }
        }
    }
};
static int Fail() { std::printf("006f4e40_TEST FAIL\n"); return 1; }
int main()
{
    Node h = {0, 0, 0, 0, 0}, a = {0, 0, 0, 0, 0}, b = {0, 0, 0, 0, 0}, c = {0, 0, 0, 0, 0};
    // b is the root with children a (left) and c (right)
    h.parent = &b; h.left = &a; h.right = &c; b.parent = &h; b.left = &a; b.right = &c; a.parent = &b; c.parent = &b;
    Map m = {&h, 3};
    m.EraseRange(&a, &c);                     // erases a and b
    if (m.size != 1 || a.destroyed != 1 || b.destroyed != 1 || c.destroyed != 0 || g_freed != 2) return Fail();
    Map m2 = {&h, 3}; h.left = &a;
    m2.EraseRange(&a, &h);                    // whole range: clear
    if (g_cleared != 1 || m2.size != 0 || h.left != &h || h.right != &h || h.parent != 0) return Fail();
    std::printf("006f4e40_TEST PASS\n");
    return 0;
}
