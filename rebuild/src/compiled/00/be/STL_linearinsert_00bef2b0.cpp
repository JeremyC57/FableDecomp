// std::__linear_insert(CDecalEntry* first, CDecalEntry* last, CDecalEntry val, CDecalEntryLess comp) -- insertion step: a new minimum shifts [first, last) up one slot and lands at first, anything else takes the unguarded linear insert
// Leaf: ??$__linear_insert@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEF2B0, 156 bytes, `ret 20h` (the 28-byte value + the empty comparator).
// Callees: __copy_backward @ 0x00BEE4E0 (landed), vector<CCompositeBlendDesc>::operator= @ 0x006CADE0 and copy
// constructor @ 0x005BB346 (out of line here), __unguarded_linear_insert @ 0x00BEE320 (this chunk), free @ 0x00BFEA14.
// Label: the manifest's "Std_Vector_InsertWithCompare" is descriptive; caller: __insertion_sort @ 0x00BEF640 (this chunk).
// Element: see 0x00BEE320. Shape: STLport 4.6 __linear_insert verbatim (`if (comp(val, *first)) { copy_backward(first,
// last, last + 1); *first = val; } else __unguarded_linear_insert(last, val, comp);`, the implicit pair assignment and
// copy inline); copy_backward's random_access_iterator_tag is homed in the dead comparator slot. Retail keeps the value's
// key words in ebx/ebp from entry: that needs same-TU noinline bodies of the blend vector's copy constructor 0x005BB346 and
// operator= 0x006CADE0 (the value's address only reaches them through &val.second.OverlaidBlends); declared only, the
// value is address-exposed and its key is re-read from the argument slot (164 bytes).
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
    vector(const vector& x);                        // 0x005BB346 (same-TU body below)
    ~vector() { }
    vector& operator=(const vector& x);             // 0x006CADE0 (same-TU body below)
};

// Same-TU stand-in bodies of the vector's out-of-line copy constructor and assignment: their visibility tells VC7.1
// that the value's address (passed as &val.second.OverlaidBlends) does not escape.
#pragma auto_inline(off)
template <class T, class Alloc>
__declspec(noinline) vector<T, Alloc>::vector(const vector& x) : _Vector_base<T, Alloc>(x.size(), x.get_allocator())
{
    this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
}
template <class T, class Alloc>
__declspec(noinline) vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& x)
{
    if (&x != this) {
        unsigned int xlen = x.size();
        this->_M_end_of_storage.deallocate(this->_M_start, 0);
        this->_M_start = this->_M_end_of_storage.allocate(xlen);
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
        this->_M_end_of_storage._M_data = this->_M_finish;
    }
    return *this;
}
#pragma auto_inline(on)
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
};
typedef CKeyPairCompareLess<CDecalKey, CEngineTextureDecal> CDecalEntryLess;

namespace std {
template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter2 __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                   const random_access_iterator_tag&, Distance*);   // 0x00BEE4E0

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);   // 0x00BEE320

template <class RandomAccessIter, class T, class Compare>
void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
}   // namespace std

template void std::__linear_insert(CDecalEntry*, CDecalEntry*, CDecalEntry, CDecalEntryLess);