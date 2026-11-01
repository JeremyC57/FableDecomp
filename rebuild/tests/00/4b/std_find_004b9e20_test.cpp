// Behaviour test for std::find<CCharString*, CCharString> 0x004B9E20: forwards to the random-access __find with the tag
#include <cstdio>
#include <cstring>
class CCharStringData { public: const char* Chars; unsigned long Length; };
class CCharString { public: CCharStringData* PStringData; };
static bool Equal(const CCharString& a, const CCharString& b)
{
    if (a.PStringData == b.PStringData) return true;
    if (a.PStringData && b.PStringData && a.PStringData->Length == b.PStringData->Length)
        return std::strcmp(a.PStringData->Chars, b.PStringData->Chars) == 0;
    return false;
}
struct random_access_iterator_tag { };
static int g_tagCalls = 0;
static CCharString* __find(CCharString* first, CCharString* last, const CCharString& val, const random_access_iterator_tag&)
{
    ++g_tagCalls;
    for (; first != last; ++first)
        if (Equal(*first, val)) return first;
    return last;
}
static CCharString* find(CCharString* first, CCharString* last, const CCharString& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}
int main()
{
    CCharStringData d0 = { "Intro", 5 }, d1 = { "Combat", 6 }, d2 = { "Magic", 5 }, q = { "Combat", 6 }, n = { "Nope", 4 };
    CCharString v[3] = { { &d0 }, { &d1 }, { &d2 } };
    CCharString key = { &q }, miss = { &n };
    if (find(v, v + 3, key) != v + 1) { std::printf("004b9e20_TEST FAIL\n"); return 1; }
    if (find(v, v + 3, miss) != v + 3) { std::printf("004b9e20_TEST FAIL\n"); return 1; }
    if (find(v, v, key) != v || g_tagCalls != 3) { std::printf("004b9e20_TEST FAIL\n"); return 1; }
    std::printf("004b9e20_TEST PASS\n");
    return 0;
}