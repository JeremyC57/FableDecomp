// Behaviour test for 0x006BA420 CFaction::CFaction(): empty name, two empty sets with self-linked red header nodes.
#include <cstdio>
#include <cstdlib>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { void* str; };
struct Set {
    Node* header; unsigned int count; unsigned char comp;
    Set() : header(0), count(0)
    {
        header = (Node*)std::malloc(sizeof(Node));
        header->color = false; header->parent = 0; header->left = header; header->right = header;
    }
};
struct CFaction { void* Name; Set AlliedFactions; Set EnemyFactions; CFaction() : Name(0) {} };
int main()
{
    CFaction f;
    if (f.Name != 0 || f.AlliedFactions.count != 0 || f.EnemyFactions.count != 0) { std::printf("006ba420_TEST FAIL\n"); return 1; }
    Set* sets[2] = { &f.AlliedFactions, &f.EnemyFactions };
    for (int i = 0; i < 2; ++i) {
        Node* h = sets[i]->header;
        if (h == 0 || h->color != false || h->parent != 0 || h->left != h || h->right != h) { std::printf("006ba420_TEST FAIL\n"); return 1; }
    }
    if (f.AlliedFactions.header == f.EnemyFactions.header) { std::printf("006ba420_TEST FAIL\n"); return 1; }
    std::printf("006ba420_TEST PASS\n");
    return 0;
}