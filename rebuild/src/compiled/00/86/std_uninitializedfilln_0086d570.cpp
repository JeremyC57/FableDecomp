// std::uninitialized_fill_n(CTextureMorphMap* __first, unsigned int __n, const CTextureMorphMap& __x) -- STLport front end: construct n copies of one texture-morph map into raw storage (the __false_type body inlined)
// Leaf: ??$uninitialized_fill_n@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086D570, 48 bytes (ends at the next function 0x0086D5A0), __fastcall (first ecx, n edx;
//   the value on the stack, `ret 4`). CTextureMorphMap = CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
//   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > (16 bytes).
// Identity (proven, two sources): (1) the only caller is the landed vector<CTextureMorphMap>::_M_fill_insert 0x0086E4B0
//   (call at 0x0086E543: the `__elems_after <= __n` arm, `uninitialized_fill_n(this->_M_finish, __n - __elems_after,
//   __x_copy)`, three arguments); (2) STLport 4.6 stl/_uninitialized.h: the public three-argument uninitialized_fill_n
//   forwards to the inline __false_type body (`ret 4`: only the value on the stack; chunk 655's ret-4 front-end rule), the
//   element copy being the implicit CVectorMap copy (base vector copy ctor 0x00673E10, empty comparator, Dirty +0x0D).
//   Byte near-twin of 0x0086D090 (the four-argument __uninitialized_fill_n, ret 8, caller _M_insert_overflow): two symbols,
//   each named from its own caller and return. The catalog label "_Fill_n<unsigned long*,...>" is propagated and wrong;
//   the landed 0x0086E4B0 binds this address as the stand-in ?UninitFill@@YIPAUElement_0086e4b0@@... (NAME_FIXES).
// Callee: vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::vector(const vector&) 0x00673E10.
// Shape: STLport 4.6 text (_IS_POD_ITER over iterator_traits / __type_traits: a class type is __false_type, whose user
//   ctor keeps the tag temporary store-free); VC7.1 inlines the __false_type body (its COMDAT is emitted unreferenced).
//   /Gr as the landed siblings of this TU.
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

template <class _Tp> struct __type_traits {                         // STLport 4.6 stl/type_traits.h (a class type is not POD)
    typedef __false_type is_POD_type;
};
template <class _Iterator> struct iterator_traits { };              // STLport 4.6 stl/_iterator_base.h
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};
#define _IS_POD_ITER(_It, _Tp) __type_traits< typename iterator_traits< _Tp >::value_type >::is_POD_type()

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, _IS_POD_ITER(__first, _ForwardIter));
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

template CTextureMorphMap* std::uninitialized_fill_n<CTextureMorphMap*, unsigned int, CTextureMorphMap>(
    CTextureMorphMap*, unsigned int, const CTextureMorphMap&);