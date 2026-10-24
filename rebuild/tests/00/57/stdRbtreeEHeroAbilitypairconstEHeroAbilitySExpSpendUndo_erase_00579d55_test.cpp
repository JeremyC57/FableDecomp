// Behaviour test for 0x00579D55 std::_Rb_tree<EHeroAbility, pair<const EHeroAbility, SExpSpendUndo> >::erase(iterator):
// the node handed back by _Rebalance_for_erase has its SExpSpendUndo destroyed once and is freed, the count drops.
// (_Rebalance_for_erase is modelled for leaf nodes only: unlink from the parent, fix leftmost/rightmost.)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_undoDtors = 0, g_frees = 0;
struct SExpSpendUndo { int Levels; ~SExpSpendUndo() { ++g_undoDtors; } };
struct Base { bool colour; Base* parent; Base* left; Base* right; };
struct Node : Base { int key; SExpSpendUndo value; };

static Base* RebalanceForErase(Base* z, Base*& root, Base*& leftmost, Base*& rightmost)
{
    Base* p = z->parent;
    if (root == z) root = 0;
    else if (p->left == z) p->left = 0;
    else p->right = 0;
    if (leftmost == z) leftmost = (root == 0) ? 0 : p;
    if (rightmost == z) rightmost = (root == 0) ? 0 : p;
    return z;
}

struct Tree {
    Base* header; unsigned int count;
    void erase(Base* pos)
    {
        Node* y = (Node*)RebalanceForErase(pos, header->parent, header->left, header->right);
        y->value.~SExpSpendUndo();
        if (y != 0) { ++g_frees; std::free(y); }
        --count;
    }
};

static Node* Make(int key)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->left = n->right = n->parent = 0; n->key = key;
    new (&n->value) SExpSpendUndo(); n->value.Levels = key;
    return n;
}

int main()
{
    Base header; Tree t; t.header = &header; t.count = 3;
    Node* root = Make(2); Node* l = Make(1); Node* r = Make(3);
    root->left = l; root->right = r; l->parent = root; r->parent = root; root->parent = &header;
    header.parent = root; header.left = l; header.right = r;
    t.erase(l);
    bool ok = t.count == 2 && root->left == 0 && header.left == root && g_undoDtors == 1 && g_frees == 1;
    t.erase(r);
    ok = ok && t.count == 1 && header.right == root && g_undoDtors == 2 && g_frees == 2;
    t.erase(root);
    ok = ok && t.count == 0 && header.parent == 0 && g_undoDtors == 3 && g_frees == 3;
    std::printf(ok ? "00579d55_TEST PASS\n" : "00579d55_TEST FAIL\n");
    return ok ? 0 : 1;
}