// std::vector::erase(iterator first, iterator last) -- (vector<std::pair<CIntelligentPointer<CThing>, long> >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy each vacated element, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@V?$allocator@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@@2@@std@@QAEPAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@2@PAU32@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080E4E0, 74 bytes (+ int3 padding to 80), thiscall, `ret 8`, returns first.
// Identity (proven): vector<std::pair<CIntelligentPointer<CThing>, long> >; evidence: its only caller is CTCDExplosion::FrameUpdate 0x0080D750, which erases this+0x40 = Ego_r.pdb CTCDExplosion::Emitters, vector<pair<CIntelligentPointer<CThing>, long> > (+64, the members before it have retail sizes); the 12-byte stride and the direct CBaseIntelligentPointer destructor 0x00A01C10 on each element's first member agree; __copy 0x00788F76.
// Label: the manifest's "std_vector_CopyAssign" is propagated across byte-identical bodies (family H009: seven rows that differ only in
//   the element stride and the call displacements; retail has no identical-code folding, so each address is one
//   instantiation); this is STLport 4.6 `vector::erase(iterator, iterator)` (12-byte elements: `add esi,Ch`).
// Callees: std::__copy<std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, int> @ 0x00788F76 (`__fastcall`: last in ecx, finish in edx; first, the
//   `random_access_iterator_tag` reference -- homed in the dead `last` argument slot -- and `(int*)0` on the stack), then
//   the pair's implicit destructor (CBaseIntelligentPointer::~CBaseIntelligentPointer(), called directly) @ 0x00A01C10 per destroyed element (a direct thiscall: the element destructor is not virtual).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` and _Destroy = the __destroy_aux(__false_type) loop of `p->~T()`
//   (agent951 G021's model with a non-virtual destructor).
#include <stddef.h>
class CThing;
class CBaseIntelligentPointer {                     // 8 bytes: vptr + PData
public:
    CBaseIntelligentPointer& operator=(const CBaseIntelligentPointer& other);
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

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
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x00788F76 (same-TU body)
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
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
typedef std::pair<CIntelligentPointer<CThing>, long> _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);
}