// Behaviour test for 0x008137E0 CTCCreatureGroupTag::FindNewTagger (model copy over a hand-built member tree).
#include <cstdio>
#include <cstdlib>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Data { int vptr_slot; void* PData; bool IsActive; unsigned int JoinedFrame; unsigned int DeactivatedFrame; };
struct Node : NodeBase { unsigned long long key; Data value; };
static void increment(NodeBase*& n)
{
    if (n->right != 0) { n = n->right; while (n->left != 0) n = n->left; }
    else { NodeBase* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
}
struct Tree { NodeBase* header; unsigned int count; };
// header + 3-node tree: 5 (root) with children 3 and 8
static NodeBase g_header; static Node g_n3, g_n5, g_n8;
static void build(Tree& t)
{
    g_header.parent = &g_n5; g_header.left = &g_n3; g_header.right = &g_n8; g_header.color = false;
    g_n5.parent = &g_header; g_n5.left = &g_n3; g_n5.right = &g_n8; g_n5.key = 5;
    g_n3.parent = &g_n5; g_n3.left = 0; g_n3.right = 0; g_n3.key = 3;
    g_n8.parent = &g_n5; g_n8.left = 0; g_n8.right = 0; g_n8.key = 8;
    g_n3.value.IsActive = false; g_n5.value.IsActive = false; g_n8.value.IsActive = false;
    t.header = &g_header; t.count = 3;
}
struct Tag { Tree Members; unsigned long long TaggerUID; };
static bool find_new_tagger(Tag& g)
{
    for (NodeBase* it = g.Members.header->left; it != g.Members.header; increment(it))
        if (((Node*)it)->value.IsActive) { g.TaggerUID = ((Node*)it)->key; return true; }
    g.TaggerUID = 0;
    return false;
}
int main()
{
    Tag g; int ok = 1;
    build(g.Members);
    g.TaggerUID = 77;
    if (find_new_tagger(g) || g.TaggerUID != 0) ok = 0;   // none active
    g_n8.value.IsActive = true; g_n8.key = 0x500000008ULL;
    if (!find_new_tagger(g) || g.TaggerUID != 0x500000008ULL) ok = 0;
    g_n5.value.IsActive = true;
    if (!find_new_tagger(g) || g.TaggerUID != 5) ok = 0;  // first active in key order
    g_n3.value.IsActive = true;
    if (!find_new_tagger(g) || g.TaggerUID != 3) ok = 0;
    if (!ok) { std::printf("008137e0_TEST FAIL\n"); return 1; }
    std::printf("008137e0_TEST PASS\n");
    return 0;
}