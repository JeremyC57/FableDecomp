// _STL::__unguarded_linear_insert(CDecalEntry* last, CDecalEntry val, CDecalEntryLess comp) -- shift larger-keyed ((long, long), CEngineTextureDecal) entries up one slot until val's place is found (no lower bound check), store val there
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@U?$pair@JJ@_STL@@VCEngineTextureDecal@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE320, 126 bytes, `ret 20h` (the 28-byte value + the empty comparator).
// Callees: vector<CCompositeBlendDesc>::operator= @ 0x006CADE0, free @ 0x00BFEA14 (the by-value value's blend vector).
// Label: the manifest's "std_vector_insert_sorted" is descriptive; this is the STLport sort instantiation of
// CVectorMap<pair<long, long>, CEngineTextureDecal, CKeyPairCompareLess<...> > (Ego_r.pdb name set; the morph info's
// TextureDecals at +0).
// Element: _STL::pair<pair<long, long>, CEngineTextureDecal> (0x1c bytes): key (two longs), then Ego_r.pdb
// CEngineTextureDecal {OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends (STLport, 12 bytes)}.
// The implicit operator= copies four words and calls the vector's out-of-line operator=; the by-value value's vector
// frees its block (inline ~_Vector_base). Shape: STLport 4.6 __unguarded_linear_insert, lexicographic signed key compare.
// Register shape: retail loads the value's two key words into ebx/ebp at entry and keeps them across the operator= calls
// (the landed __push_heap 0x00BEE420 of this TU does the same with the first word); the verbatim body re-reads them from
// the argument slot (122 bytes). The model reads the key into a local once, compares through a (key, entry) overload of
// the comparator (same lexicographic test) and drops the value as `last->first = key; last->second = val.second`.
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
};

namespace _STL {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T1, class T2> inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class InputIter, class ForwardIter, class Tag>
ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const Tag&);   // 0x005BAF28 (blend descs)

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
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
    ~vector() { }
    vector& operator=(const vector& x);             // 0x006CADE0
};
}   // namespace _STL

// Ego_r.pdb CEngineTextureDecal (20 bytes in retail: the STLport vector is 12).
class CEngineTextureDecal {
public:
    long OriginalTextureIndex;                      // +0x00
    long DecalID;                                   // +0x04
    _STL::vector<CCompositeBlendDesc, _STL::allocator<CCompositeBlendDesc> > OverlaidBlends;   // +0x08
};

typedef _STL::pair<long, long> CDecalKey;
typedef _STL::pair<CDecalKey, CEngineTextureDecal> CDecalEntry;   // 0x1c bytes

// The vector map's key order (an empty functor passed by value): the lexicographic pair<long, long> compare.
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const K& a, const _STL::pair<K, V>& b) const { return a < b.first; }
};
typedef CKeyPairCompareLess<CDecalKey, CEngineTextureDecal> CDecalEntryLess;
namespace _STL {
template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
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
}   // namespace _STL

template void _STL::__unguarded_linear_insert(CDecalEntry*, CDecalEntry, CDecalEntryLess);