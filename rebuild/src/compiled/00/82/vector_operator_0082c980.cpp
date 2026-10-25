// vector::operator=(const vector& x) -- STLport vector<CMinimapData::MinimapTheme> copy assignment: reallocate and copy when x outgrows the capacity, else assign over the live elements and copy-construct the tail
// Leaf: ??4?$vector@W4MinimapTheme@CMinimapData@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082C980, 222 bytes (+2 int3), thiscall `ret 4`.
// Identity: the 4-byte element sits at +8 of CMinimapData (Ego_r.pdb MinimapThemeList, vector<CMinimapData::MinimapTheme>,
//   the record 0x0082CB70 copies); the callers 0x0082CB29 / 0x0082CD37 are the CMinimapData assignment loops. The catalog
//   label "std_Move_UintVector" is descriptive. An enum element has no STLport __type_traits specialisation, so every
//   path takes the __false_type form: the element-wise __copy loop (no memmove) and the out-of-line
//   __uninitialized_copy.
// Callees: vector::_M_allocate_and_copy(n, first, last) @ 0x0082C450 (thiscall), __uninitialized_copy(first, last,
//   result, const __false_type&) @ 0x0082C420 (__fastcall, landed), free @ 0x00BFEA14 (the inline _M_clear).
// Shape: STLport 4.x `vector::operator=` verbatim, the `_M_finish = _M_start + xlen` tail duplicated into each branch;
//   the tag of the uninitialized copy is homed on the dead `x` argument slot. Same-TU noinline stand-ins for the two
//   callees (only operator= is graded); explicit instantiation of operator=. Lever: `__copy` / `__copy_ptrs` are
//   `__forceinline` -- plain `inline` leaves the __false_type copy as a cdecl call (206v222).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);           // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CMinimapData {
public:
    enum MinimapTheme { MINIMAP_THEME_FIRST };          // Ego_r.pdb CMinimapData::MinimapTheme (enumerators not needed)
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0082C420
{
    for (; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}

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

template <class _Tp>
__forceinline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _Alloc>
class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage._M_data - _M_start); }
    __declspec(noinline) pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last);   // 0x0082C450
    void _M_clear()
    {
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    vector& operator=(const vector& __x);
};

template <class _Tp, class _Alloc>
_Tp* vector<_Tp, _Alloc>::_M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
{
    pointer __result = _M_end_of_storage.allocate(__n);
    __uninitialized_copy(__first, __last, __result, __false_type());
    return __result;
}

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
        } else if (size() >= __xlen) {
            __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
        } else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
}

template std::vector<CMinimapData::MinimapTheme>& std::vector<CMinimapData::MinimapTheme>::operator=(const std::vector<CMinimapData::MinimapTheme>&);