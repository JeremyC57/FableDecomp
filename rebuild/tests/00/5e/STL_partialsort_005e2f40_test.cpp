// Behaviour test of _STL::__partial_sort(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* middle, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) (retail 0x005E2F40)
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

void __fastcall __make_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp, SAnimComponentPair*, int*);   // 0x005E1C30
void __fastcall __pop_heap(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair* result,
                           SAnimComponentPair val, TAnimComponentLess comp, int*);   // 0x005E1B60
void __fastcall sort_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp);   // 0x005E2860

inline void make_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp)
{
    __make_heap(first, last, comp, (SAnimComponentPair*)0, (int*)0);
}

void __fastcall __partial_sort(SAnimComponentPair* first, SAnimComponentPair* middle, SAnimComponentPair* last, SAnimComponentPair*, TAnimComponentLess comp)
{
    make_heap(first, middle, comp);
    for (SAnimComponentPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SAnimComponentPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
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

int main()
{
    SAnimComponentPair* a = MakePairs(12, 5);
    __partial_sort(a, a + 4, a + 12, (SAnimComponentPair*)0, TAnimComponentLess());
    bool ok = PairsIntact(a, 12);
    for (int i = 0; ok && i < 4; ++i)
        if (PKey(a[i]) != i)
            ok = false;
    if (!ok) { std::printf("005e2f40_TEST FAIL\n"); return 1; }
    std::printf("005e2f40_TEST PASS\n");
    return 0;
}