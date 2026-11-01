// Test for ~CTCHeroOpinionDeedLog (0x006C5D70): every deed pointer held in the handle -> deed map is deleted exactly once
// (null entries skipped by delete's own test), the end node is read once before the walk, and the walk visits the entries
// in key order through an in-order successor (right subtree's leftmost node, else the first ancestor reached from the left).
#include <cstdio>
static int g_deleted = 0;
struct Deed { long handle; ~Deed() { ++g_deleted; } };
struct Node { Node* parent; Node* left; Node* right; long key; Deed* value; };
static Node* Increment(Node* n)
{
    if (n->right) { n = n->right; while (n->left) n = n->left; }
    else { Node* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    return n;
}
static long g_order[8]; static int g_count = 0;
static void DestroyLog(Node* header)
{
    Node* end = header;
    for (Node* it = header->left; it != end; it = Increment(it)) { g_order[g_count++] = it->key; delete it->value; }
}
int main()
{
    // tree: 20 (root) with children 10 and 30; 30 has left child 25
    Node header, n10, n20, n25, n30;
    n20.parent = &header; n20.left = &n10; n20.right = &n30; n20.key = 20; n20.value = new Deed;
    n10.parent = &n20; n10.left = 0; n10.right = 0; n10.key = 10; n10.value = new Deed;
    n30.parent = &n20; n30.left = &n25; n30.right = 0; n30.key = 30; n30.value = new Deed;
    n25.parent = &n30; n25.left = 0; n25.right = 0; n25.key = 25; n25.value = 0;
    header.parent = &n20; header.left = &n10; header.right = &n30;
    DestroyLog(&header);
    if (g_deleted != 3 || g_count != 4 || g_order[0] != 10 || g_order[1] != 20 || g_order[2] != 25 || g_order[3] != 30) {
        std::printf("006c5d70_TEST FAIL\n"); return 1;
    }
    Node empty; empty.parent = 0; empty.left = &empty; empty.right = &empty; g_count = 0;
    DestroyLog(&empty);
    if (g_count != 0) { std::printf("006c5d70_TEST FAIL\n"); return 1; }
    std::printf("006c5d70_TEST PASS\n");
    return 0;
}