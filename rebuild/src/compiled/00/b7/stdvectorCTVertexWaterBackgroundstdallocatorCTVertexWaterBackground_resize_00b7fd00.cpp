// std::vector<CTVertexWaterBackground>::resize(unsigned int __new_size, const CTVertexWaterBackground& __x) -- STLport 4.6: erase the tail
//   (erase(begin() + n, end()) 0x00B7FD70) when shrinking, else insert(end(), n - size(), x) = _M_fill_insert 0x00B7FDE0 of the missing elements
// Leaf: ?resize@?$vector@VCTVertexWaterBackground@@V?$allocator@VCTVertexWaterBackground@@@std@@@std@@QAEXIABVCTVertexWaterBackground@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7FD00, 105 bytes, thiscall, `ret 8`.
// Identity (wave 17 DEFERRED ?resize@?$vector@VCTVertexWaterBackground@@...): FableWin.pdb CWaterPatchTesselatorBase<CTVertexWaterBackground>::
//   SetBridgeVertexBufferSize resizes BridgeVertexBuffer (CArray<CTVertexWaterBackground>, 56-byte elements: the signed `cdq / idiv 0x38`);
//   the callees are the landed erase 0x00B7FD70 and _M_fill_insert 0x00B7FDE0 rows of the same instantiation.
// Shape: the landed erase row's STLport text (the vertex, the __copy chain) plus the in-class resize / insert; erase and _M_fill_insert are
//   the out-of-line members retail called.
// Naming wave 18: landed as the descriptive CRetailVector_elem56::resize(unsigned long, const SElement56&) with Erase / Insert stand-ins;
//   re-authored on the STLport instantiation (name-only: the bytes are unchanged).
#include "fable_compiler.h"
class CTVertexWaterBackground {                         // 56 bytes: Ego_r.pdb CGenericVertex<CTVertexWaterBackgroundBase>
public:
    short X;                                            // +0x00
    short Y;                                            // +0x02
    float Z;                                            // +0x04
    float ShoreLookUpArray[12];                         // +0x08
};

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
FABLE_FORCEINLINE _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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
FABLE_FORCEINLINE _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp>
FABLE_FORCEINLINE _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __true_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last);                                      // 0x00B7FD70 (its own row)
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);          // 0x00B7FDE0 (its own row)
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<CTVertexWaterBackground>::resize(unsigned int __new_size, const CTVertexWaterBackground& __x);
}