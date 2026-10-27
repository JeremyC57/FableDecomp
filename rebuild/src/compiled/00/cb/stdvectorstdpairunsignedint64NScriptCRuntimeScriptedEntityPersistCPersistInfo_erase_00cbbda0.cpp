// std::vector<std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>>::erase(iterator first, iterator last) -- STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy each vacated element, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@U?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBBDA0, 77 bytes, thiscall, `ret 8`, returns first.
// Identity (proven): the landed CVectorMap<unsigned __int64, CPersistInfo>::Clear 0x00CBC660 and NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript 0x00CB8440 call it as the PersistScripts vector's erase(first, last); Ego_r.pdb CPersistInfo {ulong64 UID; CCharString PersistScript} makes the 24-byte pair with the string at +0x10.
// Callees: std::__copy 0x00CB9C80 (landed as the pair<unsigned __int64, CPersistInfo> __copy), CCharString::~CCharString 0x0099EAE0 at +0x10 (second.PersistScript).
// Shape (family K021, representative 0x0074FB20): STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first,
//   _TrivialAss()); _Destroy(__i, _M_finish); _M_finish = __i; return __first;` -- __copy is the /Gr `__fastcall` out-of-line
//   template (last in ecx, finish in edx; first, the tag reference in a dead argument slot and `(ptrdiff_t*)0` on the stack),
//   then the __destroy_aux loop of direct destructor calls (0x18-byte stride). The landed H009 erase 0x00AD7C20 model.
#include <stddef.h>
class CCharString {
public:
    void* PData;                                         // +0x00
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
    ~CCharString();                                      // 0x0099EAE0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;
};
}
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {                                 // Ego_r.pdb, 16 bytes (8-aligned)
    public:
        unsigned __int64 UID;                            // +0x00
        CCharString PersistScript;                       // +0x08
    };
};
}
typedef std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> _Elem;   // 24 bytes

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
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x00CB9C80 (same-TU body)
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
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);
} // namespace std