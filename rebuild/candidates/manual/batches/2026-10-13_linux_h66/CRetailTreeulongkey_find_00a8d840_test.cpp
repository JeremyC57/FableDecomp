#include <cstdio>
struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; unsigned long k; }; struct it { node* c; it(node* n) : c(n) {} }; node* h; unsigned long n;
  it find(const unsigned long& k) { node* res = h; node* x = h->parent; while (x) { if (!(x->k < k)) { res = x; x = x->l; } else x = x->r; } node* f = res; if (f == h || k < f->k) f = h; return it(f); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}, a = {1,{0},&h,0,0,5}; h.parent = &a; T t; t.h = &h; t.n = 1; unsigned long k = 5, m = 6; if (t.find(k).c != &a || t.find(m).c != &h) { std::printf("00a8d840_TEST FAIL\n"); return 1; }

    std::printf("00a8d840_TEST PASS\n");
    return 0;
}
