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
// Model of std::__unguarded_partition<pair<CCharString,CPersistInfo>*, ..> @ 0x004BA080: Hoare partition by quest
// name around a by-value pivot taken from the range.
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
template <class _ForwardIter1, class _ForwardIter2>
void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2);                                  // 0x004B7800

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
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

namespace TLib {
template <class T> inline T* VT(T*) { return (T*)0; }
template <class RI, class T> inline void IterSwapT(RI a, RI b, T*) { T tmp = *a; *a = *b; *b = tmp; }
template <class RI, class T, class C> void UnguardedLinearInsert(RI last, T val, C comp)
{
    RI next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class RI, class T, class C> void LinearInsertT(RI first, RI last, T*, C comp)
{
    T val = *last;
    if (comp(val, *first)) {
        for (RI p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    }
    else
        UnguardedLinearInsert(last, val, comp);
}
template <class RI, class C> void InsertionSort(RI first, RI last, C comp)
{
    if (first == last)
        return;
    for (RI i = first + 1; i != last; ++i)
        LinearInsertT(first, i, VT(first), comp);
}
template <class RI, class D, class T, class C> void PushHeap(RI first, D hole, D top, T val, C comp)
{
    D parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = val;
}
template <class RI, class D, class T, class C> void AdjustHeap(RI first, D hole, D len, T val, C comp)
{
    D top = hole;
    D second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1))))
            second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) {
        *(first + hole) = *(first + (second - 1));
        hole = second - 1;
    }
    PushHeap(first, hole, top, val, comp);
}
template <class RI, class C, class T, class D> void MakeHeapT(RI first, RI last, C comp, T*, D*)
{
    if (last - first < 2)
        return;
    D len = (D)(last - first);
    D parent = (len - 2) / 2;
    for (;;) {
        T v = *(first + parent);
        AdjustHeap(first, parent, len, v, comp);
        if (parent == 0)
            return;
        parent--;
    }
}
template <class RI, class T, class C> void PopHeapAux(RI first, RI last, T*, C comp)
{
    T val = *(last - 1);
    *(last - 1) = *first;
    AdjustHeap(first, 0, (int)((last - 1) - first), val, comp);
}
template <class RI, class T, class C> void PartialSortT(RI first, RI middle, RI last, T*, C comp)
{
    MakeHeapT(first, middle, comp, (T*)0, (int*)0);
    for (RI i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            T val = *i;
            *i = *first;
            AdjustHeap(first, 0, (int)(middle - first), val, comp);
        }
    while (middle - first > 1) {
        PopHeapAux(first, middle, (T*)0, comp);
        --middle;
    }
}
template <class T, class C> const T& Median(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b))
        return comp(b, c) ? b : (comp(a, c) ? c : a);
    return comp(a, c) ? a : (comp(b, c) ? c : b);
}
template <class RI, class T, class C> RI UnguardedPartition(RI first, RI last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        IterSwapT(first, last, VT(first));
        ++first;
    }
}
template <class RI, class T, class C> void IntrosortLoop(RI first, RI last, T*, int depth, C comp)
{
    while (last - first > 16) {
        if (depth == 0) {
            PartialSortT(first, last, last, (T*)0, comp);
            return;
        }
        --depth;
        T pivot = Median(*first, *(first + (last - first) / 2), *(last - 1), comp);
        RI cut = UnguardedPartition(first, last, pivot, comp);
        IntrosortLoop(cut, last, (T*)0, depth, comp);
        last = cut;
    }
}
}
namespace std {
template <class A, class B> void __fastcall iter_swap(A a, B b) { TLib::IterSwapT(a, b, TLib::VT(a)); }
}
int main()
{
    bool ok = true;
    InitBlocks();
    unsigned int seed = 7u;
    for (int t = 0; t < 15; ++t) {
        int n = 2 + t * 2;
        CPersistEntry* a = NewEntries(n);
        for (int i = 0; i < n; ++i) {
            seed = seed * 1103515245u + 12345u;
            SetEntry(a[i], (int)((seed >> 16) % 7u) - 1);
        }
        int sum = KeySum(a, n);
        CPersistEntry pivot(a[n / 2]);
        int pk = KeyOf(pivot);
        CPersistEntry* cut = std::__unguarded_partition(a, a + n, CPersistEntry(pivot), CPersistLess());
        if (cut < a || cut > a + n)
            ok = false;
        for (CPersistEntry* p = a; p < cut; ++p)
            if (KeyOf(*p) > pk)
                ok = false;
        for (CPersistEntry* p = cut; p < a + n; ++p)
            if (KeyOf(*p) < pk)
                ok = false;
        for (int i = 0; i < n; ++i)
            if (!Consistent(a[i]))
                ok = false;
        if (KeySum(a, n) != sum)
            ok = false;
    }
    if (!ok) { printf("004ba080_TEST FAIL\n"); return 1; }
    printf("004ba080_TEST PASS\n");
    return 0;
}