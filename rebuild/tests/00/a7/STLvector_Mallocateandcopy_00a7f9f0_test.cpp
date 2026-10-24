// Behaviour test for 0x00A7F9F0 vector<CNavigationPosition>::_M_allocate_and_copy: a block of n positions (null for n = 0)
// whose first (last - first) entries are copies of the source range.
#include <cstdio>
#include <cstdlib>
#include <new>
struct CNavigationPosition { float X, Y; int Layer; };
static CNavigationPosition* AllocateAndCopy(unsigned int n, const CNavigationPosition* first, const CNavigationPosition* last)
{
    CNavigationPosition* result = n != 0 ? (CNavigationPosition*)std::malloc(n * sizeof(CNavigationPosition)) : 0;
    CNavigationPosition* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) new (cur) CNavigationPosition(*first);
    return result;
}
int main()
{
    CNavigationPosition src[3] = { { 1.0f, 2.0f, 0 }, { 3.0f, 4.0f, 1 }, { 5.0f, 6.0f, 2 } };
    CNavigationPosition* b = AllocateAndCopy(5, src, src + 3);
    if (!b || b[0].X != 1.0f || b[1].Y != 4.0f || b[2].Layer != 2) { std::printf("00a7f9f0_TEST FAIL\n"); return 1; }
    std::free(b);
    if (AllocateAndCopy(0, src, src) != 0) { std::printf("00a7f9f0_TEST FAIL\n"); return 1; }
    std::printf("00a7f9f0_TEST PASS\n");
    return 0;
}