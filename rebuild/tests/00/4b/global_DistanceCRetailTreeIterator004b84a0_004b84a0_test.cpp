#include <cstdio>
struct I { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; }; node* c; I(node* n) : c(n) {}
  I& operator++() { node* p = c; if (p->r) { p = p->r; while (p->l) p = p->l; } else { node* q; while (p == (q = p->parent)->r) p = q; if (p->r != q) p = q; } c = p; return *this; }
  bool operator!=(const I& o) const { return c != o.c; } };
static long __cdecl D(const I& f, const I& l) { long n = 0; I i = f; while (i != l) { ++i; ++n; } return n; }
int main()
{
    I::node h = {0,{0},0,0,0}, a = {1,{0},&h,0,0}, b = {1,{0},&a,0,0}; h.parent = &a; h.l = &b; h.r = &a; a.l = &b; I f(&b), l(&h); if (D(f, l) != 2) { std::printf("004b84a0_TEST FAIL\n"); return 1; }

    std::printf("004b84a0_TEST PASS\n");
    return 0;
}