#include <cstdio>
#include <new>
// Behaviour model: n copies, each bumping the shared count.
struct Info { unsigned long rc; };
struct CP { void* d; Info* i; CP(void* dd, Info* ii) : d(dd), i(ii) { } CP(const CP& o) : d(o.d), i(o.i) { if (i) ++i->rc; } };
static CP* fill(CP* f, unsigned int n, const CP& x) { for (; n > 0; --n, ++f) new ((void*)f) CP(x); return f; }
int main()
{
    Info inf = { 1 }; CP x(0, &inf); static void* raw[8]; CP* d = (CP*)raw;
    if (fill(d, 3, x) != d + 3 || inf.rc != 4) { std::printf("00b82dc0_TEST FAIL\n"); return 1; }
    std::printf("00b82dc0_TEST PASS\n");
    return 0;
}