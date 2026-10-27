// 005e1f90 behaviour test for std::__copy_aux<pair<unsigned long, CAnimationEntry>*, ...>(..., const __false_type&)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
static int g_copyCalls = 0;
struct random_access_iterator_tag { };
struct false_type { };
class CAnimationEntry { public: long BankIndex; long AnimName; long GroupName; void* Components; };
struct Entry { unsigned long first; CAnimationEntry second; };
template <class T> inline random_access_iterator_tag __fastcall iterator_category(const T*) { return random_access_iterator_tag(); }
template <class T> inline int* __fastcall distance_type(const T*) { return (int*)0; }
template <class RI, class OI, class D>
OI __fastcall Copy(RI first, RI last, OI result, const random_access_iterator_tag&, D*)   // stands in for __copy 0x005E18B0
{
    ++g_copyCalls;
    for (D n = (D)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class II, class OI>
OI __fastcall CopyAux(II first, II last, OI result, const false_type&)
{
    return Copy(first, last, result, iterator_category(first), distance_type(first));
}
static void run()
{
    Entry src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].first = 10 + i; src[i].second.BankIndex = i; src[i].second.AnimName = 100 + i; src[i].second.GroupName = 7; src[i].second.Components = 0; }
    for (int j = 0; j < 4; ++j) { dst[j].first = 0; dst[j].second.BankIndex = -1; }
    Entry* end = CopyAux(src, src + 3, dst, false_type());
    CHECK(end == dst + 3);
    CHECK(g_copyCalls == 1);
    CHECK(dst[0].first == 10 && dst[2].first == 12 && dst[2].second.AnimName == 102);
    CHECK(dst[3].second.BankIndex == -1);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("005e1f90_TEST FAIL\n");
        return 1;
    }
    std::printf("005e1f90_TEST PASS\n");
    return 0;
}