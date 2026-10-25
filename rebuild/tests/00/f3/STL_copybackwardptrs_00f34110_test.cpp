#include <cstdio>
// Behaviour test for 00f34110: __copy_backward_ptrs assigns [first, last) back to front so that it ends at result (an
// overlapping move up is safe) and returns result - (last - first).
static int g_assigns = 0;
struct Def { int base; int pad; Def& operator=(const Def& o) { base = o.base; pad = o.pad; ++g_assigns; return *this; } };
static Def* copy_backward_ptrs(Def* first, Def* last, Def* result)
{
    for (long n = (long)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
int main()
{
    Def a[6];
    for (int i = 0; i < 6; ++i) { a[i].base = i; a[i].pad = 10 * i; }
    Def* r = copy_backward_ptrs(a, a + 4, a + 6);   // shift up by two
    int ok = r == a + 2 && a[2].base == 0 && a[3].base == 1 && a[4].base == 2 && a[5].base == 3 && a[5].pad == 30 && g_assigns == 4;
    Def* e = copy_backward_ptrs(a + 3, a + 3, a + 5);
    ok = ok && e == a + 5 && g_assigns == 4;
    if (!ok) { std::printf("00f34110_TEST FAIL\n"); return 1; }
    std::printf("00f34110_TEST PASS\n");
    return 0;
}