// std::vector::operator=(const vector& __x) -- (vector<NUISystem::CUIStateDef>) STLport vector assignment for the 0x7C-byte UI state def: reallocate and copy when x outgrows the capacity, else assign over the live elements and destroy the surplus or copy-construct the rest
// Leaf: ??4?$vector@VCUIStateDef@NUISystem@@V?$allocator@VCUIStateDef@NUISystem@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006315E0, 290 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): the landed _M_clear 0x00630E00 and _M_allocate_and_copy 0x00631590 of vector<NUISystem::CUIStateDef>
//   name this row as their only caller (the vptr 0x0125871C of RTTI .?AVCUIStateDef@NUISystem@@ is stored by the
//   element's copy constructor 0x006250B0); it is CUIDef::States' assignment, called from CUIDef's copy constructor
//   (0x0063053B) and CUIDef::operator= (0x00631757). Stride 0x7C (`imul 84210843h ; add ; sar 6`).
// Callees: _M_allocate_and_copy<const CUIStateDef*> 0x00631590 and _M_clear 0x00630E00 (thiscall, out of line, their
//   landed spellings), __copy<const CUIStateDef*, CUIStateDef*, int> 0x006314C0, _Destroy<CUIStateDef*> 0x00630DD0
//   (two-argument, the virtual destructor loop), __uninitialized_copy<const CUIStateDef*, CUIStateDef*> 0x00631560
//   -- the free helpers __fastcall.
// Shape: STLport 4.6 vector.c `operator=` verbatim (the 0x00624A30 model of this chunk) with _M_allocate_and_copy and
//   _M_clear out of line; __copy_ptrs (__false_type) forwards to the out-of-line __copy; the range helpers are same-TU
//   `__declspec(noinline)` bodies, their tags homed in the dead __x slot.
inline void* operator new(unsigned int, void* p) { return p; }

namespace NUISystem {
class CUIStateDef {                                         // retail 0x7C bytes, polymorphic (CParentDefClassBase)
public:
    virtual ~CUIStateDef();
    unsigned char _body[0x7C - 4];
    CUIStateDef(const CUIStateDef& other);                  // 0x006250B0
    CUIStateDef& operator=(const CUIStateDef& other);
};
}

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{                                                                                              // 0x00630DD0
    __destroy_aux(__first, __last, __false_type());
}
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00631560
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x006314C0
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    protected:
    void _M_clear();                                                                           // 0x00630E00
    public:
    protected:
    template <class _ForwardIter>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIter __first, _ForwardIter __last);    // 0x00631590
    public:
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0,
                                      (pointer)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                        _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0,
                                 this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<NUISystem::CUIStateDef>& vector<NUISystem::CUIStateDef>::operator=(const vector<NUISystem::CUIStateDef>&);
} // namespace std