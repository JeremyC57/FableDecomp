// std::vector<CConversation>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport insert of n copies of a conversation def (0x5C-byte CConversation) at position: within the spare capacity slide the tail up (copy-construct into the raw end, assign backwards, assign x over the gap, or fill the raw end first when the tail is short), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCConversation@@V?$allocator@VCConversation@@@std@@@std@@QAEXPAVCConversation@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F33810, 450 bytes, thiscall, `ret 0Ch`; 0x5C-byte frame (x_copy).
// Identity (proven): the 0x5C stride and the callees are the landed CConversation members -- copy constructor 0x00E54CA0
//   (??0CConversation@@QAE@ABV0@@Z) for x_copy and the raw-end copies, operator= 0x00E54C50 (??4CConversation@@QAEAAV0@ABV0@@Z)
//   in the backward copy and both fills, the complete destructor 0x00E54D80 (??1CConversation@@UAE@XZ) for x_copy -- and the
//   growth path is the landed vector<CConversation>::_M_insert_overflow 0x00F33A10 (whose Identity names this row as its only
//   caller); the out-of-line fill is the STLport uninitialized_fill_n 0x00F339E0 (__fastcall, `ret 4`). The only caller is
//   vector<CConversation>::resize 0x00F33730 (`call` at 0x00F3378D, insert(end(), n - size(), x) inlined to this). Same
//   STLport text as the landed vector<CWideString>::_M_fill_insert 0x004126E0. The catalog label
//   vector_insert_impl_CConversation is behavioural; the bound ?Insert@CRetailVector_elem92@@... is invented. Ego_r.pdb
//   (Dinkumware) has no STLport _M_fill_insert spelling, so the symbol follows the landed STLport vector rows.
// Callees: CConversation(const CConversation&) 0x00E54CA0, CConversation::operator= 0x00E54C50, ~CConversation 0x00E54D80,
//   uninitialized_fill_n(first, n, x) 0x00F339E0, _M_insert_overflow(position, x, __false_type, n, false) 0x00F33A10.
// Shape: STLport 4.x `_M_fill_insert` verbatim. Unlike 0x004126E0, __uninitialized_copy, __copy_backward_ptrs /
//   __copy_backward and fill are `__forceinline` (retail inlines all three: the raw-end copies with the placement null test,
//   the counted backward assign loop, the two forward assign loops); uninitialized_fill_n is a declaration (out of line,
//   result unused). __false_type has no user constructor: retail zero-stores the tag (`mov byte [esp+84h], 0`) on the
//   overflow call. Matched on the first probe.
inline void* operator new(unsigned int, void* p) { return p; }
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, 0x28 bytes, vtable 0x01230BCC); CConversation as
//   the landed vector<CConversation>::operator= 0x00E54AA0 declares it.
class CConversation : public CDefClassBase {            // 0x5C bytes in retail (Ego_r.pdb 108: Dinkumware vectors)
public:
    virtual ~CConversation();                           // slot 0 0x00E54D60 (scalar deleting); complete dtor 0x00E54D80
    virtual void Transfer(CPersistContext&);            // slot 18 0x00E579E0
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00E54D10
    virtual unsigned long GetSizeofClass();             // slot 20 0x00E54D00
    long Lines;                                         // +0x28
    unsigned char Speaker[0x0c];                        // +0x2c vector<CCharString> Speaker (opaque here)
    unsigned char Dialogue[0x0c];                       // +0x38 vector<CCharString> Dialogue
    unsigned char Animation[0x0c];                      // +0x44 vector<CCharString> Animation
    unsigned char AnimLoop[0x0c];                       // +0x50 vector<CCharString> AnimLoop
    CConversation(const CConversation& other);          // 0x00E54CA0
    CConversation& operator=(const CConversation& other);   // 0x00E54C50
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
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x00F339E0
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
                            bool __atend = false);                               // 0x00F33A10
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

template void vector<CConversation>::_M_fill_insert(CConversation*, unsigned int, const CConversation&);
}   // namespace std