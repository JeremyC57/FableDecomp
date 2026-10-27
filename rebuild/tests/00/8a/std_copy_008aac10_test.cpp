// behaviour test for 0x008AAC10 std::__copy over CCharString (model: an element type that counts its assignments)
#include <cstdio>
static int g_assign = 0;
struct E { int v; E& operator=(const E& o) { v = o.v; ++g_assign; return *this; } };
template <class I, class O> O Copy(I first, I last, O result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    E a[3] = { { 1 }, { 2 }, { 3 } }, b[3] = { { 0 }, { 0 }, { 0 } };
    E* end = Copy(a, a + 3, b);
    int ok = end == b + 3 && b[0].v == 1 && b[2].v == 3 && g_assign == 3;
    ok = ok && Copy(a, a, b) == b && g_assign == 3;
    if (!ok) { std::printf("008aac10_TEST FAIL\n"); return 1; }
    std::printf("008aac10_TEST PASS\n");
    return 0;
}