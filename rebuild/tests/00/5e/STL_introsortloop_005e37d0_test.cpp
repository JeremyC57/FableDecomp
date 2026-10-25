// Behaviour test of std::__introsort_loop(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int depth_limit, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) (retail 0x005E37D0)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> struct CIVCountedPointer {              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

template <class T1, class T2> struct pair {                // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > SAnimComponentPair;   // 8 bytes

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > TAnimComponentLess;

SAnimComponentPair* __fastcall __unguarded_partition(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair pivot, TAnimComponentLess comp);   // 0x005E19D0
void __fastcall __partial_sort(SAnimComponentPair* first, SAnimComponentPair* middle, SAnimComponentPair* last, SAnimComponentPair*, TAnimComponentLess comp);   // 0x005E2F40

inline void partial_sort(SAnimComponentPair* first, SAnimComponentPair* middle, SAnimComponentPair* last, TAnimComponentLess comp)
{
    __partial_sort(first, middle, last, (SAnimComponentPair*)0, comp);
}

inline const SAnimComponentPair& __median(const SAnimComponentPair& a, const SAnimComponentPair& b, const SAnimComponentPair& c, TAnimComponentLess comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

void __fastcall __introsort_loop(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair*, int depth_limit, TAnimComponentLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        SAnimComponentPair* cut = __unguarded_partition(first, last,
                                                        SAnimComponentPair(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                                        comp);
        __introsort_loop(cut, last, (SAnimComponentPair*)0, depth_limit, comp);
        last = cut;
    }
}

// ---- test bodies of the retail callees ----
static int g_deleted = 0;                                  // components whose last reference went away
void CAnimComponentBase::v00() { }
void CAnimComponentBase::Delete() { ++g_deleted; }

struct SRawP8 { EAnimComponent first; CAnimComponentBase* second; };   // the pair's layout, viewed as SAnimComponentPair
static CAnimComponentBase g_comp[64];
static SRawP8 g_rawp8[64];
static SAnimComponentPair* ArrangePairs(const int* keys, int n)   // pair i = { keys[i], one reference to g_comp[keys[i]] }
{
    for (int i = 0; i < n; ++i) {
        g_rawp8[i].first = (EAnimComponent)keys[i];
        g_comp[keys[i]].IVRefCount = 1;
        g_rawp8[i].second = &g_comp[keys[i]];
    }
    return (SAnimComponentPair*)g_rawp8;
}
static SAnimComponentPair* MakePairs(int n, int mul)       // a permutation of the keys 0..n-1 (gcd(mul, n) == 1)
{
    int keys[64];
    for (int i = 0; i < n; ++i)
        keys[i] = (i * mul + 5) % n;
    return ArrangePairs(keys, n);
}
static int PKey(const SAnimComponentPair& p) { return (int)p.first; }
static bool PairsIntact(const SAnimComponentPair* a, int n)    // a key permutation, each pair still owning its component once
{
    int seen[64] = { 0 };
    for (int i = 0; i < n; ++i) {
        int k = PKey(a[i]);
        if (k < 0 || k >= n || seen[k] || a[i].second.Object != &g_comp[k] || g_comp[k].IVRefCount != 1)
            return false;
        seen[k] = 1;
    }
    return g_deleted == 0;
}
static bool PSorted(const SAnimComponentPair* a, int n) { for (int i = 1; i < n; ++i) if (PKey(a[i]) < PKey(a[i - 1])) return false; return true; }
static bool PHeap(const SAnimComponentPair* a, int n) { for (int i = 1; i < n; ++i) if (PKey(a[(i - 1) / 2]) < PKey(a[i])) return false; return true; }

static void TestPushHeap(SAnimComponentPair* first, int holeIndex, int topIndex, const SAnimComponentPair& val, TAnimComponentLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], val)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = val;
}

void __fastcall __adjust_heap(SAnimComponentPair* first, int holeIndex, int len, SAnimComponentPair val, TAnimComponentLess comp)   // 0x005E1210
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    TestPushHeap(first, holeIndex, topIndex, val, comp);
}

void __fastcall __make_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp, SAnimComponentPair*, int*)   // 0x005E1C30
{
    if (last - first < 2)
        return;
    int len = last - first;
    for (int parent = (len - 2) / 2; parent >= 0; --parent)
        __adjust_heap(first, parent, len, SAnimComponentPair(first[parent]), comp);
}

void __fastcall __pop_heap(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair* result,
                           SAnimComponentPair val, TAnimComponentLess comp, int*)   // 0x005E1B60
{
    *result = *first;
    __adjust_heap(first, 0, last - first, val, comp);
}

void __fastcall sort_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp)   // 0x005E2860
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, SAnimComponentPair(*last), comp, (int*)0);
    }
}

void __fastcall __partial_sort(SAnimComponentPair* first, SAnimComponentPair* middle, SAnimComponentPair* last, SAnimComponentPair*, TAnimComponentLess comp)   // 0x005E2F40
{
    __make_heap(first, middle, comp, (SAnimComponentPair*)0, (int*)0);
    for (SAnimComponentPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SAnimComponentPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

SAnimComponentPair* __fastcall __unguarded_partition(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair pivot, TAnimComponentLess comp)   // 0x005E19D0
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        SAnimComponentPair tmp = *first;
        *first = *last;
        *last = tmp;
        ++first;
    }
}

static void TestInsertionSort(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp)
{
    for (SAnimComponentPair* i = first + 1; i < last; ++i)
        for (SAnimComponentPair* j = i; j != first && comp(j[0], j[-1]); --j) {
            SAnimComponentPair tmp = j[0];
            j[0] = j[-1];
            j[-1] = tmp;
        }
}

int main()
{
    SAnimComponentPair* a = MakePairs(40, 7);
    __introsort_loop(a, a + 40, (SAnimComponentPair*)0, 10, TAnimComponentLess());
    bool ok = PairsIntact(a, 40);
    TestInsertionSort(a, a + 40, TAnimComponentLess());       // the loop leaves ordered runs of at most 16
    ok = ok && PSorted(a, 40) && PairsIntact(a, 40);
    a = MakePairs(40, 3);                                       // depth exhausted: partial_sort sorts the whole range
    __introsort_loop(a, a + 40, (SAnimComponentPair*)0, 0, TAnimComponentLess());
    ok = ok && PSorted(a, 40) && PairsIntact(a, 40);
    if (!ok) { std::printf("005e37d0_TEST FAIL\n"); return 1; }
    std::printf("005e37d0_TEST PASS\n");
    return 0;
}