#include <cstdio>
struct I { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; }; node* c;
  I& Inc() { node* p = c; if (p->r) { node* q = p->r; while (q->l) q = q->l; p = q; } else { node* q = p->parent; while (p == q->r) { p = q; q = q->parent; } if (p->r != q) p = q; } c = p; return *this; } };
int main()
{
    I::node a = {1,{0},0,0,0}, b = {1,{0},&a,0,0}; a.l = &b; I i; i.c = &b; i.Inc(); if (i.c != &a) { std::printf("009ee1f0_TEST FAIL\n"); return 1; }

    std::printf("009ee1f0_TEST PASS\n");
    return 0;
}