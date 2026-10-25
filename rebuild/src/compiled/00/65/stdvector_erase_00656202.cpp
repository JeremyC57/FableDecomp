// std::vector::erase(iterator first, iterator last) -- (vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy the vacated tail through the out-of-line __destroy_aux, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656202, 55 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 8`, returns first.
// Identity: std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::erase(iterator, iterator) -- proven; evidence: its callers are 0x0064CCB3, reached from CPlayerGui::DrawPreparePrimitivesForEngine through [CPlayerGui+0x148] = PDrawStamina.Data (Ego_r.pdb +0x150), which erases [begin, end) of the vector at +0x34 after testing PreviousMaxStamina +0x44, the clear() 0x006567CF (the EH cleanup 0x00657037 of the CDrawHealth / CDrawStamina / CDrawAbilityButtons / CDrawQuickAccessIcons / CDrawMeleeActionButtonEffects / CStatBase constructors, whose Ego_r.pdb Effects members all are this vector) and the resize 0x00656F6D; Ego_r.pdb CDrawStamina::IncreaseEffects (+0x3C) is vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >; the 8-byte element's operator= 0x0065533C is GetPItem 0x00A01B50 + SetPItem 0x00A01B90 and its destructor is virtual; the PDB name sets hold vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::erase
//   Family G072 (seven byte-identical bodies, one COMDAT per T: retail has no ICF). Catalogue label "std::vector_assign_bodypartmesh" is propagated/behavioural.
// Callees: std::__copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x006557FB (`__fastcall`: first in
//   ecx, last in edx, result / tag reference / null distance pointer on the stack, `ret 0Ch`; 0x8-byte elements, the element's
//   operator= 0x0065533C); std::__destroy_aux<T*>(first, last, const __false_type&) 0x0043F850 (`__fastcall`, `ret 4`; the element's virtual destructor, slot 0).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the family G067 body with __copy left out of line: both callees are same-TU noinline
//   `__fastcall` bodies that never touch their tag, so VC7.1 homes both tags in the live `first` slot ([ebp+0Bh]) as retail does.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
namespace NParticleEngine { class CParticleEmitter; }
class CBaseObjectPointer;
class CBaseIntelligentPointer {                              // 8 bytes (Ego_r.pdb): vptr, PData
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
    CIntelligentPointer<T>& operator=(const CIntelligentPointer<T>& other);   // GetPItem 0x00A01B50 + SetPItem 0x00A01B90
};

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

#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>                   // 0x006557FB (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                  const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>                                                        // 0x0043F850 (same-TU body)
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

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

typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);

} // namespace std