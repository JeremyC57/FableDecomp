// std::vector::erase(iterator first, iterator last) -- (vector<std::pair<CCharString, CTCBase::CRegisteredTCInfo> >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy the vacated tail through the out-of-line __destroy_aux, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@U?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1B5D, 55 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 8`, returns first.
// Identity: std::vector<std::pair<CCharString, CTCBase::CRegisteredTCInfo> >::erase(iterator, iterator) -- proven; evidence: its callers are the component-registration initialiser 0x004EE23F (catalogue "Init_RegisterGameComponents"), which empties the global 0x013BA89C before refilling it through push_back 0x004E40C3 with pair(name, CRegisteredTCInfo) elements -- the registered-TC CVectorMap<CCharString, CTCBase::CRegisteredTCInfo> the landed CTCBase::GetPTCInfo 0x00686F60 searches -- and the clear() 0x004E3477; the 0x10-byte element's operator= 0x004DA56A is CCharString::operator= 0x0099EFB0 then CRegisteredTCInfo's 0x004D78C5 at +4; the PDB name sets hold vector<pair<CCharString, CTCBase::CRegisteredTCInfo> >::erase
//   Family G072 (seven byte-identical bodies, one COMDAT per T: retail has no ICF). Catalogue label "CArray_CopyAssign" is propagated/behavioural.
// Callees: std::__copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x004DBDE1 (`__fastcall`: first in
//   ecx, last in edx, result / tag reference / null distance pointer on the stack, `ret 0Ch`; 0x10-byte elements, the element's
//   operator= 0x004DA56A); std::__destroy_aux<T*>(first, last, const __false_type&) 0x004DBE8E (`__fastcall`, `ret 4`; the pair's destructor 0x004D6902: ~CCharString 0x0099EAE0 on Name (+0xC) then first (+0)).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the family G067 body with __copy left out of line: both callees are same-TU noinline
//   `__fastcall` bodies that never touch their tag, so VC7.1 homes both tags in the live `first` slot ([ebp+0Bh]) as retail does.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
class CThing;
class CCharString {                                          // 4 bytes (Ego_r.pdb)
public:
    void* PStringData;                                       // +0x0 CCharStringData*
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
    ~CCharString();                                          // 0x0099EAE0
};
class CTCBase {
public:
    class CRegisteredTCInfo {                                // 12 bytes (Ego_r.pdb)
    public:
        CTCBase*      (__cdecl* AllocFunc)(CThing*);         // +0x00
        unsigned long DriverType;                            // +0x04
        CCharString   Name;                                  // +0x08
    };
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
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
template <class _RandomAccessIter, class _OutputIter, class _Distance>                   // 0x004DBDE1 (same-TU body)
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
template <class _ForwardIterator>                                                        // 0x004DBE8E (same-TU body)
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

typedef pair<CCharString, CTCBase::CRegisteredTCInfo> _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);

} // namespace std