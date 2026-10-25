// _STL::vector<C3DTriangle2>::resize(unsigned int new_size, const C3DTriangle2& x) -- STLport resize of a 6-byte index-triangle vector: erase the tail past new_size, or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A97A50, 85 bytes (+ int3 padding), `__fastcall` (this -> esi, start -> edi), `ret 8`.
// Identity: the landed C3DMeshFileTrianglesChunk::ReadFromFile 0x00ABD280 calls it (0x00ABD2F5) as TriangleList.resize(n,
//   T()) and names it vector<C3DTriangle2>::resize; its _M_fill_insert 0x00A96F00 uses the C3DTriangle2 uninitialized copy
//   0x00A6E350. Element is Ego_r.pdb C3DTriangle2 (unsigned short VertexIndex[3]; 6 bytes). Other callers: 0x00A9096D,
//   0x00A9815D, 0x00A98330, 0x00AE3D6B. The catalog's "vector<CBrainBehaviour>::resize" is propagated.
// Callees: STLport __copy<C3DTriangle2*, C3DTriangle2*, int>(first, last, result, const random_access_iterator_tag&,
//   int*) 0x00A744B0 (`__fastcall`: first ecx, last edx; the empty tag by reference into the dead new_size slot),
//   vector::_M_fill_insert(pos, n, x) 0x00A96F00 (catalog "Vector_Assign_C3DTriangle2_Vertices").
// Shape: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size -
//   size(), __x);` -- erase is `__i = __copy_ptrs(__last, _M_finish, __first, __false_type()); _Destroy(__i, _M_finish);
//   _M_finish = __i;` (the destroy loop of the trivially destructible record compiles away); size() is the 0x2AAAAAAB
//   reciprocal divide by 6. Byte twin of 0x00A55670 (vector<C3DAnimationSequenceData::C3DVectorWord>, other callees).
class C3DTriangle2 {                                    // Ego_r.pdb, 6 bytes (only copied here)
public:
    unsigned short VertexIndex[3];                      // +0x00
};

namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x00A744B0

template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> inline void __fastcall _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator>
inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x00A96F00
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<C3DTriangle2>::resize(unsigned int, const C3DTriangle2&);
}