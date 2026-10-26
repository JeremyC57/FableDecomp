// behaviour test for 00bef350: SGI sort_heap over (pair<long,long>, CEngineTextureDecal) entries ordered lexicographically by the key pair
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_vectors = 0;
class CCompositeBlendDesc { public:
    unsigned long TextureIndex;
    long          BlendType;
    unsigned long DiffuseColour;
    unsigned long SpecularColour;
};
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector(const vector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { Assign(x._M_start, x.size()); }
    ~vector() { Free(); }
    vector& operator=(const vector& x) { if (this != &x) { Free(); Assign(x._M_start, x.size()); } return *this; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void Free() { if (_M_start) { std::free(_M_start); --g_vectors; } _M_start = _M_finish = _M_end_of_storage = 0; }
    void Assign(const T* p, unsigned int n)
    {
        unsigned int i = 0;
        _M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        if (_M_start) ++g_vectors;
        for (i = 0; i < n; ++i) _M_start[i] = p[i];
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
};
class CEngineTextureDecal { public:
    long                        OriginalTextureIndex;
    long                        DecalID;
    vector<CCompositeBlendDesc> OverlaidBlends;
    CEngineTextureDecal() : OriginalTextureIndex(0), DecalID(0) { }
};
struct SLongPair {
    long first;
    long second;
    SLongPair() : first(0), second(0) { }
    SLongPair(const SLongPair& other) : first(other.first), second(other.second) { }
};
struct SDecalEntry {
    SLongPair           first;
    CEngineTextureDecal second;
};
struct CKeyPairCompareLess {
    bool operator()(const SDecalEntry& a, const SDecalEntry& b) const
    {
        return a.first.first < b.first.first || (!(b.first.first < a.first.first) && a.first.second < b.first.second);
    }
};
typedef SDecalEntry ElemT;
typedef CKeyPairCompareLess CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    CCompositeBlendDesc blends[3];
    unsigned int count = (unsigned int)(k % 3) + 1;
    unsigned int i = 0;
    for (i = 0; i < count; ++i) {
        blends[i].TextureIndex = (unsigned long)(k + i);
        blends[i].BlendType = (long)i;
        blends[i].DiffuseColour = 0xff000000u | (unsigned long)k;
        blends[i].SpecularColour = (unsigned long)(k * 2);
    }
    e.first.first = k / 10;
    e.first.second = k % 10;
    e.second.OriginalTextureIndex = k + 100;
    e.second.DecalID = -k;
    e.second.OverlaidBlends.Free();
    e.second.OverlaidBlends.Assign(blends, count);
}
static int KeyOf(const ElemT& e) { return int(e.first.first * 10 + e.first.second); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    unsigned int i = 0;
    const vector<CCompositeBlendDesc>& v = e.second.OverlaidBlends;
    if (e.second.OriginalTextureIndex != k + 100 || e.second.DecalID != -k) return false;
    if (v.size() != (unsigned int)(k % 3) + 1) return false;
    for (i = 0; i < v.size(); ++i)
        if (v._M_start[i].TextureIndex != (unsigned long)(k + i) || v._M_start[i].BlendType != (long)i
            || v._M_start[i].DiffuseColour != (0xff000000u | (unsigned long)k) || v._M_start[i].SpecularColour != (unsigned long)(k * 2))
            return false;
    return true;
}
static bool Live(int n) { return g_vectors == n; }
static bool Final() { return g_vectors == 0; }

static void PushHeapT(ElemT* first, long hole, long top, ElemT value, CompT comp)
{
    long parent = (hole - 1) / 2;
    while (top < hole && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeapT(ElemT* first, long hole, long len, ElemT value, CompT comp)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < len; idx = 2 * idx + 2) {
        if (comp(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx];
        hole = idx;
    }
    if (idx == len) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeapT(first, hole, top, value, comp);
}

// the out-of-line callee (__pop_heap in retail), modelled with the standard hole algorithm
static void __pop_heap(ElemT* first, ElemT* last, ElemT* result, ElemT value, CompT comp, int*)
{
    *result = *first;
    AdjustHeapT(first, 0, long(last - first), value, comp);
}
// the reconstructed body: SGI sort_heap with pop_heap / __pop_heap_aux inline
inline void __pop_heap_aux(ElemT* first, ElemT* last, ElemT*, CompT comp)
{
    __pop_heap(first, last - 1, last - 1, ElemT(*(last - 1)), comp, (int*)0);
}
inline void pop_heap(ElemT* first, ElemT* last, CompT comp)
{
    __pop_heap_aux(first, last, (ElemT*)0, comp);
}
static void sort_heap(ElemT* first, ElemT* last, CompT comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}
#define SORT(a, n, comp) sort_heap(a, a + n, comp)

int main()
{
    static const int keys[9] = { 5, 31, 12, 7, 64, 0, 99, 40, 2 };
    const int n = 9;
    int ok = 1;
    {
        ElemT a[9];
        CompT comp = MakeComp();
        int i = 0;
        for (i = 0; i < n; ++i) Fill(a[i], keys[i]);
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        for (i = 0; i < n; ++i) if (!Check(a[i])) ok = 0;
        if (KeyOf(a[0]) != 0 || KeyOf(a[n - 1]) != 99) ok = 0;
        if (!Live(n)) ok = 0;
        // re-heap the sorted output and sort again; a one-element range is left as it is
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        SORT(a, 1, comp);
        if (KeyOf(a[0]) != 0 || !Check(a[0])) ok = 0;
        if (!Live(n)) ok = 0;
    }
    if (!Final()) ok = 0;
    if (!ok) { std::printf("00bef350_TEST FAIL\n"); return 1; }
    std::printf("00bef350_TEST PASS\n");
    return 0;
}