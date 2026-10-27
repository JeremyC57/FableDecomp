// behaviour test for 00c19010 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct Node { Node* Left; Node* Right; Node* Parent; long Colour; void* Key; };
struct Tree { Node Sentinel; Node* Root; Node* LastFound; };
static Node* mk(Node* n, Tree& t, void* key) { n->Left = &t.Sentinel; n->Right = &t.Sentinel; n->Parent = 0; n->Colour = 0; n->Key = key; return n; }
static unsigned long K[5] = { 10, 20, 30, 40, 50 };
static void build(Tree& t, Node* n) {   // a balanced tree over K: 30 / (20 (10)) (40 (50))
    for (int i = 0; i < 5; i++) mk(&n[i], t, &K[i]);
    t.Root = &n[2]; n[2].Left = &n[1]; n[1].Left = &n[0]; n[2].Right = &n[3]; n[3].Right = &n[4]; t.LastFound = 0; }

struct Comparator {
    int Equal(const void* a, const void* b) { return *(const unsigned long*)a == *(const unsigned long*)b; }
    int Less(const void* a, const void* b) { return *(const unsigned long*)a < *(const unsigned long*)b; }
    int EqualToKey(const void* key, const void* b) { return *(const unsigned long*)key == *(const unsigned long*)b; }
    int LessThanKey(const void* key, const void* b) { return *(const unsigned long*)key < *(const unsigned long*)b; } };

static void* Search(Tree& t, Comparator* cmp, const void* key) {
    Node* node = t.Root;
    while (node != &t.Sentinel) { if (cmp->EqualToKey(key, node->Key)) { t.LastFound = node; return node->Key; }
        if (cmp->LessThanKey(key, node->Key)) node = node->Left; else node = node->Right; }
    return 0; }

int main()
{
    Tree t; Node n[5]; build(t, n); Comparator c; unsigned long q50 = 50, q10 = 10, q35 = 35;
    if (!(Search(t, &c, &q50) == &K[4] && t.LastFound == &n[4] && Search(t, &c, &q10) == &K[0] && Search(t, &c, &q35) == 0 && t.LastFound == &n[0])) { std::printf("00c19010_TEST FAIL\n"); return 1; }
    std::printf("00c19010_TEST PASS\n");
    return 0;
}