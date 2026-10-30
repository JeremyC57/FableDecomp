// std::vector::_M_fill_insert(CEnvironmentThemeDef* __position, unsigned int __n, const CEnvironmentThemeDef& __x) -- (vector<CEnvironmentThemeDef>) STLport insert of n copies of x
// Leaf: ?_M_fill_insert@?$vector@VCEnvironmentThemeDef@@V?$allocator@VCEnvironmentThemeDef@@@std@@@std@@QAEXPAVCEnvironmentThemeDef@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00476FF7, 370 bytes, thiscall, `ret 0Ch` (frame-pointer size module).
// Identity (probable): its only caller is 0x00476F90 (the vector's resize), and every element helper it calls is bound to
//   CEnvironmentThemeDef (landed): copy constructor 0x0047318D, operator= 0x00473046, the vector's uninitialized_fill_n 0x0047725F and
//   _M_insert_overflow 0x00477169; stride 0xC4 = retail sizeof(CEnvironmentThemeDef).
// Callees: CEnvironmentThemeDef(const CEnvironmentThemeDef&) 0x0047318D, CEnvironmentThemeDef::operator= 0x00473046, uninitialized_fill_n<CEnvironmentThemeDef*, unsigned int, CEnvironmentThemeDef> 0x0047725F
//   (__fastcall), ~CParentDefClassBase 0x00430300 (the x copy, ICF-folded def destructor), _M_insert_overflow 0x00477169.
// Shape: the landed family M065 model (0x00476B67 / 0x004766C2): STLport vector.c `_M_fill_insert`, helpers `__forceinline`,
//   `optimize("s", on)` (no `y off` here, see below), a record with out-of-line copy and assignment; the x copy's inline destructor
//   (novtable record) leaves only the base destructor call.
//   The 0xCC-byte frame gets VC7.1's biased frame (`lea ebp,[esp-6Ch]`) from `optimize("s", on)` alone; `y off` removes it.
// Header debt: the record is modelled by its retail size (0xC4) with its copy constructor, assignment and destructor
//   (no generated engine header; Ego_r.pdb layout is 4 bytes larger, retail CParentDefClassBase is 0x3C).
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
class __declspec(novtable) CEnvironmentThemeDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CEnvironmentThemeDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CEnvironmentThemeDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CEnvironmentThemeDef@@UAEKXZ
    CEnvironmentThemeDef(const CEnvironmentThemeDef& other);                 // 0x0047318D
    CEnvironmentThemeDef& operator=(const CEnvironmentThemeDef& other);      // 0x00473046
    __forceinline virtual ~CEnvironmentThemeDef() { }
    unsigned long _members[(0xc4 - 0x3c) / 4];
};
namespace std {
struct __false_type { };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__forceinline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BI1, class _BI2>
__forceinline _BI2 __copy_backward_ptrs(_BI1 __first, _BI1 __last, _BI2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, (int*)0);
}
template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);   // 0x0047725F
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x);
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00477169
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(_Tp* __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if (unsigned int(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            _Tp* __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}
template void vector<CEnvironmentThemeDef>::_M_fill_insert(CEnvironmentThemeDef*, unsigned int, const CEnvironmentThemeDef&);
}