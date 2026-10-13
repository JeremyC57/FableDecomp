#include <cstdio>
static int g_c = 0;
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Cl() { ++g_c; } node* Cp(node* s, node* p) { s->parent = p; return s; }
  T& operator=(const T& o) { if (this != &o) { Cl(); n = 0; if (o.h->parent == 0) { h->parent = 0; h->l = h; h->r = h; } else { node* hh = h; hh->parent = Cp(o.h->parent, hh); node* x = h->parent; while (x->l) x = x->l; h->l = x; x = h->parent; while (x->r) x = x->r; h->r = x; n = o.n; } } return *this; } };
int main()
{
    T::node ha = {0,{0},0,0,0}, hb = {0,{0},0,0,0}, root = {0,{0},0,0,0}; T a, b; a.h = &ha; a.n = 0; b.h = &hb; hb.parent = &root; b.n = 1; a = b; if (g_c != 1 || ha.parent != &root || ha.l != &root || ha.r != &root || a.n != 1) { std::printf("004e4dc7_TEST FAIL\n"); return 1; }
    T e; T::node he = {0,{0},0,0,0}; e.h = &he; a = e; if (ha.parent || ha.l != &ha || a.n != 0) { std::printf("004e4dc7_TEST FAIL\n"); return 1; }

    std::printf("004e4dc7_TEST PASS\n");
    return 0;
}