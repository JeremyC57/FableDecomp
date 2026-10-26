// Behaviour test for vector<CCharString>::_M_allocate_and_copy (0x00432FF0): n == 0 gives a null block, otherwise a block of n
// strings whose first (last - first) are copy-constructed (shared data, one copy-constructor call each) from the source range.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
class CCharStringData { public: const char* text; int refs; };
class CCharString {
public:
    CCharString(CCharStringData* d) : PStringData(d) { ++d->refs; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++PStringData->refs; ++g_copies; }
    CCharStringData* PStringData;
};
CCharString* allocate_and_copy(size_t n, const CCharString* first, const CCharString* last)
{
    CCharString* result = n != 0 ? (CCharString*)std::malloc(n * sizeof(CCharString)) : 0;
    CCharString* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CCharString(*first);
    return result;
}
int main()
{
    int bad = 0;
    CCharStringData a = { "alpha", 0 }, b = { "beta", 0 };
    CCharString src[2] = { CCharString(&a), CCharString(&b) };
    if (allocate_and_copy(0, src, src) != 0 || g_copies != 0) bad = 1;
    CCharString* block = allocate_and_copy(4, src, src + 2);
    if (!block || block[0].PStringData != &a || block[1].PStringData != &b || g_copies != 2) bad = 1;
    if (a.refs != 2 || b.refs != 2) bad = 1;
    std::free(block);
    if (bad) { std::printf("00432ff0_TEST FAIL\n"); return 1; }
    std::printf("00432ff0_TEST PASS\n");
    return 0;
}