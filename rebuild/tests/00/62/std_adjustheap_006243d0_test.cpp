// Behaviour test for 0x006243D0: std::__adjust_heap(CTestQuest* first, int holeIndex, int len, CTestQuest val, CTestQuestNameCompare comp) -- STLport heap sift-down for the test-quest sort: move the preferred child up into the hole until the bottom, then sift `val` back up through __push_heap
// A copy of the source model and logic (compiled separately from src/006243d0.cpp) with test stand-ins for the callees.
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
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
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
    CTestQuest(const CTestQuest& other);           // 0x004A89D0
};

class CTestQuestNameCompare {
public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00623F80

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
}

template void __fastcall std::__adjust_heap<CTestQuest*, int, CTestQuest, CTestQuestNameCompare>(CTestQuest*, int, int, CTestQuest, CTestQuestNameCompare);

// ---- test stand-ins ----
static int g_live = 0;
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
const CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
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
CTestQuest::CTestQuest(const CTestQuest& o)
    : QuestName(o.QuestName), HSPName(o.HSPName), ColourIndex(o.ColourIndex), DisplayName(o.DisplayName), IniFile(o.IniFile),
      OutiFile(o.OutiFile), QuestCardDefName(o.QuestCardDefName) { }

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{                                                         // STLport body
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}

static int Fail() { std::printf("006243d0_TEST FAIL\n"); return 1; }

int main()
{
    const int n = 37;
    CTestQuest* p = (CTestQuest*)std::malloc(n * sizeof(CTestQuest));
    CTestQuestNameCompare comp;
    for (unsigned int seed = 3; seed < 20; ++seed) {
        Fill(p, n, seed);
        int before[64] = { 0 }, after[64] = { 0 };
        for (int i = 0; i < n; ++i) ++before[Key(p[i])];
        int live = g_live;
        for (int parent = (n - 2) / 2; ; --parent) {          // make_heap through __adjust_heap
            std::__adjust_heap(p, parent, n, CTestQuest(p[parent]), comp);
            if (parent == 0) break;
        }
        for (int c = 1; c < n; ++c)                            // max-heap by quest name
            if (comp(p[(c - 1) / 2], p[c])) return Fail();
        for (int len = n; len > 1; --len) {                    // sort_heap: pop the top to the back
            CTestQuest tmp(p[len - 1]);
            p[len - 1] = p[0];
            std::__adjust_heap(p, 0, len - 1, tmp, comp);
        }
        for (int i = 0; i < n; ++i) { if (!Intact(p[i])) return Fail(); ++after[Key(p[i])]; }
        for (int i = 1; i < n; ++i) if (comp(p[i], p[i - 1])) return Fail();
        for (int k = 0; k < 64; ++k) if (before[k] != after[k]) return Fail();
        if (g_live != live) return Fail();
    }
    std::free(p);
    std::printf("006243d0_TEST PASS\n");
    return 0;
}