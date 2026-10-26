// Behaviour test of vector<pair<unsigned long, CAnimationEntry> >::push_back(const pair<unsigned long, CAnimationEntry>& x) (retail 0x005E36B0)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

#include <new>
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

struct __false_type { __false_type() { } };                // user constructor: no zero store into the tag byte

template <class T> struct vector {                         // STLport vector: {start, finish, end of storage}
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x005E31F0
    void push_back(const T& x);
};

template <class T> inline void _Construct(T* p, const T& value)
{
    new (p) T(value);
}

template <class T> void vector<T>::push_back(const T& x)
{
    if (_M_finish != _M_end_of_storage) {
        _Construct(_M_finish, x);
        ++_M_finish;
    }
    else
        _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
}

template struct vector<SAnimationEntryPair>;

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

static int g_overflows = 0;
static unsigned int g_fillLen = 0;
static bool g_atEnd = false;
static SRawP20 g_grow[8];                                   // the reallocation target
template <class T> void vector<T>::_M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool atend)   // 0x005E31F0
{
    ++g_overflows;
    g_fillLen = fill_len;
    g_atEnd = atend && pos == _M_finish;
    T* start = (T*)g_grow;
    T* p = start;
    for (T* q = _M_start; q != _M_finish; ++q, ++p)
        new (p) T(*q);
    new (p) T(x);
    ++p;
    for (T* q = _M_start; q != _M_finish; ++q)
        q->~T();
    _M_start = start;
    _M_finish = p;
    _M_end_of_storage = start + 8;
}
int main()
{
    static const int keys[3] = { 0, 1, 2 };
    SAnimationEntryPair* src = ArrangeEntries(keys, 3);
    static SRawP20 storage[2];
    vector<SAnimationEntryPair> v;
    v._M_start = v._M_finish = (SAnimationEntryPair*)storage;
    v._M_end_of_storage = v._M_start + 2;
    v.push_back(src[0]);
    v.push_back(src[1]);
    bool ok = v._M_finish == v._M_start + 2 && g_overflows == 0 && g_maps[0].IVRefCount == 2 && g_maps[1].IVRefCount == 2;
    v.push_back(src[2]);                                    // full: the overflow path
    ok = ok && g_overflows == 1 && g_fillLen == 1 && g_atEnd && v._M_finish - v._M_start == 3;
    for (int i = 0; ok && i < 3; ++i)
        if (EKey(v._M_start[i]) != i || v._M_start[i].second.BankIndex != 100 + i || v._M_start[i].second.Components.Object != &g_maps[i]
            || g_maps[i].IVRefCount != 2)
            ok = false;
    ok = ok && g_deleted == 0;
    if (!ok) { std::printf("005e36b0_TEST FAIL\n"); return 1; }
    std::printf("005e36b0_TEST PASS\n");
    return 0;
}