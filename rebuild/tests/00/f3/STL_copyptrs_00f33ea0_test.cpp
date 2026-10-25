#include <cstdio>
// Behaviour test for 00f33ea0: __copy_ptrs assigns [first, last) front to back onto result (an overlapping move down is
// safe) and returns result + (last - first).
static int g_assigns = 0;
struct Def { int base; int pad; Def& operator=(const Def& o) { base = o.base; pad = o.pad; ++g_assigns; return *this; } };
static Def* copy_ptrs(Def* first, Def* last, Def* result)
{
    for (long n = (long)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Def a[6];
    for (int i = 0; i < 6; ++i) { a[i].base = i; a[i].pad = 10 * i; }
    Def* r = copy_ptrs(a + 2, a + 6, a);         // shift down by two
    int ok = r == a + 4 && a[0].base == 2 && a[1].base == 3 && a[2].base == 4 && a[3].base == 5 && a[3].pad == 50 && g_assigns == 4;
    Def* e = copy_ptrs(a + 1, a + 1, a);
    ok = ok && e == a && g_assigns == 4;
    if (!ok) { std::printf("00f33ea0_TEST FAIL\n"); return 1; }
    std::printf("00f33ea0_TEST PASS\n");
    return 0;
}