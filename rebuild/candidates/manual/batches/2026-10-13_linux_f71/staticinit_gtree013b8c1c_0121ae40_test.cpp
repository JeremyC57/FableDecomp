#include <cstdio>
#include <cstdlib>
struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; T() { h = (node*)std::malloc(sizeof(node)); n = 0; h->c = 0; h->parent = 0; h->l = h; h->r = h; } ~T() { std::free(h); } };
T g_t;
int main()
{
    if (!g_t.h || g_t.n || g_t.h->l != g_t.h) { std::printf("0121ae40_TEST FAIL\n"); return 1; }

    std::printf("0121ae40_TEST PASS\n");
    return 0;
}
