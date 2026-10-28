#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of std::__unguarded_linear_insert<pair<CCharString,CPersistInfo>*, ..> @ 0x004B9480: walks the entry down
// from last past every entry whose quest name sorts after it (a null name sorts first).
template <class T> class CBasicString {
public:
    T* m_pData;                                             // +0x0
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};

static int gCopies = 0, gDtors = 0;
class CCharString {
public:
    CBasicString<char>* PStringData;                        // +0x0 the shared string block
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++gCopies; }                  // 0x0099EC30
    ~CCharString() { ++gDtors; }                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }       // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                    // Ego_r.pdb, 8 bytes
    public:
        CCharString QuestName;                              // +0x0
        CCharString PersistScript;                          // +0x4
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo CPersistInfo;
typedef std::pair<CCharString, CPersistInfo> CPersistEntry;
typedef CKeyPairCompareLess<CCharString, CPersistInfo> CPersistLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

}

static char gText[6][8] = { "alpha", "bravo", "charlie", "delta", "echo", "fox" };
static CBasicString<char> gBlocks[6];
static void InitBlocks() { for (int i = 0; i < 6; ++i) gBlocks[i].m_pData = gText[i]; }
static void SetEntry(CPersistEntry& e, int k)
{
    CBasicString<char>* b = k < 0 ? 0 : &gBlocks[k];
    e.first.PStringData = b;
    e.second.QuestName.PStringData = b;
    e.second.PersistScript.PStringData = b;
}
static int KeyOf(const CPersistEntry& e) { return e.first.PStringData ? (int)(e.first.PStringData - gBlocks) : -1; }
static bool Consistent(const CPersistEntry& e)
{
    return e.second.QuestName.PStringData == e.first.PStringData && e.second.PersistScript.PStringData == e.first.PStringData;
}
static CPersistEntry* NewEntries(int n) { return (CPersistEntry*)malloc((n + 1) * sizeof(CPersistEntry)); }
static int KeySum(const CPersistEntry* a, int n) { int s = 0; for (int i = 0; i < n; ++i) s += KeyOf(a[i]) + 1; return s; }
int main()
{
    bool ok = true;
    InitBlocks();
    CPersistEntry* a = NewEntries(5);
    int base[4] = { 0, 2, 3, 4 };
    for (int i = 0; i < 4; ++i)
        SetEntry(a[i], base[i]);
    SetEntry(a[4], 1);
    int dtors = gDtors;
    std::__unguarded_linear_insert(a + 4, CPersistEntry(a[4]), CPersistLess());
    int expect[5] = { 0, 1, 2, 3, 4 };
    for (int i = 0; i < 5; ++i)
        if (KeyOf(a[i]) != expect[i] || !Consistent(a[i]))
            ok = false;
    if (gDtors - dtors != 3)                             // the by-value entry's three strings
        ok = false;
    int base2[3] = { -1, 1, 5 };
    for (int i = 0; i < 3; ++i)
        SetEntry(a[i], base2[i]);
    SetEntry(a[3], 3);
    std::__unguarded_linear_insert(a + 3, CPersistEntry(a[3]), CPersistLess());
    int expect2[4] = { -1, 1, 3, 5 };
    for (int i = 0; i < 4; ++i)
        if (KeyOf(a[i]) != expect2[i] || !Consistent(a[i]))
            ok = false;
    if (!ok) { printf("004b9480_TEST FAIL\n"); return 1; }
    printf("004b9480_TEST PASS\n");
    return 0;
}