// std::vector<CArenaCreatureDef>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport insert of n copies of an arena creature def (0x38-byte CArenaCreatureDef) at position: within the spare capacity slide the tail up (copy-construct into the raw end, assign backwards, assign x over the gap, or fill the raw end first when the tail is short), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCArenaCreatureDef@@V?$allocator@VCArenaCreatureDef@@@std@@@std@@QAEXPAVCArenaCreatureDef@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26570, 456 bytes, thiscall, `ret 0Ch`; 0x38-byte frame (x_copy).
// Identity (proven): the 0x38 stride (`imul 0x92492493 ; add ; sar 5` = /56) and the callees are the landed CArenaCreatureDef
//   members -- copy constructor 0x00F25DD0 (??0CArenaCreatureDef@@QAE@ABV0@@Z) for x_copy and the raw-end copies, operator=
//   0x00F25F10 (??4CArenaCreatureDef@@QAEAAV0@ABV0@@Z) in the backward copy and both fills; x_copy's inline implicit
//   destructor destroys HUDType (+0x30) and CreatureType (+0x28), exactly the out-of-line copy 0x00F25E40 behind the def's
//   vtable 0x012FB358. The growth path 0x00F26770 is vector<CArenaCreatureDef>::_M_insert_overflow (catalog) and the fill
//   0x00F26740 uninitialized_fill_n (__fastcall, `ret 4`). The only caller is 0x00F264ED, the CArenaCreatureDef vector's
//   resize (insert(end(), n - size(), x) inlined to this). Same STLport text as the landed vector<CFeatDef> /
//   vector<CConversation>::_M_fill_insert rows. The catalog label vector_insert_impl_CParticleMorphs is wrong.
// Callees: CArenaCreatureDef(const CArenaCreatureDef&) 0x00F25DD0, CArenaCreatureDef::operator= 0x00F25F10,
//   uninitialized_fill_n(first, n, x) 0x00F26740, _M_insert_overflow(position, x, __false_type, n, false) 0x00F26770,
//   ~CCharString 0x0099EAE0 and CResource::~CResource 0x009FC550 (x_copy's inline dtor).
// Shape: STLport 4.x `_M_fill_insert` verbatim, the CConversation model. x_copy's destructor is inline: CArenaCreatureDef and
//   CDefClassBase have compiler-generated destructors (no vptr store of their own), CDefPointeeBase's user-written inline
//   destructor stores its vtable 0x01230BA0, then ~CResource is called out of line.
inline void* operator new(unsigned int, void* p) { return p; }
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in);
//   CArenaCreatureDef as the landed CArenaCreatureDef::Transfer 0x00F25E70 declares it (Ego_r.pdb members, 0x38 bytes).
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CArenaCreatureDef : public CDefClassBase {       // vtable 0x012FB358 (no RTTI), 0x38 bytes
public:
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F25E70
    virtual void Copy(const CDefClassBase* other);      // slot 19 0x00F25F00
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F25E10
    CCharString CreatureType;                           // +0x28
    long        NumCreatures;                           // +0x2c
    CCharString HUDType;                                // +0x30
    long        DeathScore;                             // +0x34
    CArenaCreatureDef(const CArenaCreatureDef& other);  // 0x00F25DD0
    CArenaCreatureDef& operator=(const CArenaCreatureDef& other);   // 0x00F25F10
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
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x00F26740
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
                            bool __atend = false);                               // 0x00F26770
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

template void vector<CArenaCreatureDef>::_M_fill_insert(CArenaCreatureDef*, unsigned int, const CArenaCreatureDef&);
}   // namespace std