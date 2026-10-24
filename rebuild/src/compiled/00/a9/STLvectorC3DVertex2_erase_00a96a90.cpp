// _STL::vector<C3DVertex2>::erase(C3DVertex2* first, C3DVertex2* last) -- remove a range of 32-byte mesh vertices: slide the tail down over it and pull the finish pointer back
// Leaf: ?erase@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96A90, 80 bytes, thiscall `ret 8`, returns `first`.
// Identity: 7 call sites; C3DMeshFileVerticesChunk::ReadFromFile 0x00ABD150 calls it as the shrink arm of
//   `VertexList.resize(n)` (`erase(begin() + n, end())` on the CArray<C3DVertex2> at +0x10); the others are
//   N3DPrimitive::CDeviceIndependent 0x00A98100 and the primitive builders. C3DVertex2 is Ego_r's 32-byte
//   {C3DVector Position, C3DVector Normal, float U, V}. The catalog's "Mesh_CopyVertexBuffer_32ByteElements" is descriptive.
// Callees: none.
// Shape: STLport 4.x `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;` --
//   C3DVertex2 has no __type_traits specialisation, so copy() takes the __copy_ptrs(__false_type) arm: the random-access
//   `__copy` loop `for (n = last - first; n > 0; --n) *result++ = *first++;` (signed `jle`), each implicit 32-byte
//   assignment an 8-dword `rep movsd`; the trivial _Destroy is empty; `this` spilled to the `push ecx` frame word.
class C3DVector {
public:
    float X, Y, Z;
};

class C3DVertex2 {
public:
    C3DVector Position;                                 // +0x00
    C3DVector Normal;                                   // +0x0c
    float     U;                                        // +0x18
    float     V;                                        // +0x1c
};

namespace _STL {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp>
inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
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
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy((const _Tp*)__last, (const _Tp*)this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template C3DVertex2* vector<C3DVertex2>::erase(C3DVertex2* __first, C3DVertex2* __last);
}