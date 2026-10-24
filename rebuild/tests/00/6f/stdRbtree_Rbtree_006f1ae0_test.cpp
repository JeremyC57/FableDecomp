// Behaviour test for 0x006F1AE0 (STLport _Rb_tree(comp, a) for map<unsigned __int64, CCombatantInfo>: an empty red sentinel).
#include <cstdio>
#include <cstdlib>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { unsigned long long key; unsigned char value[0x70]; };
static unsigned int g_lastSize = 0;
static void* Alloc(unsigned int n) { g_lastSize = n; return std::malloc(n); }
struct Tree {
    Node* header; unsigned int node_count;
    Tree() : header(0), node_count(0xFFFFFFFFu)
    {
        header = (Node*)Alloc(sizeof(Node));
        node_count = 0;
        header->color = false; header->parent = 0; header->left = header; header->right = header;
    }
    bool empty() const { return node_count == 0 && header->left == header && header->right == header; }
};
#define FAIL { std::printf("006f1ae0_TEST FAIL\n"); return 1; }
int main()
{
    Tree t;
    if (g_lastSize != (unsigned int)sizeof(Node) || t.header == 0) FAIL;
    if (t.header->color != false || t.header->parent != 0 || !t.empty()) FAIL;
    std::free(t.header);
    std::printf("006f1ae0_TEST PASS\n");
    return 0;
}