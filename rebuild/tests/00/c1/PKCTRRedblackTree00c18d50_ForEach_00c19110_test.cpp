// behaviour test for 00c19110 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct Node { Node* Left; Node* Right; Node* Parent; long Colour; void* Key; };
struct Tree { Node Sentinel; Node* Root; Node* LastFound; };
static Node* mk(Node* n, Tree& t, void* key) { n->Left = &t.Sentinel; n->Right = &t.Sentinel; n->Parent = 0; n->Colour = 0; n->Key = key; return n; }
static unsigned long K[5] = { 10, 20, 30, 40, 50 };
static void build(Tree& t, Node* n) {   // a balanced tree over K: 30 / (20 (10)) (40 (50))
    for (int i = 0; i < 5; i++) mk(&n[i], t, &K[i]);
    t.Root = &n[2]; n[2].Left = &n[1]; n[1].Left = &n[0]; n[2].Right = &n[3]; n[3].Right = &n[4]; t.LastFound = 0; }

static int g_seen[8]; static int g_n = 0;
struct Visitor { void Visit(void** key) { g_seen[g_n++] = (int)*(unsigned long*)*key; } };
static void VisitInOrder(Tree& t, Node* node, Visitor* v) { while (node != &t.Sentinel) { VisitInOrder(t, node->Left, v); v->Visit(&node->Key); node = node->Right; } }
static void ForEach(Tree& t, Visitor* v) { VisitInOrder(t, t.Root, v); }

int main()
{
    Tree t; Node n[5]; build(t, n); Visitor v; ForEach(t, &v);
    if (!(g_n == 5 && g_seen[0] == 10 && g_seen[2] == 30 && g_seen[4] == 50)) { std::printf("00c19110_TEST FAIL\n"); return 1; }
    std::printf("00c19110_TEST PASS\n");
    return 0;
}