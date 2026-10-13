#include <cstdio>
static int g_e = 0;
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Erase(node*) { ++g_e; } node* Cp(node* s, node* p) { s->parent = p; return s; }
  void Cl() { if (n) { Erase(h->parent); h->l = h; h->parent = 0; h->r = h; n = 0; } }
  T& operator=(const T& o) { if (this != &o) { Cl(); n = 0; if (o.h->parent == 0) { h->parent = 0; h->l = h; h->r = h; } else { node* hh = h; hh->parent = Cp(o.h->parent, hh); node* x = h->parent; while (x->l) x = x->l; h->l = x; x = h->parent; while (x->r) x = x->r; h->r = x; n = o.n; } } return *this; } };
int main()
{
    T::node ha = {0,{0},0,0,0}, hb = {0,{0},0,0,0}, root = {0,{0},0,0,0}; T a, b; a.h = &ha; a.n = 1; ha.parent = &root; b.h = &hb; hb.parent = &root; b.n = 1; a = b; if (g_e != 1 || ha.parent != &root || a.n != 1) { std::printf("00884770_TEST FAIL\n"); return 1; }

    std::printf("00884770_TEST PASS\n");
    return 0;
}
