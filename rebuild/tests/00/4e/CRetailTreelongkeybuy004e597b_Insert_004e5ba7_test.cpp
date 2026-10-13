#include <cstdio>
#include <cstdlib>
struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; long k; }; struct it { node* c; it(node* n) : c(n) {} }; node* h; unsigned long n;
  node* Buy(const long& v) { node* z = (node*)std::malloc(sizeof(node)); z->k = v; return z; }
  it Ins(node* x, node* w, const long& v, long fr) { node* z; if (w == h || (!fr && (x != 0 || v < w->k))) { z = Buy(v); w->l = z; if (w == h) { h->parent = z; h->r = z; } else if (w == h->l) h->l = z; } else { z = Buy(v); w->r = z; if (w == h->r) h->r = z; } z->l = 0; z->r = 0; z->parent = w; ++n; return it(z); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}; T t; t.h = &h; t.n = 0; long v = 3; T::it i = t.Ins(0, &h, v, 0); if (h.parent != i.c || h.r != i.c || t.n != 1 || i.c->k != 3) { std::printf("004e5ba7_TEST FAIL\n"); return 1; }
    std::free(i.c);

    std::printf("004e5ba7_TEST PASS\n");
    return 0;
}