// std::vector<CAppearanceModifierScalingDef>::_M_fill_insert(CAppearanceModifierScalingDef* pos, unsigned int n, const CAppearanceModifierScalingDef& x) -- STLport vector insert(pos, n, x): shift the tail in place when capacity allows, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCAppearanceModifierScalingDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047A08C, 349 bytes, thiscall `ret 0Ch`, ebp frame (size-optimised defs TU).
// Identity (proven): x_copy and the placement constructs call 0x004784B3, bound ??0CAppearanceModifierScalingDef@@QAE@ABV0@@Z; the backward copy and the
//   fills call 0x0047848C, bound ??4CAppearanceModifierScalingDef@@QAEAAV0@ABV0@@Z (0x48-byte stride); its only caller is the vector's resize 0x00479FF3.
// Callees: the copy constructor 0x004784B3, operator= 0x0047848C, uninitialized_fill_n 0x0047A2CF (tag-less, __fastcall),
//   ~CParentDefClassBase 0x00430300 (x_copy: the implicit element destructor inlines to the base call), _M_insert_overflow 0x0047A1E9.
// Shape (family K103, representative 0x00478C79): the landed vector<CMultiStaticMeshEntryDef>::_M_fill_insert 0x004EDFDC with
//   every copy helper inline; x_copy's destructor is the implicit one, inlined to the bare ~CParentDefClassBase 0x00430300 call (no vptr store). The element is modelled on a packed 0x39-byte CParentDefClassBase
//   (virtual destructor 0x00430300) with its own bytes opaque.
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
class CAppearanceModifierScalingDef : public CParentDefClassBase {               // retail 0x48 bytes
public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CAppearanceModifierScalingDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CAppearanceModifierScalingDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CAppearanceModifierScalingDef@@UAEKXZ
    unsigned char _own[0xF];                       // +0x39 (not touched here)
    CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef& other);                           // 0x004784B3
    CAppearanceModifierScalingDef& operator=(const CAppearanceModifierScalingDef& other);                // 0x0047848C
};

namespace std {
struct __false_type { };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type is_POD_type;
};

template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2>
__forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x0047A2CF

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                 _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
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
    for ( ; __first != __last; ++__first)
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
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x0047A1E9
    public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<CAppearanceModifierScalingDef >::_M_fill_insert(CAppearanceModifierScalingDef*, unsigned int, const CAppearanceModifierScalingDef&);
}