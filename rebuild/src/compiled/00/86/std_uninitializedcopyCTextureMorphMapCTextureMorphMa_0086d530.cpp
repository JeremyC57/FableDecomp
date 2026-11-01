// std::__uninitialized_copy<CTextureMorphMap*, CTextureMorphMap*>(CTextureMorphMap* __first, CTextureMorphMap* __last, CTextureMorphMap* __result, const __false_type&) -- STLport copy-construct the texture-morph maps [first, last) into raw storage at result
// Leaf: ??$__uninitialized_copy@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@@std@@YIPAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086D530, 52 bytes, __fastcall (first ecx, last edx; result and the tag reference on the
//   stack, `ret 8`); CTextureMorphMap = CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
//   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > (16 bytes).
// Identity: the callers are the landed vector<CTextureMorphMap>::_M_insert_overflow 0x0086E1C0 (head and tail copies,
//   `__uninitialized_copy(this->_M_start, __position, ...)` -- non-const pointers) and the landed _M_fill_insert 0x0086E4B0
//   (the `__old_finish - __n` tail move); both are instantiated over the per-body-part texture-morph maps (Ego_r.pdb
//   CRandomAppearanceMorph::BodyPartTextureMorphs, FableWin.pdb ?resize@?$vector@V?$CVectorMap@KVCTextureMorph@... of
//   the landed 0x0086EA80). The callee 0x00673E10 is the map base's vector<pair<unsigned long, CTextureMorph> > copy
//   constructor (bound). Already the bound spelling of reloc_bindings.tsv (from 0x0086E1C0); the second binding
//   ?UninitCopy@@YIPAUElement_0086e4b0@@... (from 0x0086E4B0) is a stand-in (NAME_FIXES). The twin 0x00674CE0 is the
//   const-source instantiation __uninitialized_copy<const CTextureMorphMap*, CTextureMorphMap*> (bound PBV; its callers are
//   _M_allocate_and_copy 0x00674EB0 and operator= 0x006753F0, which copy from a const vector), so the two bodies are two
//   COMDATs. The manifest "_Uninit_copy<CFeatDef*,...>" is propagated (CFeatDef is 100 bytes).
// Shape: STLport 4.6 stl/_uninitialized.h verbatim (no EH: _STLP_TRY/_STLP_UNWIND are empty without /EHsc); the implicit
//   CVectorMap copy constructor = the out-of-line vector copy, nothing for the empty comparator, the Dirty byte (+0x0D);
//   `_Construct` is placement new with its null test.
inline void* operator new(unsigned int, void* __p) { return __p; }

namespace std {
struct __false_type { };

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
public:
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
template <class _InputIter, class _ForwardIter>
inline _ForwardIter
__uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

template <class K, class V> class CKeyPairCompareLess {             // empty
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
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

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;

template CTextureMorphMap* std::__uninitialized_copy<CTextureMorphMap*, CTextureMorphMap*>(
    CTextureMorphMap*, CTextureMorphMap*, CTextureMorphMap*, const std::__false_type&);