#include <cstdio>
// Behaviour model of STLport _Rb_tree::insert_unique's search (unbalanced BST stand-in for the out-of-line _M_insert):
// descend to the leaf, step back once unless at begin(), insert only when the key is new.
struct N { N* l; N* r; N* p; unsigned long long k; };
static N pool[16]; static int used;
struct Tree {
    N* root; int count;
    N* minimum() { N* x = root; if (!x) return 0; while (x->l) x = x->l; return x; }
    N* prev(N* x) { if (x->l) { x = x->l; while (x->r) x = x->r; return x; } N* y = x->p; while (y && x == y->l) { x = y; y = y->p; } return y; }
    N* make(N* parent, bool left, unsigned long long k) { N* n = &pool[used++]; n->l = n->r = 0; n->p = parent; n->k = k;
        if (!parent) root = n; else if (left) parent->l = n; else parent->r = n; ++count; return n; }
    bool insert_unique(unsigned long long k, N** where)
    {
        N* y = 0; N* x = root; bool comp = true;
        while (x) { y = x; comp = k < x->k; x = comp ? x->l : x->r; }
        if (!y) { *where = make(0, true, k); return true; }
        N* j = y;
        if (comp) { if (j == minimum()) { *where = make(y, true, k); return true; } j = prev(j); }
        if (j->k < k) { *where = make(y, comp, k); return true; }
        *where = j; return false;
    }
};
int main()
{
    Tree t; t.root = 0; t.count = 0; used = 0; N* w;
    unsigned long long ks[6] = { 0x500000000ULL, 0x300000000ULL, 0x700000000ULL, 0x300000001ULL, 0x2FFFFFFFFULL, 0x700000000ULL };
    bool expect[6] = { true, true, true, true, true, false };
    for (int i = 0; i < 6; ++i) {
        bool r = t.insert_unique(ks[i], &w);
        if (r != expect[i] || w->k != ks[i]) { std::printf("006f1ef0_TEST FAIL\n"); return 1; }
    }
    if (t.count != 5 || t.insert_unique(0x300000000ULL, &w) || w->k != 0x300000000ULL) { std::printf("006f1ef0_TEST FAIL\n"); return 1; }
    std::printf("006f1ef0_TEST PASS\n");
    return 0;
}