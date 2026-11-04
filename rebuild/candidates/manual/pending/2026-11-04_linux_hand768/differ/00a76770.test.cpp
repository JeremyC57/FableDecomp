// Behaviour test for 0x00A76770 CNavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const: a model; checks the quadrant choice by the node position, recursion into a child, and the shared blocked node set one level down at the quadrant centre (a quarter cell off).
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Node { float x, y; int level, layer; Node* q[4]; bool leaf; };
static Node Blocked;
static float Cell[8] = { 64, 32, 16, 8, 4, 2, 1, 0.5f };
static const Node* Leaf(const Node* n, float px, float py)
{
    if (n->leaf) return n;
    float quarter = Cell[n->level] * 0.25f; int i = (px < n->x ? 0 : 2) + (py < n->y ? 0 : 1);
    if (n->q[i] == &Blocked) { Blocked.level = n->level + 1; Blocked.layer = n->layer; Blocked.x = n->x + (i >= 2 ? quarter : -quarter); Blocked.y = n->y + ((i & 1) ? quarter : -quarter); return &Blocked; }
    return Leaf(n->q[i], px, py);
}
int main()
{
    int ok = 1; Node leaf = { 0, 0, 1, 0, { 0, 0, 0, 0 }, true }; Node root = { 32, 32, 0, 3, { &leaf, &Blocked, &Blocked, &leaf }, false };
    ok &= Leaf(&root, 1, 1) == &leaf && Leaf(&root, 40, 40) == &leaf;
    const Node* b = Leaf(&root, 1, 40); ok &= b == &Blocked && b->level == 1 && b->layer == 3 && b->x == 16 && b->y == 48;
    b = Leaf(&root, 40, 1); ok &= b->x == 48 && b->y == 16;
    std::printf(ok ? "00A76770_TEST PASS\n" : "00A76770_TEST FAIL\n"); return ok ? 0 : 1;
}
