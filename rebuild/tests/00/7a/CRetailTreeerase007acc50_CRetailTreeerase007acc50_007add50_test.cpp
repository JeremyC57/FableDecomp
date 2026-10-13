#include <cstdio>
#include <cstdlib>
static int g_e = 0; struct T { struct node { unsigned char c, p[3]; node* parent; node* l; node* r; }; node* h; unsigned long n; void Erase(node*) { ++g_e; }
  ~T() { if (n) { Erase(h->parent); h->l = h; h->parent = 0; h->r = h; n = 0; } if (h) std::free(h); } };
int main()
{
    { T t; t.h = (T::node*)std::malloc(sizeof(T::node)); t.h->parent = 0; t.n = 1; } if (g_e != 1) { std::printf("007add50_TEST FAIL\n"); return 1; }

    std::printf("007add50_TEST PASS\n");
    return 0;
}