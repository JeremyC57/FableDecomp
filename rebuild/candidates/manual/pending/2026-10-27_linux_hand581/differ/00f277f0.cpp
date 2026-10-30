// std::vector<CHeroSoulsCreatureDef>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport insert of n copies of a hero-souls creature def (0x34-byte CHeroSoulsCreatureDef) at position: within the spare capacity slide the tail up (copy-construct into the raw end, assign backwards, assign x over the gap, or fill the raw end first when the tail is short), else grow through _M_insert_overflow
// Grade: DIFFER(601v493) -- VC7.1 inlines the element copy constructor at all three sites (x_copy and both raw-end __uninitialized_copy loops); retail inlines it only for x_copy and calls it out of line in the loops (placement null test inline)
// Leaf: ?_M_fill_insert@?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAEXPAVCHeroSoulsCreatureDef@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F277F0, 493 bytes, thiscall, `ret 0Ch`; 0x34-byte frame (x_copy).
// Identity (proven): the 0x34 stride (`imul 0x4EC4EC4F ; sar 4` = /52), x_copy built inline exactly as the landed
//   CHeroSoulsCreatureDef copy constructor 0x00F27060 (CDefClassBase copy 0x0046644C, vtable 0x012FB500, CreatureType +0x28 and
//   Location +0x2C by CCharString's copy 0x0099EC30, NumCreatures +0x30) and destroyed inline (two ~CCharString, the
//   CDefPointeeBase vtable 0x01230BA0, ~CResource); the raw-end copies call 0x00F27060 and the assignments the landed
//   CHeroSoulsCreatureDef::operator= 0x00F27190. The growth path is the landed vector<CHeroSoulsCreatureDef>::_M_insert_overflow
//   0x00F27A10 and the fill 0x00F279E0 uninitialized_fill_n (__fastcall, `ret 4`). Only caller 0x00F27768, the vector's resize
//   0x00F27710. Same STLport text as the landed vector<CFeatDef> / vector<CConversation>::_M_fill_insert rows. The catalog
//   label vector_insert_impl_pair_CSymbolInfo is wrong.
// Callees: CDefClassBase(const CDefClassBase&) 0x0046644C, CCharString(const CCharString&) 0x0099EC30 / ~ 0x0099EAE0,
//   CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef&) 0x00F27060, CHeroSoulsCreatureDef::operator= 0x00F27190,
//   uninitialized_fill_n 0x00F279E0, _M_insert_overflow(position, x, __false_type, n, false) 0x00F27A10, ~CResource 0x009FC550.
// Shape: STLport 4.x `_M_fill_insert` verbatim, the CConversation model, with CHeroSoulsCreatureDef's compiler-generated copy
//   constructor and destructor.
inline void* operator new(unsigned int, void* p) { return p; }
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in);
//   CHeroSoulsCreatureDef members as the landed copy constructor 0x00F27060 declares them (Ego_r.pdb names).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator { };
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
};
struct __false_type;
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);      // CHeroSoulsCreatureDef: 0x00F27A10
};
}   // namespace std
class CHeroSoulsCreatureDef : public CDefClassBase {   // vtable 0x012FB500 (no RTTI), 0x34 bytes
public:
    virtual void Transfer(CPersistContext&);            // slot 18 0x00F27100
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00F27180
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F270A0
    CCharString CreatureType;                           // +0x28
    CCharString Location;                               // +0x2C
    long        NumCreatures;                           // +0x30
    CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef& other);   // 0x00F27190
};

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
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
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x00F279E0
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

template void vector<CHeroSoulsCreatureDef>::_M_fill_insert(CHeroSoulsCreatureDef*, unsigned int, const CHeroSoulsCreatureDef&);
}   // namespace std
