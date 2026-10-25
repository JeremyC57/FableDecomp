// Behaviour test for 005ba8cf: std::_Rb_tree<EClothingSuitPart,..>::_M_insert -- link left/right of y, maintain the header's root/leftmost/rightmost, rebalance once, count
#include <cstdio>
#include <cstdlib>
enum EClothingSuitPart { PART_0, PART_1, PART_2, PART_3, PART_4, PART_5 };
struct Node { bool color; Node* parent; Node* left; Node* right; EClothingSuitPart value; };
static int g_rebalanced = 0;
static void Rebalance(Node*, Node*&) { ++g_rebalanced; }
struct Tree {
    Node* header; unsigned int count;
    Node* CreateNode(const EClothingSuitPart& v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->value = v; return n; }
    Node* Insert(Node* x, Node* y, const EClothingSuitPart& v, Node* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || v < y->value))) {
            z = CreateNode(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; } else if (y == header->left) header->left = z;
        } else {
            z = CreateNode(v); y->right = z; if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent); ++count; return z;
    }
};
int main()
{
    Node h; h.color = false; h.parent = 0; h.left = &h; h.right = &h;
    Tree t; t.header = &h; t.count = 0;
    EClothingSuitPart k3 = PART_3, k1 = PART_1, k5 = PART_5, k4 = PART_4;
    Node* a = t.Insert(0, &h, k3, 0);
    int ok = h.parent == a && h.left == a && h.right == a && a->parent == &h && a->left == 0 && a->right == 0;
    Node* b = t.Insert(0, a, k1, 0);        // less than y: left, new leftmost
    ok &= a->left == b && h.left == b && h.right == a && b->parent == a;
    Node* c = t.Insert(0, a, k5, 0);        // greater: right, new rightmost
    ok &= a->right == c && h.right == c && h.left == b;
    Node* d = t.Insert(0, c, k4, 0);        // less than y (5): left of c, not leftmost
    ok &= c->left == d && h.left == b && h.right == c;
    Node* e = t.Insert(0, d, k1, d);        // w != 0 forces the right side
    ok &= d->right == e && t.count == 5 && g_rebalanced == 5;
    std::printf(ok ? "005ba8cf_TEST PASS\n" : "005ba8cf_TEST FAIL\n");
    return ok ? 0 : 1;
}
