#include <cstdio>
#include <cstdlib>
static int g_e = 0; struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Erase(node*) { ++g_e; } T() : h(0), n(0) {}
  ~T() { if (n) { Erase(h->parent); h->l = h; h->parent = 0; h->r = h; n = 0; } if (h) std::free(h); } };
static T g_t;
int main()
{
    g_t.h = (T::node*)std::malloc(sizeof(T::node)); g_t.h->parent = 0; g_t.n = 1; if (g_t.n != 1) { std::printf("012290c0_TEST FAIL\n"); return 1; }

    std::printf("012290c0_TEST PASS\n");
    return 0;
}
