#include <cstdio>
#include <cstdlib>
static int g_e = 0, g_f = 0;
struct N { long c; N* p; N* l; N* r; };
struct Tr { N* h; long n; void E(N*) { ++g_e; } void C() { if (n != 0) { E(h->p); h->l = h; h->p = 0; h->r = h; n = 0; } } ~Tr() { C(); if (h) { ++g_f; std::free(h); } } };
static void Run() { Tr t; t.h = (N*)std::malloc(sizeof(N)); t.h->p = t.h; t.n = 1; }
int main()
{
    Run(); if (g_e != 1 || g_f != 1) { std::printf("0122a100_TEST FAIL\n"); return 1; }

    std::printf("0122a100_TEST PASS\n");
    return 0;
}