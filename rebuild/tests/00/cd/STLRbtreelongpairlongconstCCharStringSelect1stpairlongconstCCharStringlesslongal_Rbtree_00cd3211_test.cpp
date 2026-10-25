// Behaviour test for 00cd3211: _Rb_tree<map<long,CCharString>>::~_Rb_tree -- clear() then the header node freed
#include <cstdio>
#include <cstdlib>
static int g_erased = 0; static int g_freed = 0;
struct Node { bool color; Node* parent; Node* left; Node* right; int value; };
static Node* NewNode(Node* parent) { Node* n = (Node*)std::malloc(sizeof(Node)); n->color = false; n->parent = parent; n->left = 0; n->right = 0; n->value = 0; return n; }
static void Free(void* p) { if (p) { ++g_freed; std::free(p); } }
struct Tree {
    Node* header; unsigned int count; int compare;
    Tree() { header = NewNode(0); header->color = false; header->parent = 0; header->left = header; header->right = header; count = 0; }
    void erase(Node* x) { while (x) { erase(x->right); Node* y = x->left; ++g_erased; Free(x); x = y; } }
    void clear() { if (count != 0) { erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~Tree() { clear(); Free(header); }
    void build3() { Node* r = NewNode(header); header->parent = r; r->left = NewNode(r); r->right = NewNode(r); header->left = r->left; header->right = r->right; count = 3; }
};

int main()
{
    { Tree t; t.build3(); }
    int ok = g_erased == 3 && g_freed == 4;
    { Tree e; }
    ok = ok && g_erased == 3 && g_freed == 5;
    std::printf(ok ? "00cd3211_TEST PASS\n" : "00cd3211_TEST FAIL\n");
    return ok ? 0 : 1;
}
