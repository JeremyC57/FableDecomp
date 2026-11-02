// std::vector<CTVertexRHWColSpecTex1>::erase(iterator first, iterator last) -- STLport range erase: assign the tail [last, finish) down onto first, destroy the vacated (trivial) vertices, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCTVertexRHWColSpecTex1@@V?$allocator@VCTVertexRHWColSpecTex1@@@std@@@std@@QAEPAVCTVertexRHWColSpecTex1@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1440, length 79 bytes, thiscall, `ret 8`, returns first.
// Identity: callers 0x009DB678 (CRenderManager2D, `erase(begin(), end())` on the vector at +0x3E88 = QuickDraw2DVertices, the
//   landed CRenderManager2D ctor 0x009D9A30 model) and 0x009E14A7 (the out-of-line clear() 0x009E14A0: `erase(_M_start, _M_finish)`).
//   Ego_r CRenderManager2D: QuickDraw2DVertices is vector<CTVertexRHWColSpecTex1> (class key V, 32 bytes, the D3D
//   XYZRHW|DIFFUSE|SPECULAR|TEX1 vertex: Ego_r ?reserve@?$vector@VCTVertexRHWColSpecTex1@@...); the element stride here is 32
//   (`sar ecx,5`, `rep movsd` x 8). Retail STL is STLport 4.6, whose vector<T>::iterator is T*, so the symbol is the STLport
//   erase(T*, T*) spelling. The July-era label "Render2DDrawList::CopyBlock" (Render2DDrawListBlockView in
//   fable_render2d_vertex_queue.h) is descriptive and wrong.
// Shape: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with the random-access __copy inlined (count = (finish - last) / 32, one struct copy
//   per pass) and the __destroy_aux loop over an implicit destructor folded away. The element has no __type_traits
//   specialisation, so both traits are STLport's default __false_type (tags with user constructors, chunk lessons).
// Repair (chunk 705): the landed source's bytes matched but its July-era test included the source through a relative path and
//   a project header (fable_render2d_vertex_queue.h) and printed a non-standard pass line, so the lane's test compile failed
//   (TCC_FAIL). The function is re-authored under its real STLport identity with a standalone test.
#include <stddef.h>
class CTVertexRHWColSpecTex1 { public:                         // 32 bytes (D3D XYZRHW | DIFFUSE | SPECULAR | TEX1)
    float X, Y, Z, RHW;
    unsigned long Diffuse;
    unsigned long Specular;
    float U, V;
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                          _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template vector<CTVertexRHWColSpecTex1>::iterator vector<CTVertexRHWColSpecTex1>::erase(vector<CTVertexRHWColSpecTex1>::iterator, vector<CTVertexRHWColSpecTex1>::iterator);
} // namespace std