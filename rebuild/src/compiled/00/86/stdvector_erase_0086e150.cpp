// std::vector::erase(iterator first, iterator last) -- (vector<CRandomAppearanceMorph::CBodyPartMesh >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, free each vacated element's owned block (member at +0x8), set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@V?$allocator@VCBodyPartMesh@CRandomAppearanceMorph@@@std@@@std@@QAEPAVCBodyPartMesh@CRandomAppearanceMorph@@PAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086E150, 99 bytes (+ int3 padding to 112), thiscall, `ret 8`, returns first.
// Identity (proven): std::vector<CRandomAppearanceMorph::CBodyPartMesh >::erase(iterator, iterator); evidence: CRandomAppearanceMorph::TransferBinaryIn 0x0086BFB0 (Ego_r.pdb name) shrinks a body-part array with it at 0x0086C0C5 (the inline resize(n) builds its default entry as 24 zero bytes plus the Dirty byte at +0x15, i.e. MeshIndex, Priority and a CVectorMap at +8 -- Ego_r.pdb CRandomAppearanceMorph::BodyParts is CArray<CArray<CRandomAppearanceMorph::CBodyPartMesh> >), and so does vector<CBodyPartMesh>::resize(n, x) 0x0086E9F0 (`/24` element count); its __copy 0x0086DC50 is landed as std::__copy<CRandomAppearanceMorph::CBodyPartMesh*, ...>; FableWin.pdb holds ?erase@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@.
// Label: the manifest's "std::vector::Reallocate_CEngineTextureMorph_with_free" is propagated (family J010: six erase bodies whose element owns one malloc block in a
//   member past +0 -- `lea esi, [ebx + 8h]` / `add esi, 18h`; retail has no identical-code folding, so each
//   address is one element type).
// Callees: std::__copy<CRandomAppearanceMorph::CBodyPartMesh*, CRandomAppearanceMorph::CBodyPartMesh*, int> 0x0086DC50 (`__fastcall`: last in ecx, finish in edx; first, the
//   `random_access_iterator_tag` reference and `(int*)0` on the stack; retail homes the empty tag in the LIVE `first`
//   slot, which VC7.1 does only when it sees the same-TU __copy body never touches the tag, so the body is given here),
//   free 0x00BFEA14 (each vacated element's inlined member `_Vector_base` destructor).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` and _Destroy = the __destroy_aux(__false_type) loop of `p->~T()`.
// Model: family G041 (agent951; same body with the block at +0).
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
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x0086DC50 (same-TU body)
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
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {                                                // 16 bytes in retail
public:
    CArray<std::pair<K, V> > Data;                                // +0x00
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
    class CBodyPartMesh {                                         // 24 bytes in retail (Ego_r.pdb members)
    public:
        long MeshIndex;                                           // +0x00
        long Priority;                                            // +0x04
        CVectorMap<unsigned long, CTextureMorph> TextureMorphs;   // +0x08
    };
};
template std::vector<CRandomAppearanceMorph::CBodyPartMesh >::iterator std::vector<CRandomAppearanceMorph::CBodyPartMesh >::erase(std::vector<CRandomAppearanceMorph::CBodyPartMesh >::iterator, std::vector<CRandomAppearanceMorph::CBodyPartMesh >::iterator);