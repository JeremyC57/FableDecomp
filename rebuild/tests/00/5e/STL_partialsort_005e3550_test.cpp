// Behaviour test of std::__partial_sort(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* middle, pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry>*, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp) (retail 0x005E3550)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

class CAnimComponentMap { public:                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
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

class CDefString { public:                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

class CAnimationEntry { public:                                   // Ego_r.pdb, 0x10 bytes
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
};

template <class T1, class T2> struct pair {                // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CAnimationEntry> SAnimationEntryPair;   // 0x14 bytes: CRC key + entry

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> TAnimationEntryLess;

void __fastcall __make_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp, SAnimationEntryPair*, int*);   // 0x005E2320
void __fastcall __pop_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result,
                           SAnimationEntryPair val, TAnimationEntryLess comp, int*);   // 0x005E21F0
void __fastcall sort_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp);   // 0x005E2FB0

inline void make_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp)
{
    __make_heap(first, last, comp, (SAnimationEntryPair*)0, (int*)0);
}

void __fastcall __partial_sort(SAnimationEntryPair* first, SAnimationEntryPair* middle, SAnimationEntryPair* last, SAnimationEntryPair*, TAnimationEntryLess comp)
{
    make_heap(first, middle, comp);
    for (SAnimationEntryPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SAnimationEntryPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

// ---- test bodies of the retail callees ----
static int g_deleted = 0;                                  // component maps whose last reference went away
void CAnimComponentMap::v00() { }
void CAnimComponentMap::Delete() { ++g_deleted; }

struct SRawP20 { unsigned long first; long bank; long name; long group; CAnimComponentMap* comps; };   // viewed as SAnimationEntryPair
static CAnimComponentMap g_maps[64];
static SRawP20 g_rawp20[64];
static SAnimationEntryPair* ArrangeEntries(const int* keys, int n)   // entry i = { CRC keys[i], bank/name/group derived from it, one map reference }
{
    for (int i = 0; i < n; ++i) {
        int k = keys[i];
        g_rawp20[i].first = (unsigned long)k;
        g_rawp20[i].bank = 100 + k;
        g_rawp20[i].name = 200 + k;
        g_rawp20[i].group = 300 + k;
        g_maps[k].IVRefCount = 1;
        g_rawp20[i].comps = &g_maps[k];
    }
    return (SAnimationEntryPair*)g_rawp20;
}
static SAnimationEntryPair* MakeEntries(int n, int mul)   // a permutation of the keys 0..n-1 (gcd(mul, n) == 1)
{
    int keys[64];
    for (int i = 0; i < n; ++i)
        keys[i] = (i * mul + 5) % n;
    return ArrangeEntries(keys, n);
}
static int EKey(const SAnimationEntryPair& p) { return (int)p.first; }
static bool EntriesIntact(const SAnimationEntryPair* a, int n)   // a key permutation, every field moved with its key, one reference each
{
    int seen[64] = { 0 };
    for (int i = 0; i < n; ++i) {
        int k = EKey(a[i]);
        if (k < 0 || k >= n || seen[k] || a[i].second.BankIndex != 100 + k || a[i].second.AnimName.TablePos != 200 + k
            || a[i].second.GroupName.TablePos != 300 + k || a[i].second.Components.Object != &g_maps[k] || g_maps[k].IVRefCount != 1)
            return false;
        seen[k] = 1;
    }
    return g_deleted == 0;
}
static bool ESorted(const SAnimationEntryPair* a, int n) { for (int i = 1; i < n; ++i) if (EKey(a[i]) < EKey(a[i - 1])) return false; return true; }
static bool EHeap(const SAnimationEntryPair* a, int n) { for (int i = 1; i < n; ++i) if (EKey(a[(i - 1) / 2]) < EKey(a[i])) return false; return true; }

static void TestPushHeap(SAnimationEntryPair* first, int holeIndex, int topIndex, const SAnimationEntryPair& val, TAnimationEntryLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], val)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = val;
}

void __fastcall __adjust_heap(SAnimationEntryPair* first, int holeIndex, int len, SAnimationEntryPair val, TAnimationEntryLess comp)   // 0x005E1CB0
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

void __fastcall __make_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp, SAnimationEntryPair*, int*)   // 0x005E2320
{
    if (last - first < 2)
        return;
    int len = last - first;
    for (int parent = (len - 2) / 2; parent >= 0; --parent)
        __adjust_heap(first, parent, len, SAnimationEntryPair(first[parent]), comp);
}

void __fastcall __pop_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result,
                           SAnimationEntryPair val, TAnimationEntryLess comp, int*)   // 0x005E21F0
{
    *result = *first;
    __adjust_heap(first, 0, last - first, val, comp);
}

void __fastcall sort_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp)   // 0x005E2FB0
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, SAnimationEntryPair(*last), comp, (int*)0);
    }
}

int main()
{
    SAnimationEntryPair* a = MakeEntries(12, 5);
    __partial_sort(a, a + 4, a + 12, (SAnimationEntryPair*)0, TAnimationEntryLess());
    bool ok = EntriesIntact(a, 12);
    for (int i = 0; ok && i < 4; ++i)
        if (EKey(a[i]) != i)
            ok = false;
    if (!ok) { std::printf("005e3550_TEST FAIL\n"); return 1; }
    std::printf("005e3550_TEST PASS\n");
    return 0;
}