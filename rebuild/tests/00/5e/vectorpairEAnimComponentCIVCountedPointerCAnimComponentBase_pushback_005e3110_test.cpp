// Behaviour test of vector<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > >::push_back(const pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >& x) (retail 0x005E3110)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

#include <new>
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

struct __false_type { __false_type() { } };                // user constructor: no zero store into the tag byte

template <class T> struct vector {                         // STLport vector: {start, finish, end of storage}
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x005E29D0
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

template struct vector<SAnimComponentPair>;

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

static int g_overflows = 0;
static unsigned int g_fillLen = 0;
static bool g_atEnd = false;
static SRawP8 g_grow[8];                                    // the reallocation target
template <class T> void vector<T>::_M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool atend)   // 0x005E29D0
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
    SAnimComponentPair* src = ArrangePairs(keys, 3);
    static SRawP8 storage[2];
    vector<SAnimComponentPair> v;
    v._M_start = v._M_finish = (SAnimComponentPair*)storage;
    v._M_end_of_storage = v._M_start + 2;
    v.push_back(src[0]);
    v.push_back(src[1]);
    bool ok = v._M_finish == v._M_start + 2 && g_overflows == 0 && g_comp[0].IVRefCount == 2 && g_comp[1].IVRefCount == 2;
    v.push_back(src[2]);                                    // full: the overflow path
    ok = ok && g_overflows == 1 && g_fillLen == 1 && g_atEnd && v._M_finish - v._M_start == 3;
    for (int i = 0; ok && i < 3; ++i)
        if (PKey(v._M_start[i]) != i || v._M_start[i].second.Object != &g_comp[i] || g_comp[i].IVRefCount != 2)
            ok = false;
    ok = ok && g_deleted == 0;
    if (!ok) { std::printf("005e3110_TEST FAIL\n"); return 1; }
    std::printf("005e3110_TEST PASS\n");
    return 0;
}