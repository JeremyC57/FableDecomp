#include <cstdio>
// Behaviour model: assigning a counted pointer releases the old block and add-refs the new one.
struct Info { long rc; };
struct CP { void* d; Info* i; };
static void assign(CP& a, const CP& b) { Info* ni = b.i; void* nd = b.d; if (a.i != ni) { if (a.i) --a.i->rc; a.d = nd; a.i = ni; if (ni) ++ni->rc; } }
int main()
{
    Info x = { 1 }, y = { 1 }; CP a = { 0, &x }, b = { 0, &y };
    assign(a, b);
    if (x.rc != 0 || y.rc != 2 || a.i != &y) { std::printf("008abd10_TEST FAIL\n"); return 1; }
    assign(a, b);
    if (y.rc != 2) { std::printf("008abd10_TEST FAIL\n"); return 1; }
    std::printf("008abd10_TEST PASS\n");
    return 0;
}