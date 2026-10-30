// std::vector<CQuickAccessItemButtonGuiDef>::_M_fill_insert(CQuickAccessItemButtonGuiDef* position, unsigned int n, const CQuickAccessItemButtonGuiDef& x) -- STLport insert of n copies of a quick-access button def: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCQuickAccessItemButtonGuiDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00476212, 411 bytes, thiscall `ret 0Ch`; size TU with a frame pointer.
// Identity: the only caller is vector<CQuickAccessItemButtonGuiDef>::insert(position, n, x) 0x00476163; stride 0x50
//   with the element copy constructor 0x0046EE2D and the __copy_backward_ptrs instance 0x00476497 (both landed). The
//   catalog label "Vector_InsertQuickAccessButtonAndShift" is behavioural.
// Callees: CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef&) 0x0046EE2D, __copy_backward_ptrs(first,
//   last, result, const __false_type&) 0x00476497 (__fastcall), CParentDefClassBase::operator= 0x00431ED0 (the inlined
//   element assignment's first step), uninitialized_fill_n(first, n, x) 0x00476505 (__fastcall), the x_copy destructor's
//   out-of-line base step 0x00430300 (the identical-code-folded ~CParentDefClassBase), _M_insert_overflow 0x004763AF.
// Shape: STLport 4.x `_M_fill_insert` verbatim. The element count is a signed `idiv` by 0x50; `x_copy` is copy-constructed
//   in the frame and destroyed by the implicit (no vptr store) destructor; the inlined `_Construct` is the null-checked
//   placement copy; both `fill` loops inline the member-wise assignment; the two empty `__false_type` tags are zero-stored
//   (`xor eax,eax ; stosb`) into dead argument slots ([ebp+13h], [ebp+0Fh]).
// Model: as the landed 0x0046EE2D / 0x00476497: CParentDefClassBase is 0x3C bytes with its own operator=, then
//   ButtonGraphic +0x3C, Position +0x40, Offset +0x48 (Ego_r.pdb names; retail base 4 bytes smaller than the PDB's).
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

class C2DVector { public:
    float X;
    float Y;
};

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CQuickAccessItemButtonGuiDef : public CParentDefClassBase { public:  // vtable 0x012360A4, retail sizeof 0x50
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CQuickAccessItemButtonGuiDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CQuickAccessItemButtonGuiDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CQuickAccessItemButtonGuiDef@@UAEKXZ
    long      ButtonGraphic;                                 // +0x3c
    C2DVector Position;                                      // +0x40
    C2DVector Offset;                                        // +0x48
    CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef& other);   // 0x0046EE2D
    __forceinline CQuickAccessItemButtonGuiDef& operator=(const CQuickAccessItemButtonGuiDef& other)
    {
        CParentDefClassBase::operator=(other);
        ButtonGraphic = other.ButtonGraphic;
        Position = other.Position;
        Offset = other.Offset;
        return *this;
    }
};

namespace std {

struct __false_type { };

CQuickAccessItemButtonGuiDef* __fastcall uninitialized_fill_n(CQuickAccessItemButtonGuiDef* first, unsigned int n,
                                                              const CQuickAccessItemButtonGuiDef& x);       // 0x00476505

template <class _Tp> _Tp* __fastcall __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result,
                                                         const __false_type&);                             // 0x00476497

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _Tp> __forceinline _Tp* __uninitialized_copy(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    _Tp* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(__cur, *__first);
    return __cur;
}

template <class _Tp> __forceinline void fill(_Tp* __first, _Tp* __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x004763AF
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = _M_finish - __position;
            pointer __old_finish = _M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                _M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                _M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CQuickAccessItemButtonGuiDef>::_M_fill_insert(CQuickAccessItemButtonGuiDef*, unsigned int,
                                                                   const CQuickAccessItemButtonGuiDef&);

}