// Behaviour test for 0x0047C6A0 _Rb_tree<..., vector<CHeroLogBookEntry> >::_M_erase (model over a small tree).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_entryDtors = 0, g_frees = 0;
struct Entry { int id; ~Entry() { ++g_entryDtors; } };
struct Vec {
    Entry* start; Entry* finish; Entry* eos;
    ~Vec() { for (Entry* p = start; p != finish; ++p) p->~Entry(); if (start) { std::free(start); ++g_frees; } }
};
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { long key; Vec value; };

static void Erase(Node* x)
{
    while (x != 0) {
        Erase((Node*)x->right);
        Node* y = (Node*)x->left;
        x->value.~Vec();
        std::free(x); ++g_frees;
        x = y;
    }
}

static Node* Make(int n)
{
    Node* node = (Node*)std::malloc(sizeof(Node));
    node->left = node->right = 0;
    node->value.start = n ? (Entry*)std::malloc(n * sizeof(Entry)) : 0;
    for (int i = 0; i < n; ++i) new (node->value.start + i) Entry();
    node->value.finish = node->value.eos = node->value.start + n;
    return node;
}

int main()
{
    Node* root = Make(2);
    root->left = Make(1);
    root->right = Make(0);
    root->right->right = Make(3);
    Erase(root);
    // 4 nodes freed + 3 non-empty vectors freed; 6 entries destroyed
    if (g_entryDtors != 6 || g_frees != 7) { std::printf("0047c6a0_TEST FAIL\n"); return 1; }
    std::printf("0047c6a0_TEST PASS\n");
    return 0;
}