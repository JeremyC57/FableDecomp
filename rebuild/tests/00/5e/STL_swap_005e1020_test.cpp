// Behaviour test of std::swap(pair<unsigned long, CAnimationEntry>& a, pair<unsigned long, CAnimationEntry>& b) (retail 0x005E1020)
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

void __fastcall swap(SAnimationEntryPair& a, SAnimationEntryPair& b)
{
    SAnimationEntryPair tmp = a;
    a = b;
    b = tmp;
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

int main()
{
    static const int keys[2] = { 1, 0 };
    SAnimationEntryPair* a = ArrangeEntries(keys, 2);
    swap(a[0], a[1]);
    bool ok = EKey(a[0]) == 0 && EKey(a[1]) == 1 && EntriesIntact(a, 2);
    swap(a[1], a[1]);                                       // self swap keeps the reference
    ok = ok && EKey(a[1]) == 1 && EntriesIntact(a, 2);
    if (!ok) { std::printf("005e1020_TEST FAIL\n"); return 1; }
    std::printf("005e1020_TEST PASS\n");
    return 0;
}