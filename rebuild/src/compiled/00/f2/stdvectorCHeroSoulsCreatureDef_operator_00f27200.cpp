// std::vector<CHeroSoulsCreatureDef>::operator=(const vector& x) -- STLport copy assignment of a Hero Souls wave's creature-def vector (0x34-byte defs): reallocate and copy-construct when x outgrows the capacity, else assign over the live prefix (destroying the surplus) or assign the prefix and copy-construct the tail
// Leaf: ??4?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F27200, 422 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): the callees are the landed CHeroSoulsCreatureDef members -- operator= 0x00F27190
//   (??4CHeroSoulsCreatureDef@@QAEAAV0@ABV0@@Z) in the assignment loops, the copy constructor 0x00F27060
//   (??0CHeroSoulsCreatureDef@@QAE@ABV0@@Z) in the construct loop -- the landed vector<CHeroSoulsCreatureDef>::_M_allocate_and_copy
//   0x00F273F0 and the destroy-and-free 0x00F273B0 (STLport `_M_clear`, landed as the invented ~CRetailVector_poly0x34), with the
//   0x34 stride (Ego_r.pdb CHeroSoulsCreatureDef is 52 bytes; vtable 0x012FB500, GetSizeofClass 0x00F270A0 returns 0x34); the
//   Ego_r.pdb name set holds ??4?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAEAAV01@ABV01@@Z
//   (CHeroSoulsWaveDef::Creatures). Six callers (0x00F271EB, 0x00F27DE5, 0x00F28975, 0x00F28A83, 0x00F28B43, 0x00F28D7B).
//   Catalog label std::vector_assign_pair_CCharString_CSymbolInfo is propagated (wrong element type).
// Callees: _M_allocate_and_copy 0x00F273F0, _M_clear 0x00F273B0, CHeroSoulsCreatureDef::operator= 0x00F27190,
//   CHeroSoulsCreatureDef::CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef&) 0x00F27060; the surplus is destroyed through
//   the element's virtual destructor (vtable slot 0, flag 0).
// Shape: the landed vector<CConversation>::operator= 0x00E54AA0 model (`__forceinline` __copy_ptrs / __copy /
//   __uninitialized_copy, inline placement _Construct with its null test), and here the range _Destroy inlined as well
//   (`__forceinline` _Destroy(first, last) / __destroy / __destroy_aux over the inline `p->~T()`); _M_allocate_and_copy and
//   _M_clear are declarations.
inline void* operator new(unsigned int, void* p) { return p; }
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, 0x28 bytes, vtable 0x01230BCC).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CHeroSoulsCreatureDef : public CDefClassBase {    // 0x34 bytes; vtable 0x012FB500 (no RTTI)
public:
    virtual ~CHeroSoulsCreatureDef();                   // slot 0 (scalar deleting)
    virtual void Transfer(CPersistContext&);            // slot 18 0x00F27100
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00F27180
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F270A0
    CCharString CreatureType;                           // +0x28
    CCharString Location;                               // +0x2c
    long        NumCreatures;                           // +0x30
    CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& other);              // 0x00F27060
    CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef& other);   // 0x00F27190
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

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator, class _Tp>
__forceinline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp>
__forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, (_Tp*)0); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
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
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector& operator=(const vector& __x);
protected:
    void _M_clear();                                    // 0x00F273B0
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00F273F0
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CHeroSoulsCreatureDef>& vector<CHeroSoulsCreatureDef>::operator=(const vector<CHeroSoulsCreatureDef>&);
}   // namespace std