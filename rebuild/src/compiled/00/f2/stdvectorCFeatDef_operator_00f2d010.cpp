// std::vector<CFeatDef>::operator=(const vector& x) -- STLport copy assignment of a script def's feat vector (0x64-byte CFeatDef defs): reallocate and copy-construct when x outgrows the capacity, else assign over the live prefix (destroying the surplus) or assign the prefix and copy-construct the tail
// Leaf: ??4?$vector@VCFeatDef@@V?$allocator@VCFeatDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2D010, 417 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): the callees are the landed CFeatDef members -- operator= 0x00F2D200 (??4CFeatDef@@QAEAAV0@ABV0@@Z) in the
//   assignment loops, the copy constructor 0x00F2D270 (??0CFeatDef@@QAE@ABV0@@Z) in the construct loop -- the landed
//   vector<CFeatDef>::_M_allocate_and_copy 0x00F2D690 and _Destroy<CFeatDef*> 0x00F28F20, with the 0x64 stride (the
//   `imul 0x51EB851F ; sar 5` divide by 100); Ego_r.pdb name set ??4?$vector@VCFeatDef@@V?$allocator@VCFeatDef@@@std@@@std@@QAEAAV01@ABV01@@Z.
//   Only caller 0x00F2CF6B (CScriptDef::operator=, its Feats member per the landed _Destroy 0x00F28F20). Catalog label
//   std_vector_assign_CFeatDef (descriptive).
// Callees: _M_allocate_and_copy 0x00F2D690, _M_clear 0x00F2D1C0 (destroy + free; landed as the invented
//   CRetailVector_poly0x64::~CRetailVector_poly0x64), CFeatDef::operator= 0x00F2D200, std::_Destroy 0x00F28F20 (__fastcall),
//   CFeatDef::CFeatDef(const CFeatDef&) 0x00F2D270.
// Shape: the landed STLport 4.6 vector<CConversation>::operator= 0x00E54AA0 model unchanged: __copy_ptrs / __copy /
//   __uninitialized_copy `__forceinline`, the inline placement _Construct (null test, copy constructor out of line);
//   _M_allocate_and_copy, _M_clear and _Destroy are declarations.
inline void* operator new(unsigned int, void* p) { return p; }
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in); CFeatDef is
//   the Ego_r.pdb 0x64-byte def (vtable 0x012FB778, no RTTI; members opaque here, see the landed CFeatDef::Transfer 0x00F2D330).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CFeatDef : public CDefClassBase {                 // 0x64 bytes; vtable 0x012FB778 (no RTTI)
public:
    virtual ~CFeatDef();                                // slot 0  0x00F2D300 (vector deleting)
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F2D330
    virtual void Copy(const CDefClassBase* other);      // slot 19 0x00F2D680
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F2D2F0
    unsigned char _members[0x3c];                       // +0x28 FeatName .. CO_ItemName (opaque here)
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
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                    // 0x00F28F20
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
    void _M_clear();                                    // 0x00F2D1C0
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00F2D690
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

template vector<CFeatDef>& vector<CFeatDef>::operator=(const vector<CFeatDef>&);
}   // namespace std