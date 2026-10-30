// std::vector<CScriptThing>::erase(iterator position) -- STLport single erase: shift the tail down by one (counted-pointer assignment), destroy the old last element through its virtual destructor, return position
// Leaf: ?erase@?$vector@VCScriptThing@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3152, 51 bytes, thiscall, `ret 4`, returns position in eax; built in
//   a size-optimised frame-pointer TU (`push ebp ; mov ebp,esp`, no alignment padding, `add [esi+4],-0Ch` in memory)
//   -- `#pragma optimize("s", on)` + `#pragma optimize("y", off)`.
// Identity: the manifest's "std::vector::pop_back" is propagated. The landed
//   NScript::CQ_OrchardFarmRaidScript::CArtefact::OnPredicateFail 0x00DCF920 declares `CScriptThing* erase(CScriptThing*)`
//   @ 0x00CD3152 on its vector<CScriptThing>; the landed __copy 0x008AC2A0 (CScriptThing, `__fastcall`) lists 0x00CD316D
//   as a caller. CScriptThing (Ego_r.pdb, 12 bytes) = { vptr; CCountedPointer<CScriptThing> PImp; }.
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) 0x008AC2A0 (`__fastcall`), the element's
//   virtual destructor (slot 0, called with 0).
// Shape: STLport 4.6 `if (position + 1 != end()) __copy_ptrs(position + 1, finish, position, _TrivialAss());
//   --finish; _Destroy(finish); return position;` with `__copy_ptrs(.., __false_type)` inline as the forward
//   `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)`: the empty tag temporary lands in the top
//   byte of the dead `position` argument slot (`lea eax,[ebp+0Bh]`); _Destroy is the explicit `~CScriptThing()`.
#pragma optimize("s", on)
#pragma optimize("y", off)

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo { public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void*);                   // +0x04
    void* Data;                                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x008AC2A0

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position);
};

template <class _Tp, class _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::erase(iterator __position)
{
    if (__position + 1 != end())
        __copy_ptrs(__position + 1, this->_M_finish, __position, __false_type());
    --this->_M_finish;
    _Destroy(this->_M_finish);
    return __position;
}

template vector<CScriptThing>::iterator vector<CScriptThing>::erase(iterator);

} // namespace std