// Behaviour test for 0x00A82000 _STL::__copy over (unsigned __int64 ID, list) entries: every entry of [first, last)
// is assigned forward into result (key copied, the list assigned through its operator=), the advanced result is
// returned, an empty range returns result untouched; overlapping forward copies read before they overwrite.
#include <cstdio>
static int g_assigns = 0;
struct SList { int head; SList& operator=(const SList& o) { head = o.head; ++g_assigns; return *this; } };
struct SEntry { unsigned long long first; SList second; };
// the function under test (same shape as the source)
static SEntry* Copy(SEntry* first, SEntry* last, SEntry* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    SEntry v[6];
    for (int i = 0; i < 6; ++i) { v[i].first = 0x100000000ULL * i + 7; v[i].second.head = 100 + i; }
    SEntry* r = Copy(v + 2, v + 5, v);              // shift left by two (forward copy is safe)
    if (r != v + 3 || g_assigns != 3) { std::printf("00a82000_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (v[i].first != 0x100000000ULL * (i + 2) + 7 || v[i].second.head != 102 + i) { std::printf("00a82000_TEST FAIL\n"); return 1; }
    if (Copy(v, v, v + 4) != v + 4 || g_assigns != 3 || v[4].second.head != 104) { std::printf("00a82000_TEST FAIL\n"); return 1; }
    std::printf("00a82000_TEST PASS\n");
    return 0;
}