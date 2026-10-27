// behaviour test for 00c19130 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct Node { Node* Left; Node* Right; Node* Parent; long Colour; void* Key; };
struct Tree { Node Sentinel; Node* Root; Node* LastFound; };
static Node* mk(Node* n, Tree& t, void* key) { n->Left = &t.Sentinel; n->Right = &t.Sentinel; n->Parent = 0; n->Colour = 0; n->Key = key; return n; }
static unsigned long K[5] = { 10, 20, 30, 40, 50 };
static void build(Tree& t, Node* n) {   // a balanced tree over K: 30 / (20 (10)) (40 (50))
    for (int i = 0; i < 5; i++) mk(&n[i], t, &K[i]);
    t.Root = &n[2]; n[2].Left = &n[1]; n[1].Left = &n[0]; n[2].Right = &n[3]; n[3].Right = &n[4]; t.LastFound = 0; }

static void CountNodes(Tree& t, Node* node, long* c) { while (node != &t.Sentinel) { CountNodes(t, node->Left, c); ++*c; node = node->Right; } }
static long GetCount(Tree& t) { long count = 0; CountNodes(t, t.Root, &count); return count; }

int main()
{
    Tree t; Node n[5]; build(t, n);
    if (!(GetCount(t) == 5)) { std::printf("00c19130_TEST FAIL\n"); return 1; }
    t.Root = &t.Sentinel;
    if (!(GetCount(t) == 0)) { std::printf("00c19130_TEST FAIL\n"); return 1; }
    std::printf("00c19130_TEST PASS\n");
    return 0;
}