#include <cstdio>
struct BS { int v; bool operator<(const BS& o) { return v < o.v; } };
struct CS { BS* d; bool operator<(const CS& o) const { if (d == o.d) return false; if (d == 0) return true; if (o.d == 0) return false; return d->operator<(*o.d); } };
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; CS k; }; node* h; unsigned long n;
  node* find(const CS& k) { node* res = h; node* x = h->parent; while (x) { if (!(x->k < k)) { res = x; x = x->l; } else x = x->r; } if (res == h || k < res->k) return h; return res; } };
int main()
{
    BS b5 = { 5 }, b6 = { 6 }; T::node h = {0,{0},0,0,0,{0}}, a = {1,{0},&h,0,0,{&b5}}; h.parent = &a; T t; t.h = &h; t.n = 1; CS k5 = { &b5 }, k6 = { &b6 }; if (t.find(k5) != &a || t.find(k6) != &h) { std::printf("005e04b0_TEST FAIL\n"); return 1; }

    std::printf("005e04b0_TEST PASS\n");
    return 0;
}
