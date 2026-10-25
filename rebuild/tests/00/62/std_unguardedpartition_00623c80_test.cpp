// Behaviour test for 0x00623C80: std::__unguarded_partition(CTestQuest* first, CTestQuest* last, CTestQuest pivot, CTestQuestNameCompare comp) -- STLport quicksort partition for the test-quest sort: walk both ends towards the pivot by quest name, swap the out-of-place pair, return the split point
// A copy of the source model and logic (compiled separately from src/00623c80.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class CTestQuest {
public:
    CCharString QuestName;                         // +0x00
    CCharString HSPName;                           // +0x04
    long        ColourIndex;                       // +0x08
    CCharString DisplayName;                       // +0x0c
    CCharString IniFile;                           // +0x10
    CCharString OutiFile;                          // +0x14
    CCharString QuestCardDefName;                  // +0x18
};

class CTestQuestNameCompare {
public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

namespace std {
template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template CTestQuest* __fastcall std::__unguarded_partition<CTestQuest*, CTestQuest, CTestQuestNameCompare>(CTestQuest*, CTestQuest*, CTestQuest, CTestQuestNameCompare);

// ---- test stand-ins ----
static int g_live = 0;
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
bool CTestQuestNameCompare::operator()(const CTestQuest& a, const CTestQuest& b) const
{
    return std::strcmp((const char*)a.QuestName.m_pData, (const char*)b.QuestName.m_pData) < 0;
}
static char g_names[64][8];
static void Set(CTestQuest& q, int k)
{
    std::sprintf(g_names[k], "Q%02d", k);
    q.QuestName.m_pData = g_names[k]; q.HSPName.m_pData = g_names[k] + 1; q.ColourIndex = k * 3;
    q.DisplayName.m_pData = g_names[k] + 2; q.IniFile.m_pData = 0; q.OutiFile.m_pData = 0; q.QuestCardDefName.m_pData = g_names[k] + 1;
}
static int Key(const CTestQuest& q) { return ((const char*)q.QuestName.m_pData)[1] * 10 + ((const char*)q.QuestName.m_pData)[2] - '0' * 11; }
static bool Intact(const CTestQuest& q) { return q.ColourIndex == Key(q) * 3 && q.HSPName.m_pData == (char*)q.QuestName.m_pData + 1 && q.DisplayName.m_pData == (char*)q.QuestName.m_pData + 2; }
static void Fill(CTestQuest* p, int n, unsigned int seed)
{
    for (int i = 0; i < n; ++i) {
        seed = seed * 1103515245u + 12345u;
        Set(p[i], (int)((seed >> 9) % 60u));
    }
}

static int Fail() { std::printf("00623c80_TEST FAIL\n"); return 1; }

int main()
{
    const int n = 40;
    CTestQuest* p = (CTestQuest*)std::malloc(n * sizeof(CTestQuest));
    for (unsigned int seed = 1; seed < 30; ++seed) {
        Fill(p, n, seed);
        int sum = 0, before[64] = { 0 }, after[64] = { 0 };
        for (int i = 0; i < n; ++i) { sum += Key(p[i]); ++before[Key(p[i])]; }
        int pivotIndex = (int)(seed % (unsigned int)n);
        int liveBefore = g_live;
        CTestQuest* cut;
        {
            CTestQuest pivot(p[pivotIndex]);
            cut = std::__unguarded_partition(p, p + n, pivot, CTestQuestNameCompare());
            for (CTestQuest* q = p; q != cut; ++q)
                if (CTestQuestNameCompare()(pivot, *q)) return Fail();
            for (CTestQuest* r = cut; r != p + n; ++r)
                if (CTestQuestNameCompare()(*r, pivot)) return Fail();
        }
        if (cut < p || cut > p + n || g_live != liveBefore) return Fail();
        for (int i = 0; i < n; ++i) { if (!Intact(p[i])) return Fail(); ++after[Key(p[i])]; }
        for (int k = 0; k < 64; ++k) if (before[k] != after[k]) return Fail();
    }
    std::free(p);
    std::printf("00623c80_TEST PASS\n");
    return 0;
}