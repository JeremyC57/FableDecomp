// behaviour test for 00bf2e00 (standalone copy of the model)
#include <cstdio>

struct N { N* Tab[4]; unsigned char IsLeaf, KeyOffset, KeyBitStart, KeyBitCount; void* Entry; N* PrevSibling; N* NextSibling; N* ChildListHead; N* ChildListTail; N* Parent; };
static int g_visits = 0;
struct F { void CheckStructure(N* node, N* parent) {
    while (node) { ++g_visits;
        if (!node->IsLeaf) { N* child = node->ChildListHead; N** slot = node->Tab; while (child) { if (*slot) child = child->NextSibling; slot++; } CheckStructure(node->ChildListHead, node); }
        node = node->NextSibling; } } };

int main()
{
    N l1 = N(), l2 = N(), root = N(); l1.IsLeaf = 1; l2.IsLeaf = 1; l1.NextSibling = &l2;
    root.ChildListHead = &l1; root.Tab[0] = &l1; root.Tab[1] = 0; root.Tab[2] = &l2; root.Tab[3] = 0;
    F f; f.CheckStructure(&root, 0);
    if (!(g_visits == 3)) { std::printf("00bf2e00_TEST FAIL\n"); return 1; }
    std::printf("00bf2e00_TEST PASS\n");
    return 0;
}