// std::vector<CScriptThing>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport insert of n copies of a script-thing handle at position: within the spare capacity slide the tail up (copy-construct into the raw end, assign backwards, assign x over the gap, or fill the raw end first when the tail is short), else grow through _M_insert_overflow
// Grade: DIFFER(474v480) -- VC7.1 inlines the CScriptThing copy constructor at all three sites (x_copy and both raw-end __uninitialized_copy loops); retail inlines it only for x_copy and calls 0x004ABE90 in the two loops (null-checked placement new kept inline)
// Leaf: ?_M_fill_insert@?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAEXPAVCScriptThing@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D34B30, 480 bytes, thiscall, `ret 0Ch`.
// Identity (proven): the only caller is the landed vector<CScriptThing>::resize 0x00D34AC0 (site 0x00D34B17), which binds this
//   address under exactly this symbol; the 12-byte stride (`imul 2AAAAAABh ; sar 1`), the inline x_copy storing the CScriptThing
//   vtable 0x01238C8C, the out-of-line CScriptThing copy 0x004ABE90 / dtor 0x004AA840 and the landed growth path
//   vector<CScriptThing>::_M_insert_overflow 0x008AD5F0 all name the element. Catalog label `std_vector_InsertRangeWithElement`
//   is behavioural.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (raw-end copies), ~CScriptThing 0x004AA840 (x_copy),
//   CCountedPointer<CScriptThing>::operator= 0x00704580 (the inline CScriptThing::operator= in the backward copy and both fills),
//   uninitialized_fill_n(first, n, x) 0x00D34D10 (__fastcall, `ret 4`), _M_insert_overflow(position, x, __false_type, n, false)
//   0x008AD5F0.
// Shape: STLport 4.6 `_M_fill_insert` verbatim (the landed vector<CFeatDef> 0x00F34660 text) over the retail CScriptThing header
//   (FABLE_RETAIL_NSCRIPT). Retail expands the copy constructor inline for x_copy (one level down) but calls it out of line in the
//   two raw-end loops (three levels down: __uninitialized_copy -> _Construct -> copy). Not reproduced: an inline_depth pragma
//   around this explicit specialization (depths 1 and 2 change nothing, 0 also un-inlines _Construct), a plain-inline
//   __uninitialized_copy, a non-inline same-TU copy body and the QFE 4035 toolset all leave the split unmatched.
inline void* operator new(unsigned int, void* p) { return p; }
class CCPPointerInfo {
public:
    long RefCount;                                      // +0x0
    void (__fastcall* DeleteFunc)(void* data);          // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x0
    CCPPointerInfo* Info;                               // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 for <CScriptThing>
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail's inline bodies (the header contract: defined out of class after the include).
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }   // out of line: 0x004ABE90
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x00D34D10
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__forceinline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                  _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                       _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x008AD5F0
};


template <>
void vector<CScriptThing, allocator<CScriptThing> >::_M_fill_insert(pointer __position, size_type __n, const CScriptThing& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            CScriptThing __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}   // namespace std
