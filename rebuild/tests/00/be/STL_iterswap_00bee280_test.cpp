// behaviour copy of STLport iter_swap over the CVectorMap<pair<long, long>, CEngineTextureDecal> entries @ 0x00BEE280
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Behaviour model of the CVectorMap<pair<long, long>, CEngineTextureDecal> STLport helpers (the entry is a two-long
// key plus a decal owning a heap vector of blend descriptors; copies deep-copy it, the by-value temporaries free it).
static int g_mallocs = 0, g_frees = 0;
static void* TestMalloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void TestFree(void* p) { if (p) { ++g_frees; std::free(p); } }

class CCompositeBlendDesc {
public:
    unsigned int TextureIndex;
    int          BlendType;
    unsigned int DiffuseColour;
    unsigned int SpecularColour;
};
class CBlendVector {                         // STLport vector<CCompositeBlendDesc> stand-in
public:
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
    CBlendVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CBlendVector(const CBlendVector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { Assign(x); }
    ~CBlendVector() { TestFree(_M_start); }
    CBlendVector& operator=(const CBlendVector& x) { if (this != &x) { TestFree(_M_start); _M_start = _M_finish = _M_end_of_storage = 0; Assign(x); } return *this; }
    void Assign(const CBlendVector& x)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n ? (CCompositeBlendDesc*)TestMalloc(n * sizeof(CCompositeBlendDesc)) : 0;
        for (unsigned int i = 0; i < n; ++i) _M_start[i] = x._M_start[i];
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
class CEngineTextureDecal {
public:
    long OriginalTextureIndex;
    long DecalID;
    CBlendVector OverlaidBlends;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
template <class T1, class T2> inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}
typedef pair<long, long> CDecalKey;
typedef pair<CDecalKey, CEngineTextureDecal> CDecalEntry;
struct CDecalEntryLess {
    bool operator()(const CDecalEntry& a, const CDecalEntry& b) const { return a.first < b.first; }
    bool operator()(const CDecalKey& a, const CDecalEntry& b) const { return a < b.first; }
};

// fixture: entry i gets key (k0[i], k1[i]), OriginalTextureIndex i, DecalID 10 * i and (i % 3) + 1 blends 100 * i + j
static CDecalKey g_key[64];
static void Fill(CDecalEntry& e, long k0, long k1, int i)
{
    e.first.first = k0;
    e.first.second = k1;
    g_key[i] = e.first;
    e.second.OriginalTextureIndex = i;
    e.second.DecalID = 10 * i;
    CBlendVector v;
    unsigned int n = (unsigned int)(i % 3) + 1;
    v._M_start = (CCompositeBlendDesc*)TestMalloc(n * sizeof(CCompositeBlendDesc));
    for (unsigned int j = 0; j < n; ++j) { v._M_start[j].TextureIndex = 100 * i + j; v._M_start[j].BlendType = i; v._M_start[j].DiffuseColour = 0; v._M_start[j].SpecularColour = 0; }
    v._M_finish = v._M_end_of_storage = v._M_start + n;
    e.second.OverlaidBlends = v;
}
static const long kK0[40] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3, 8, 4,
                              6, 2, 6, 4, 3, 3, 8, 3, 2, 7, 9, 5, 0, 2, 8, 8, 4, 1, 9, 7 };
static const long kK1[40] = { 7, 2, 9, 1, 0, 4, 6, 3, 8, 5, 1, 7, 2, 9, 6, 0, 4, 3, 8, 5,
                              2, 7, 1, 9, 6, 4, 0, 8, 3, 5, 7, 1, 2, 9, 4, 6, 8, 0, 3, 5 };
static void Setup(CDecalEntry* a, int n) { for (int i = 0; i < n; ++i) Fill(a[i], kK0[i], kK1[i], i); }
static bool Intact(const CDecalEntry& e)
{
    long i = e.second.OriginalTextureIndex;
    if (i < 0 || i >= 64 || e.second.DecalID != 10 * i) return false;
    if (e.first.first != g_key[i].first || e.first.second != g_key[i].second) return false;
    const CBlendVector& v = e.second.OverlaidBlends;
    if (v.size() != (unsigned int)(i % 3) + 1) return false;
    for (unsigned int j = 0; j < v.size(); ++j) if (v._M_start[j].TextureIndex != (unsigned int)(100 * i + j)) return false;
    return true;
}
static bool AllIntact(const CDecalEntry* a, int n)
{
    int seen[64];
    std::memset(seen, 0, sizeof(seen));
    for (int i = 0; i < n; ++i) {
        if (!Intact(a[i])) return false;
        if (seen[a[i].second.OriginalTextureIndex]++) return false;
    }
    return true;
}
static bool Sorted(const CDecalEntry* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[i].first < a[i - 1].first) return false;
    return true;
}

// the reconstructed STLport bodies (and their callees)
struct __false_type { };
template <class I1, class I2> inline void iter_swap(I1 a, I2 b) { CDecalEntry tmp = *a; *a = *b; *b = tmp; }
template <class R, class T, class C> void __unguarded_linear_insert(R last, T val, C comp)
{
    R next = last;
    --next;
    const CDecalKey key = val.first;
    while (comp(key, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    last->first = key;
    last->second = val.second;
}
template <class I1, class I2> I2 copy_backward(I1 first, I1 last, I2 result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
template <class R, class T, class C> void __linear_insert(R first, R last, T val, C comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
template <class R, class C> void __insertion_sort(R first, R last, C comp)
{
    if (first == last) return;
    for (R i = first + 1; i != last; ++i)
        __linear_insert(first, i, CDecalEntry(*i), comp);
}
template <class R, class T, class C> void __unguarded_insertion_sort_aux(R first, R last, T*, C comp)
{
    for (R i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
template <class R, class T, class C> R __unguarded_partition(R first, R last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        iter_swap(first, last);
        ++first;
    }
}
template <class R, class D, class T, class C> void __adjust_heap(R first, D hole, D len, T val, C comp)
{
    D top = hole;
    D second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1)))) second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { *(first + hole) = *(first + (second - 1)); hole = second - 1; }
    D parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = val;
}
template <class R, class C, class T, class D> void __make_heap(R first, R last, C comp, T*, D*)
{
    if (last - first < 2) return;
    D len = (D)(last - first);
    D parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class T> static T* ucopy(T* first, T* last, T* result)
{
    for (; first != last; ++first, ++result) new (result) T(*first);
    return result;
}
class CDecalVector {
public:
    CDecalEntry* _M_start;
    CDecalEntry* _M_finish;
    CDecalEntry* _M_end_of_storage;
    CDecalVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CDecalVector() { _M_clear(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear() { for (CDecalEntry* p = _M_start; p != _M_finish; ++p) p->~CDecalEntry(); TestFree(_M_start); }
    void _M_insert_overflow(CDecalEntry* position, const CDecalEntry& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CDecalEntry* new_start = len ? (CDecalEntry*)TestMalloc(len * sizeof(CDecalEntry)) : 0;
        CDecalEntry* new_finish = ucopy(_M_start, position, new_start);
        if (fill_len == 1) { new (new_finish) CDecalEntry(x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) CDecalEntry(x);
        if (!atend) new_finish = ucopy(position, _M_finish, new_finish);
        _M_clear();
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
    void push_back(const CDecalEntry& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CDecalEntry(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

int main()
{
    int ok = 1;
    CDecalEntryLess comp;
    (void)comp;

    {
        CDecalEntry a[2];
        Setup(a, 2);
        iter_swap(a, a + 1);
        if (a[0].second.OriginalTextureIndex != 1 || a[1].second.OriginalTextureIndex != 0 || !AllIntact(a, 2)) ok = 0;
    }
    if (g_mallocs != g_frees) ok = 0;                     // every blend block freed exactly once
    if (ok) std::printf("00bee280_TEST PASS\n");
    else { std::printf("00bee280_TEST FAIL\n"); return 1; }
    return 0;
}