#include <cstdio>
#include <cstdlib>
struct L { struct node { node* n; node* p; char v[4]; }; node* h; void Init() { h = 0; node* s = (node*)std::malloc(sizeof(node)); s->n = s; s->p = s; h = s; } };
int main()
{
    L l; l.Init(); if (!l.h || l.h->n != l.h) { std::printf("006f2bb0_TEST FAIL\n"); return 1; }
    std::free(l.h);

    std::printf("006f2bb0_TEST PASS\n");
    return 0;
}