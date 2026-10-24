// Behaviour test for 0x00A82AC0 _STL::__copy_ptrs over list<C2DLineF>: each list of [first, last) is assigned (list
// operator=) onto the matching existing list at result, the advanced result is returned, an empty range does nothing.
#include <cstdio>
static int g_assigns = 0;
struct SList { int head; SList& operator=(const SList& o) { head = o.head; ++g_assigns; return *this; } };
// the function under test (same shape as the source)
static SList* CopyPtrs(const SList* first, const SList* last, SList* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    SList src[3] = { { 7 }, { 8 }, { 9 } };
    SList dst[4] = { { 0 }, { 0 }, { 0 }, { 42 } };
    if (CopyPtrs(src, src + 3, dst) != dst + 3 || g_assigns != 3) { std::printf("00a82ac0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (dst[i].head != 7 + i) { std::printf("00a82ac0_TEST FAIL\n"); return 1; }
    if (dst[3].head != 42 || CopyPtrs(src, src, dst + 3) != dst + 3 || g_assigns != 3) { std::printf("00a82ac0_TEST FAIL\n"); return 1; }
    std::printf("00a82ac0_TEST PASS\n");
    return 0;
}