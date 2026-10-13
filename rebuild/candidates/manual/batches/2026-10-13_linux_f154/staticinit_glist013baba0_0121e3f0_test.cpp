#include <cstdio>
#include <cstdlib>
struct L { struct node { node* n; node* p; char v[12]; }; node* h; L() { h = (node*)std::malloc(sizeof(node)); h->n = h; h->p = h; } ~L() { std::free(h); } };
L g_l;
int main()
{
    if (!g_l.h || g_l.h->n != g_l.h) { std::printf("0121e3f0_TEST FAIL\n"); return 1; }

    std::printf("0121e3f0_TEST PASS\n");
    return 0;
}
