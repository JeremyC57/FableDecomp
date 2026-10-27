// std::vector<CCountedPointer<C3DClothPrimitive>>::reserve(size_type n) -- STLport reserve: when capacity() < n, allocate n elements, copy the live ones over and destroy/free the old block (or just allocate for an empty vector); keep the size
// Leaf: ?reserve@?$vector@V?$CCountedPointer@VC3DClothPrimitive@@@@V?$allocator@V?$CCountedPointer@VC3DClothPrimitive@@@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a975e0, 164 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?reserve@?$vector@V?$CCountedPointer@VC3DClothPrimitive@@@@...@QAEXI@Z; its __uninitialized_copy 0x00A95750 and __destroy_aux 0x00A96940 are bound to CCountedPointer<C3DClothPrimitive>; caller 0x00A91640.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy<T*, T*>(first, last, result, const __false_type&)
//   0x00A95750 and __destroy_aux<T*>(first, last, const __false_type&) 0x00A96940 (both __fastcall, out of line).
// Shape (family L017): STLport 4.6 vector.c reserve verbatim with the inline _M_allocate_and_copy / _M_clear / _M_set; both
//   empty __false_type tags are homed on the live n argument slot, which VC7.1 does only when the two tag-taking helpers
//   have same-TU noinline bodies that never read the tag (their real STLport bodies here; only reserve is graded).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{                                                                                          // 0x00A95750 (same-TU body)
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                                                          // 0x00A96940 (same-TU body)
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void reserve(size_type __n);
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}
class CCPPointerInfo;
template <class T> class CCountedPointer {                 // 8 bytes: Data, Info (count at Info+0)
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
class C3DClothPrimitive;
template void std::vector<CCountedPointer<C3DClothPrimitive> >::reserve(unsigned int);