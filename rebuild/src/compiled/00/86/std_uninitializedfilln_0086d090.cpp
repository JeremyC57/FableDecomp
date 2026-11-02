// std::__uninitialized_fill_n(CTextureMorphMap* __first, unsigned int __n, const CTextureMorphMap& __x, const __false_type&) -- STLport construct n copies of one texture-morph map into raw storage
// Leaf: ??$__uninitialized_fill_n@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086D090, 48 bytes (ends at the next function 0x0086D0C0), __fastcall (first ecx, n edx;
//   the value and the tag reference on the stack, `ret 8`). CTextureMorphMap = CVectorMap<unsigned long,
//   CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > (16 bytes).
// Identity (proven, two sources): (1) the only caller is the landed vector<CTextureMorphMap>::_M_insert_overflow 0x0086E1C0
//   (call at 0x0086E246: the `__fill_len != 1` arm, `__uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type())`);
//   the bound spelling of reloc_bindings.tsv from that caller is exactly the symbol this source emits; (2) STLport 4.6
//   stl/_uninitialized.h: the four-argument `__false_type` body, `ret 8` = value + tag on the stack, and the element copy is the
//   implicit CVectorMap copy (the out-of-line base vector<pair<unsigned long, CTextureMorph> > copy constructor 0x00673E10,
//   nothing for the empty comparator +0x0C, the Dirty byte +0x0D) -- the same element as the landed __uninitialized_copy
//   0x0086D530. Byte near-twin 0x0086D570 is the three-argument front end uninitialized_fill_n (ret 4, its own caller
//   _M_fill_insert 0x0086E4B0): two symbols. The catalog label "_Fill_n<unsigned long*,...>" is propagated and wrong.
// Callee: vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::vector(const vector&) 0x00673E10.
// Shape: STLport 4.6 text verbatim (no EH: _STLP_TRY / _STLP_UNWIND are empty without /EHsc); `__n > 0` on the unsigned size
//   is the `test edx,edx ; jbe` entry and `dec edi ; jne` loop; `_Construct` is placement new with its null test.
//   /Gr as the landed siblings of this TU (0x0086D530, 0x0086E1C0, 0x0086E4B0).
inline void* operator new(unsigned int, void* __p) { return __p; }

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {                       // STLport pair
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector(const vector<_Tp, _Alloc>& __x);                         // out of line (0x00673E10 for pair<ulong, CTextureMorph>)
};

// STLport 4.6 stl/_construct.h
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) {
    new (__p) _T1(__val);
}

// STLport 4.6 stl/_uninitialized.h
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
__uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

template <class K, class V> class CKeyPairCompareLess {             // empty
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                                   // +0x0C
    bool Dirty;                                                     // +0x0D
};

class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                           // Ego_r.pdb, POD
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
};

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> CTextureMorphMap;

template CTextureMorphMap* std::__uninitialized_fill_n<CTextureMorphMap*, unsigned int, CTextureMorphMap>(
    CTextureMorphMap*, unsigned int, const CTextureMorphMap&, const std::__false_type&);