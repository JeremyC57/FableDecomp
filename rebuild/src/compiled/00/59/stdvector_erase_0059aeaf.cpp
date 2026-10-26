// std::vector::erase(iterator position) -- (vector<CDateAndTime >) STLport single-element erase: when position is not the last element, assign the tail [position + 1, finish) down onto it through the out-of-line __copy; drop finish by one element (trivial destructor); return position
// Leaf: ?erase@?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@std@@QAEPAVCDateAndTime@@PAV3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059AEAF, 42 bytes (+ int3 padding to 44), thiscall, `ret 4`, returns position.
// Identity (proven): vector<CDateAndTime >::erase(iterator); evidence: its only caller CFrontEndManager::GetSavedGameNames 0x00597DF7 erases this+0x68, the vector it fills with 7-dword records next to the CCharString vectors +0x74 / +0x80 and the long vector +0x8C: Ego_r.pdb CFrontEndManager::m_vFileTimes (vector<CDateAndTime>), FileRegionNames, FileRegionMiniMapNames, FilePlayTimes; 28-byte stride; its __copy 0x0040E350 is bound as __copy<CDateAndTime*> from CUserProfileManager::LoadFileList 0x004091C0.
// Label: the manifest's "GetNewTreeNode" is propagated across byte-identical bodies (family H014: seven rows that differ only in
//   the element stride (here 28 bytes: `lea ecx,[edi+1Ch]`, `add [esi+4],-1Ch`) and the __copy call; retail has
//   no identical-code folding, so each address is one instantiation); this is STLport 4.6 `vector::erase(iterator)`.
// Callees: std::__copy<CDateAndTime*, CDateAndTime*, int> @ 0x0040E350 (`__fastcall`: position + 1 in ecx, finish in edx; position, the
//   `random_access_iterator_tag` reference -- homed in the dead high byte of the position argument slot -- and
//   `(int*)0` on the stack).
// Body: STLport 4.6 `if (__position + 1 != end()) __copy_ptrs(__position + 1, _M_finish, __position, _TrivialAss());
//   --_M_finish; _Destroy(_M_finish); return __position;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)`; __copy is a same-TU noinline body (retail's sits a few hundred bytes
//   earlier in the same TU).
// Pragma: optimize("s", on) + optimize("y", off) -- a frame-pointer size TU (`push ebp ; mov ebp,esp`).
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <stddef.h>
class CDateAndTime {                                // 28 bytes (Ego_r.pdb)
public:
    long Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
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
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x0040E350 (same-TU body)
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
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, _TrivialAss());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};
}
typedef CDateAndTime TElem;
template std::vector<TElem>::iterator std::vector<TElem>::erase(std::vector<TElem>::iterator);