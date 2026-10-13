#include <cstdio>
#include <new>
#include <cstdlib>
struct L { struct N { unsigned char l[8]; long v; }; N* B(const long& v) { N* n = (N*)std::malloc(sizeof(N)); new (&n->v) long(v); return n; } };
int main()
{
    L l; long x = 5; L::N* n = l.B(x); if (!n || n->v != 5) { std::printf("0042a008_TEST FAIL\n"); return 1; }
    std::free(n);

    std::printf("0042a008_TEST PASS\n");
    return 0;
}