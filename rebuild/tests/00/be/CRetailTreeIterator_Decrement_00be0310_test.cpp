#include <cstdio>
struct I { struct node { unsigned char f, p[3]; node* parent; node* l; node* r; }; node* c;
  I& Dec() { node* p = c; if (p->f == 0 && p->parent->parent == p) p = p->r; else if (p->l) { node* q = p->l; while (q->r) q = q->r; p = q; } else { node* q = p->parent; while (p == q->l) { p = q; q = q->parent; } p = q; } c = p; return *this; } };
int main()
{
    I::node a = {1,{0},0,0,0}, b = {1,{0},&a,0,0}; a.r = &b; I i; i.c = &b; i.Dec(); if (i.c != &a) { std::printf("00be0310_TEST FAIL\n"); return 1; }

    std::printf("00be0310_TEST PASS\n");
    return 0;
}