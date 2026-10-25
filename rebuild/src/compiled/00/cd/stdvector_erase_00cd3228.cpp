// std::vector::erase(iterator first, iterator last) -- (vector<CScriptGameResourceObjectScriptedThingBase >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy the vacated tail through the out-of-line __destroy_aux, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCScriptGameResourceObjectScriptedThingBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3228, 55 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 8`, returns first.
// Identity: std::vector<CScriptGameResourceObjectScriptedThingBase >::erase(iterator, iterator) -- proven; evidence: its only caller is vector::resize(n, x) 0x00CD3CCB (n < size(): erase(begin + n, end)), reached from resize(n) 0x00CD3DA4, which builds its default element with the constructor 0x007E72A0 (CBaseObject base 0x0099A380, then vtable 0x0127094C = RTTI CScriptGameResourceObjectScriptedThingBase, PImp {0, 0} at +8) and destroys it with 0x007E74D0; the 0x10-byte element's operator= 0x008ABD10 assigns the CBaseObject part (0x0099A3B0, copies nothing) then the CCountedPointer PImp at +8 / +0xC, and its destructor is virtual; the PDB name sets hold vector<CScriptGameResourceObjectScriptedThingBase>::erase; the catalogue labels of 0x00CD3DA4 ("CPersistContext::Transfer<CAnimationSet>") and 0x00CD3CCB ("CArray<std::pair<long..>>::push_back") are propagated
//   Family G072 (seven byte-identical bodies, one COMDAT per T: retail has no ICF). Catalogue label "std_vector_CopyElement" is propagated/behavioural.
// Callees: std::__copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00CD2BF1 (`__fastcall`: first in
//   ecx, last in edx, result / tag reference / null distance pointer on the stack, `ret 0Ch`; 0x10-byte elements, the element's
//   operator= 0x008ABD10); std::__destroy_aux<T*>(first, last, const __false_type&) 0x00CD2725 (`__fastcall`, `ret 4`; the element's virtual destructor, slot 0).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the family G067 body with __copy left out of line: both callees are same-TU noinline
//   `__fastcall` bodies that never touch their tag, so VC7.1 homes both tags in the live `first` slot ([ebp+0Bh]) as retail does.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
template <class T> class CCountedPointer {                   // 8 bytes: the object + its shared CCPPointerInfo count block
public:
    T*    Data;                                              // +0x0
    void* Info;                                              // +0x4 CCPPointerInfo*
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other);
    ~CCountedPointer();
};
class CBaseObjectPointer;
class CBaseObject {                                          // vtable 0x0129A7D8, 8 bytes
public:
    virtual ~CBaseObject();
    CBaseObjectPointer* IntelligentPointer;                  // +0x04
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject {   // vtable 0x0127094C, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;    // +0x08
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);
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
template <class _RandomAccessIter, class _OutputIter, class _Distance>                   // 0x00CD2BF1 (same-TU body)
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
template <class _ForwardIterator>                                                        // 0x00CD2725 (same-TU body)
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

typedef CScriptGameResourceObjectScriptedThingBase _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);

} // namespace std