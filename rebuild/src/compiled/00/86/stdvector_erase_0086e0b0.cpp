// std::vector::erase(iterator first, iterator last) -- (vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, free each vacated element's block, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086E0B0, 84 bytes (+ int3 padding), thiscall, `ret 8`, returns first.
// Identity: vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > >::erase(iterator, iterator) -- proven; evidence: the landed vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::resize(unsigned int) 0x0086EA80 (FableWin.pdb name; CRandomAppearanceMorph::BodyPartTextureMorphs.resize(3) in the landed constructor 0x0086C4D0) shrinks through it, as does the operator= 0x0086E950; the catalog binds this exact STLport name here.
// Label: the manifest's "std::vector::std::vector::Reallocate_copy_with_free" is propagated across byte-identical bodies; family G041 (8 erase bodies whose
//   16-byte element owns a malloc block at +0: `add esi,10h`).
// Callees: std::__copy<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >*, CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >*, int> 0x0086DC10 (`__fastcall`: last in ecx, finish in edx; first, the
//   `random_access_iterator_tag` reference and `(int*)0` on the stack; retail homes the empty tag in the LIVE `first`
//   slot, which VC7.1 does only when it sees the same-TU __copy body never touches the tag, so the body is given here),
//   free 0x00BFEA14 (each vacated element's inlined `_Vector_base` destructor).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` and _Destroy = the __destroy_aux(__false_type) loop of `p->~T()`.
#include <stddef.h>
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x0086DC10 (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class T> class CArray : public std::vector<T> { };    // retail CArray: an STLport vector (12 bytes)
template <class K, class V> class CKeyPairCompareLess {      // empty
public:
    CKeyPairCompareLess() { }
};
template <class K, class V, class C> class CVectorMap : public CArray<std::pair<K, V> > {   // 16 bytes in retail
public:
    C    Compare;                                                 // +0x0C
    bool Dirty;                                                   // +0x0D
};
class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                         // Ego_r.pdb, POD
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
};
template std::vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > >::iterator std::vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > >::erase(std::vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > >::iterator, std::vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > >::iterator);