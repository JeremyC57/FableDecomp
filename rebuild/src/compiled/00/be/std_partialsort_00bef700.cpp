// std::__partial_sort(CDecalEntry* first, CDecalEntry* middle, CDecalEntry* last, CDecalEntry*, CDecalEntryLess comp) -- heapify [first, middle), pop every smaller entry of [middle, last) into the heap, then sort the heap: the ((long, long), CEngineTextureDecal) decal table's partial sort
// Flags: /Gr
// Leaf: ??$__partial_sort@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEF700, 249 bytes, __fastcall (first ecx, middle edx), `ret 0Ch` (last, the
//   value-type pointer, the empty comparator).
// Identity (probable): its callees are the landed decal-table heap trio -- __make_heap 0x00BEEE40 (pushed as comp, 0, 0),
//   __pop_heap 0x00BEEDA0 and sort_heap 0x00BEF350 -- over the 0x1C-byte pair<pair<long,long>, CEngineTextureDecal>
//   (stride `/ 0x1C` as the 0x24924925 multiply; lexicographic key compare); the body is STLport 4.6 _algo.c
//   __partial_sort verbatim. Catalog "Std_Make_Heap_ProcessPairs" is descriptive.
// Callees: __make_heap 0x00BEEE40, __pop_heap 0x00BEEDA0, sort_heap 0x00BEF350 (all __fastcall, out of line), malloc
//   0x00BFEA0E, __uninitialized_copy<const CCompositeBlendDesc*, CCompositeBlendDesc*> 0x005BAF28 (the by-value value's
//   inline vector copy; same-TU noinline body homes its tag in the dead value-type slot).
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
template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);   // 0x00BEEE40
template <class RandomAccessIter, class T, class Compare, class Distance>
void __fastcall __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value,
                           Compare comp, Distance*);                                       // 0x00BEEDA0
template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);    // 0x00BEF350

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (CDecalEntry*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
}   // namespace std

template void __fastcall std::__partial_sort(CDecalEntry*, CDecalEntry*, CDecalEntry*, CDecalEntry*, CDecalEntryLess);