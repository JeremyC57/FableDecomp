#include <cstdio>
#include <cstdlib>
#include <cstring>
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; unsigned char v[4]; }; node* h; unsigned long n;
  node* B(const unsigned char (&v)[4]) { node* x = (node*)std::malloc(sizeof(node)); std::memcpy(x->v, v, 4); return x; }
  node* C(node* s, node* parent) { node* f = B(s->v); f->l = 0; f->r = 0; f->c = s->c; f->parent = parent; if (s->r) f->r = C(s->r, f); parent = f; for (node* q = s->l; q; q = q->l) { node* x = B(q->v); x->l = 0; x->r = 0; x->c = q->c; parent->l = x; x->parent = parent; if (q->r) x->r = C(q->r, x); parent = x; } return f; } };
int main()
{
    T::node a = {1,{0},0,0,0,{7,0,0,0}}, b = {0,{0},&a,0,0,{8,0,0,0}}, c = {1,{0},&a,0,0,{9,0,0,0}}; a.l = &b; a.r = &c; T t; T::node* d = t.C(&a, 0);
    if (!d || d->v[0] != 7 || !d->l || d->l->v[0] != 8 || d->l->parent != d || !d->r || d->r->v[0] != 9 || d->r->parent != d || d->c != 1) { std::printf("00461533_TEST FAIL\n"); return 1; }

    std::printf("00461533_TEST PASS\n");
    return 0;
}