// std::__introsort_loop(CDecalEntry* first, CDecalEntry* last, CDecalEntry*, int depth_limit, CDecalEntryLess comp) -- introsort of the ((long, long), CEngineTextureDecal) entries down to 16-element runs: median-of-three partition, recurse on the right part, heap-sort fallback when the depth budget runs out
// Leaf: ??$__introsort_loop@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEFA20, 405 bytes, `ret 0Ch` (value-type pointer, depth limit, comparator).
// Callees: __unguarded_partition @ 0x00BEEC60 (landed), __partial_sort @ 0x00BEF700, malloc @ 0x00BFEA0E and the blend
// descriptors' __uninitialized_copy @ 0x005BAF28 (the pivot's inline vector copy constructor; same-TU stand-in body
// here), itself.
// Label: the manifest's "std_make_heap_build_loop" is propagated. The name is bound by the landed caller 0x00BEFC20
// (reloc_bindings: ??$__introsort_loop@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@U12@HV?$CKeyPairCompareLess@...);
// the twin over the CCompositeSurfaceInfo map is 0x00BEF1D0 (landed). Element: see 0x00BEE320.
// Shape: STLport 4.6 __introsort_loop (__stl_threshold 16, the 28-byte distance via the 0x92492493 reciprocal);
// __median and partial_sort inline; the pivot T(...) is copy-constructed in its outgoing slot.
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CRGBColour {                                  // Ego_r.pdb: union of {B, G, R, A} and the packed word
public:
    unsigned char B, G, R, A;
};
class CCompositeBlendDesc {                         // Ego_r.pdb, 16 bytes
public:
    unsigned long TextureIndex;                     // +0x00
    long          BlendType;                        // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                    // +0x08
    CRGBColour    SpecularColour;                   // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

namespace std {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }   // STLport's explicit memberwise copy
};
template <class T1, class T2> inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

#pragma auto_inline(off)
// Same-TU body of the blend descriptors' STLport __uninitialized_copy 0x005BAF28 (the tag is never read).
template <class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result,
                                                      const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
#pragma auto_inline(on)

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const Alloc& a, Value p) : Alloc(a), _M_data(p) { }
};
template <class T, class Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int n, const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};
// The decals' blend list: STLport vector<CCompositeBlendDesc> (12 bytes in retail). Copy constructor inline (the
// allocation, then the out-of-line POD copy 0x005BAF28) or out of line (0x005BB346), operator= out of line 0x006CADE0.
template <class T, class Alloc> class vector : public _Vector_base<T, Alloc> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    Alloc get_allocator() const { return Alloc(); }
    vector(const vector& x) : _Vector_base<T, Alloc>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
    }
    ~vector() { }
    vector& operator=(const vector& x);             // 0x006CADE0
};
}   // namespace std

// Ego_r.pdb CEngineTextureDecal (20 bytes in retail: the STLport vector is 12).
class CEngineTextureDecal {
public:
    long OriginalTextureIndex;                      // +0x00
    long DecalID;                                   // +0x04
    std::vector<CCompositeBlendDesc, std::allocator<CCompositeBlendDesc> > OverlaidBlends;   // +0x08
};

typedef std::pair<long, long> CDecalKey;
typedef std::pair<CDecalKey, CEngineTextureDecal> CDecalEntry;   // 0x1c bytes

// The vector map's key order (an empty functor passed by value): the lexicographic pair<long, long> compare.
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const K& a, const std::pair<K, V>& b) const { return a < b.first; }
    bool operator()(const std::pair<K, V>& a, const K& b) const { return a.first < b; }
};
typedef CKeyPairCompareLess<CDecalKey, CEngineTextureDecal> CDecalEntryLess;

namespace std {
template <class T, class Compare>
inline const T& __median(const T& a, const T& b, const T& c, Compare comp)
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

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp);   // 0x00BEEC60

template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp);   // 0x00BEF700

template <class RandomAccessIter, class Compare>
inline void partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Compare comp)
{
    __partial_sort(first, middle, last, (CDecalEntry*)0, comp);
}

#define __stl_threshold 16

template <class RandomAccessIter, class T, class Size, class Compare>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp)
{
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        RandomAccessIter cut =
            __unguarded_partition(first, last,
                                  T(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                  comp);
        __introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}
}   // namespace std

template void std::__introsort_loop(CDecalEntry*, CDecalEntry*, CDecalEntry*, int, CDecalEntryLess);