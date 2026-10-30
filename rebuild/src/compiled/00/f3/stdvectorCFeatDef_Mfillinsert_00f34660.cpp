// std::vector<CFeatDef>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport insert of n copies of a feat def (0x64-byte CFeatDef) at position: within the spare capacity slide the tail up (copy-construct into the raw end, assign backwards, assign x over the gap, or fill the raw end first when the tail is short), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCFeatDef@@V?$allocator@VCFeatDef@@@std@@@std@@QAEXPAVCFeatDef@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F34660, 455 bytes, thiscall, `ret 0Ch`; 0x64-byte frame (x_copy).
// Identity (proven): the 0x64 stride (`imul 0x51EB851F ; sar 5` = /100) and the callees are the landed CFeatDef members --
//   copy constructor 0x00F2D270 (??0CFeatDef@@QAE@ABV0@@Z) for x_copy and the raw-end copies, operator= 0x00F2D200
//   (??4CFeatDef@@QAEAAV0@ABV0@@Z) in the backward copy and both fills -- and the growth path is the landed
//   vector<CFeatDef>::_M_insert_overflow 0x00F34860; the out-of-line fill is uninitialized_fill_n 0x00F34830 (__fastcall,
//   `ret 4`). The only caller is vector<CFeatDef>::resize 0x00F34580 (`call` at 0x00F345D8). Same STLport text as the landed
//   vector<CConversation>::_M_fill_insert 0x00F33810 and vector<CArenaRoundDef>::_M_fill_insert 0x00F34D70. The catalog label
//   vector_insert_impl_CFeatDef is behavioural.
// Callees: CFeatDef(const CFeatDef&) 0x00F2D270, CFeatDef::operator= 0x00F2D200, uninitialized_fill_n(first, n, x) 0x00F34830,
//   _M_insert_overflow(position, x, __false_type, n, false) 0x00F34860, CResource::~CResource 0x009FC550 (x_copy's inline dtor).
// Shape: STLport 4.x `_M_fill_insert` verbatim, the CConversation model. x_copy's destructor is inline: CFeatDef and
//   CDefClassBase have compiler-generated destructors (no vptr store of their own, as every retail def dtor 0x00F2D300 /
//   0x0042DAB0 shows), CDefPointeeBase's user-written inline destructor stores its vtable 0x01230BA0, then ~CResource is
//   called out of line.
inline void* operator new(unsigned int, void* p) { return p; }
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in); CFeatDef is
//   the Ego_r.pdb 0x64-byte def (vtable 0x012FB778, no RTTI; members opaque here, see the landed CFeatDef::Transfer 0x00F2D330).
class CFeatDef : public CDefClassBase {                 // 0x64 bytes; vtable 0x012FB778 (no RTTI)
public:
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F2D330
    virtual void Copy(const CDefClassBase* other);      // slot 19 0x00F2D680
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F2D2F0
    unsigned long _members[0x0f];                       // +0x28 FeatName .. CO_ItemName (opaque here; 4-aligned after the packed 0x25-byte base)
    CFeatDef(const CFeatDef& other);                    // 0x00F2D270
    CFeatDef& operator=(const CFeatDef& other);         // 0x00F2D200
};

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
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x00F34830
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
                            bool __atend = false);                               // 0x00F34860
};

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

template void vector<CFeatDef>::_M_fill_insert(CFeatDef*, unsigned int, const CFeatDef&);
}   // namespace std