#include <cstdio>
struct T { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; long k; }; struct it { node* c; it(node* n) : c(n) {} }; struct pib { it f; bool s; pib(it a, bool b) : f(a), s(b) {} };
  node* h; unsigned long n; static node* Prev(node* x) { x = x->l; while (x->r) x = x->r; return x; } it Ins(node*, node* w, const long& v, node*) { return it(w); }
  pib insert(const long& v) { node* x = h->parent; node* y = h; bool a = true; while (x) { y = x; a = v < x->k; x = a ? x->l : x->r; } it w(y); if (!a) ; else if (w.c == h->l) return pib(Ins(y, y, v, 0), true); else w.c = Prev(w.c); if (w.c->k < v) return pib(Ins(x, y, v, 0), true); return pib(w, false); } };
int main()
{
    T::node h = {0,{0},0,0,0,0}, a = {1,{0},&h,0,0,5}; h.parent = &a; h.l = &a; T t; t.h = &h; t.n = 1; long k = 5, m = 7; if (t.insert(k).s || !t.insert(m).s) { std::printf("0042cee3_TEST FAIL\n"); return 1; }

    std::printf("0042cee3_TEST PASS\n");
    return 0;
}