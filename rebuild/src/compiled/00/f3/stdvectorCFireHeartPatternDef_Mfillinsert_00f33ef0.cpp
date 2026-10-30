// std::vector<CFireHeartPatternDef>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport insert of n copies of a Fire Heart pattern def (0x34-byte CFireHeartPatternDef) at position: within the spare capacity slide the tail up (copy-construct into the raw end, assign backwards, assign x over the gap, or fill the raw end first when the tail is short), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCFireHeartPatternDef@@V?$allocator@VCFireHeartPatternDef@@@std@@@std@@QAEXPAVCFireHeartPatternDef@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F33EF0, 538 bytes, thiscall, `ret 0Ch`; 0x34-byte frame (x_copy).
// Identity (proven): the 0x34 stride (`imul 0x4EC4EC4F ; sar 4` = /52) and the callees are the landed CFireHeartPatternDef
//   helpers -- _Construct 0x00D34EC0 (bound ??$_Construct@VCFireHeartPatternDef@@V1@@std@@YIX...) in the raw-end loops,
//   __copy_backward_ptrs 0x00F34110 (whose Identity names this row's call site 0x00F33FCD), the implicit
//   CFireHeartPatternDef::operator= inlined in both fills (CDefClassBase::operator= 0x00431F10, vector<bool>::operator=
//   0x00462E78 on PadActive +0x28), x_copy's implicit copy built inline (CDefClassBase copy 0x0046644C, vtable 0x012CCC50,
//   vector<bool> copy 0x00D34F00) and destroyed inline (PadActive's block freed, CDefPointeeBase vtable 0x01230BA0,
//   ~CResource) -- and the growth path is the landed vector<CFireHeartPatternDef>::_M_insert_overflow 0x00F34160 (whose
//   Identity names this row, call site 0x00F340FD, as its only caller). Only caller 0x00F33E38, the vector's resize
//   0x00F33DE0. Same STLport text as the landed vector<CFeatDef> / vector<CConversation>::_M_fill_insert rows. The catalog
//   label vector_insert_impl_ThingBuildingDef_map is wrong.
// Callees: CDefClassBase(const CDefClassBase&) 0x0046644C, vector<bool>(const vector&) 0x00D34F00, _Construct 0x00D34EC0
//   (__fastcall), __copy_backward_ptrs 0x00F34110 (__fastcall), CDefClassBase::operator= 0x00431F10, vector<bool>::operator=
//   0x00462E78, _M_insert_overflow(position, x, __false_type, n, false) 0x00F34160, free 0x00BFEA14, ~CResource 0x009FC550.
// Shape: STLport 4.x `_M_fill_insert` verbatim. As in the landed _M_insert_overflow 0x00F34160, retail calls _Construct out of
//   line inside the raw-end loops (the out-of-line __fastcall declaration) while x_copy's implicit copy is expanded in place;
//   __uninitialized_copy, __uninitialized_fill_n (via uninitialized_fill_n) and fill are `__forceinline`; __false_type has
//   no user constructor (retail zero-stores both tags).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    vector(const vector<_Tp, _Alloc>& __x);             // vector<bool>: 0x00D34F00
    ~vector() { }                                       // _Destroy over bool is empty
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // vector<bool>: 0x00462E78
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);      // CFireHeartPatternDef: 0x00F34160
};
}   // namespace std

// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in);
//   CFireHeartPatternDef as the landed _M_insert_overflow 0x00F34160 declares it (no user ctor/dtor/operator= in either PDB).
class CFireHeartPatternDef : public CDefClassBase {     // 0x34 bytes, vtable 0x012CCC50 (no RTTI locator)
public:
    virtual void Transfer(CPersistContext&);            // slot 18 0x00D35070
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00D34FB0
    virtual unsigned long GetSizeofClass();             // slot 20 0x00D34EF0
    std::vector<bool> PadActive;                        // +0x28 (STLport, 12 bytes)
};

namespace std {
template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val);           // 0x00D34EC0
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&);   // 0x00F34110
template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
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

template void vector<CFireHeartPatternDef>::_M_fill_insert(CFireHeartPatternDef*, unsigned int, const CFireHeartPatternDef&);
}   // namespace std