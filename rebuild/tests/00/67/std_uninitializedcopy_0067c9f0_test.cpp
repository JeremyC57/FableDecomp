#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_baseCopies = 0, g_mapCopies = 0, g_attitudeCopies = 0;
struct CDefClassBase {
    long m_globalIndex;
    CDefClassBase() : m_globalIndex(0) { }
    CDefClassBase(const CDefClassBase& other) : m_globalIndex(other.m_globalIndex) { ++g_baseCopies; }
    virtual ~CDefClassBase() { }
    virtual int Kind() const { return 0; }
};
struct CSubDefInfoVector { void* m_begin; CSubDefInfoVector() : m_begin(0) { } CSubDefInfoVector(const CSubDefInfoVector& other) : m_begin(other.m_begin) { ++g_mapCopies; } };
struct CKeyPairCompareLess { };
struct CSubDefInfoMap { CSubDefInfoVector m_entries; CKeyPairCompareLess m_compare; unsigned char m_sorted; CSubDefInfoMap() : m_sorted(0) { } CSubDefInfoMap(const CSubDefInfoMap& other) : m_entries(other.m_entries), m_compare(other.m_compare), m_sorted(other.m_sorted) { } };
struct CParentDefClassBase : CDefClassBase {
    long InstantiationName; CSubDefInfoMap SubDefInfoMap;
    CParentDefClassBase() : InstantiationName(0) { }
    CParentDefClassBase(const CParentDefClassBase& other) : CDefClassBase(other), InstantiationName(other.InstantiationName), SubDefInfoMap(other.SubDefInfoMap) { }
    virtual int Kind() const { return 1; }
};
struct CAttitudeVector { void* m_begin; CAttitudeVector() : m_begin(0) { } CAttitudeVector(const CAttitudeVector& other) : m_begin(other.m_begin) { ++g_attitudeCopies; } };
struct CBlurredAttitudesDef : CParentDefClassBase {
    CAttitudeVector Attitudes;
    CBlurredAttitudesDef() { }
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other) : CParentDefClassBase(other), Attitudes(other.Attitudes) { }
    virtual int Kind() const { return 2; }
};
static char g_blocks[4][4];
typedef CBlurredAttitudesDef TElem;
static void Fill(TElem& e, int i)
{
    e.m_globalIndex = 90 + i; e.InstantiationName = 3 * i; e.SubDefInfoMap.m_entries.m_begin = g_blocks[i]; e.SubDefInfoMap.m_sorted = (unsigned char)i; e.Attitudes.m_begin = g_blocks[i] + 2;
}
static bool Check(const TElem& e, int i)
{
    return e.m_globalIndex == 90 + i && e.InstantiationName == 3 * i && e.SubDefInfoMap.m_entries.m_begin == g_blocks[i] && e.SubDefInfoMap.m_sorted == i && e.Attitudes.m_begin == g_blocks[i] + 2 && e.Kind() == 2;
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    if (g_baseCopies != 3 || g_mapCopies != 3 || g_attitudeCopies != 3) ok = false;
    if (!ok) { std::printf("0067c9f0_TEST FAIL\n"); return 1; }
    std::printf("0067c9f0_TEST PASS\n");
    return 0;
}