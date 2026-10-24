// _STL::iter_swap(CDecalEntry* a, CDecalEntry* b) -- swap two ((long, long), CEngineTextureDecal) entries through a temporary copy
// Leaf: ??$iter_swap@PAU?$pair@U?$pair@JJ@_STL@@VCEngineTextureDecal@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE280, 146 bytes, `ret` (both pointers in ecx/edx).
// Callees: vector<CCompositeBlendDesc> copy constructor @ 0x005BB346 (out of line here), operator= @ 0x006CADE0 (twice),
// free @ 0x00BFEA14 (the temporary's blend vector).
// Label: the manifest's "swap<CEngineTextureDecal>" is the Ego_r.pdb (Dinkumware) name of a different helper; retail is
// STLport's iter_swap (caller: __unguarded_partition @ 0x00BEEC60, this chunk). Element: see 0x00BEE320.
// Shape: STLport 4.6 iter_swap with __iter_swap inline (`__forceinline`: VC7.1 keeps it out of line otherwise): `T tmp =
// *a; *a = *b; *b = tmp;`.
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
    bool operator()(const _STL::pair<K, V>& a, const K& b) const { return a.first < b; }
};
typedef CKeyPairCompareLess<CDecalKey, CEngineTextureDecal> CDecalEntryLess;

namespace _STL {
template <class ForwardIter1, class ForwardIter2, class T>
__forceinline void __iter_swap(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap(a, b, (CDecalEntry*)0);
}
}   // namespace _STL

template void _STL::iter_swap(CDecalEntry*, CDecalEntry*);