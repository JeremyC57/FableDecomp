#include <cstdio>
static int g_r = 0; struct N { void* d; N* n; }; struct Li { N* h; ~Li() { for (N* n = h; n; ) { N* c = n; n = n->n; ::operator delete(c); } } }; struct X { unsigned char p[0x14]; Li l; void R() { ++g_r; } ~X() { R(); } };
static bool Check() { { X x; N* a = (N*)::operator new(sizeof(N)); N* b = (N*)::operator new(sizeof(N)); a->n = b; b->n = 0; x.l.h = a; } return g_r == 1; }

int main()
{
    if (!Check()) { std::printf("00cb1890_TEST FAIL\n"); return 1; }
    std::printf("00cb1890_TEST PASS\n");
    return 0;
}