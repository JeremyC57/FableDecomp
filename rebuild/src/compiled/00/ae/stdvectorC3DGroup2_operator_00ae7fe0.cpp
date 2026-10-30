// std::vector<C3DGroup2>::operator=(const vector& x) -- STLport assignment of the 16-byte skinning groups: if x is not *this, allocate-and-copy into a fresh block and free the old one when x outgrows the capacity, else assign over the live groups and destroy the excess (or copy-construct the tail); size becomes x.size()
// Leaf: ??4?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7FE0, 257 bytes, thiscall, returns *this, `ret 4`.
// Identity (proven): the callees are all landed vector<C3DGroup2> helpers -- __uninitialized_copy<const C3DGroup2*>
//   0x00AE7530, __copy<const C3DGroup2*, C3DGroup2*, int> 0x00AE7660, _Destroy<C3DGroup2*> 0x00AE7EC0 and the bound
//   ?_M_clear@?$vector@VC3DGroup2@@...@IAEXXZ 0x00AE7FA0; the body is the STLport 4.6 vector.c operator= (the twin of the
//   landed vector<C3DVertexBlend2>::operator= 0x00AE6690, whose callers are exactly these element helpers); stride 16 =
//   Ego_r.pdb C3DGroup2 {long BoneIndex; CArray<C3DVertexBlend2> VertexBlends} with the 12-byte retail CArray.
// Label: the catalog "Vector_InsertRange_Pair128Bit" is descriptive.
// Callers: 0x00AE39E7 (CParticleProgram area, copying a CArray<C3DGroup2>) and 0x00AE81A8.
// Callees: malloc 0x00BFEA0E (allocator::allocate inside the inline _M_allocate_and_copy), __uninitialized_copy 0x00AE7530
//   (x2), _M_clear 0x00AE7FA0, __copy 0x00AE7660 (x2, __copy_ptrs inlined into it), _Destroy 0x00AE7EC0.
// Shape: STLport 4.6 operator= verbatim; _M_allocate_and_copy and __copy_ptrs inline, the element helpers out of line.
class C3DVertexBlend2 {                                    // Ego_r.pdb, 8 bytes
public:
    unsigned long VertexIndex;                            // +0x00
    float BlendValue;                                     // +0x04
};
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                    // 0x00AE7660
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                               // 0x00AE7530
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                      // 0x00AE7EC0
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    void _M_clear();                                                                              // 0x00AE7FA0
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
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
}
template <class T> class CArray : public std::vector<T> { };   // retail CArray<T> = the 12-byte STLport vector
class C3DGroup2 {                                          // Ego_r.pdb, 16 bytes in retail
public:
    long BoneIndex;                                       // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                 // +0x04
};
template std::vector<C3DGroup2 >& std::vector<C3DGroup2 >::operator=(const std::vector<C3DGroup2 >&);