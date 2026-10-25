// std::vector::erase(CTVertexLandscapeBackground* first, CTVertexLandscapeBackground* last) -- (vector<CTVertexLandscapeBackground>, the landscape tesselator's bridge vertex buffer) move the tail down over [first, last), set the new finish and return first
// Grade: DIFFER(41v113) -- best GRADED form (the 4-level std copy -> __copy_aux -> __copy_ptrs -> __copy chain) left __copy_ptrs out of line; THIS file is the ungraded 2-level all-__forceinline form that MATCHED the sibling rows 0x00B7FD70 / 0x00B80190 (grade budget spent before it could be tried here)
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6CD80, 113 bytes. __thiscall, `ret 8`.
// Callees: none.
// Callers: vector<CTVertexLandscapeBackground>::resize(n, x) 0x00B6CD10 (call 0x00B6CD3D: the shrinking
//   `erase(begin() + n, end())`), from CLandscapePatchTesselator::SetBridgeVertexBufferSize 0x00B6CCF0 (landed).
// Identity: FableWin.pdb `?SetBridgeVertexBufferSize@CLandscapePatchTesselator@@EAEXJ@Z` resizes BridgeVertexBuffer
//   (CArray<CTVertexLandscapeBackground> at +0x18 in Ego_r.pdb; 24-byte vertex, divide imul 0x2AAAAAAB, sar 2); the
//   manifest label "Vector_Copy_Backward_6Word" is descriptive (the copy runs forward).
//   STLport `vector<_Tp>::erase(iterator, iterator)`.
// Shape: STLport 4.x `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;`
//   (template shape of the landed vector<C3DVertex2>::erase 0x00A96A90, with copy/__copy_ptrs/__copy `__forceinline`:
//   for a non-power-of-two element the pointer-difference divide makes VC7.1 refuse the plain-inline chain) -- the vertex has no __type_traits
//   specialisation, so copy() takes the __copy_ptrs(__false_type) arm: the random-access `__copy` loop
//   `for (n = last - first; n > 0; --n) *result++ = *first++;` (signed `jle`, the count homed in the dead `first`
//   argument slot, the source folded into an offset from the destination); each implicit 24-byte assignment is six
//   dword moves; the trivial _Destroy is empty.
class CTVertexLandscapeBackground {                     // 24 bytes: Ego_r.pdb CGenericVertex<CTVertexLandscapeBackgroundBase>
public:
    unsigned short X;                                   // +0x00
    unsigned short Y;                                   // +0x02
    float Z;                                            // +0x04
    float NormalX;                                      // +0x08
    float NormalY;                                      // +0x0C
    float NormalZ;                                      // +0x10
    unsigned char UVPad1;                               // +0x14
    unsigned char V1;                                   // +0x15
    unsigned char U1;                                   // +0x16
    unsigned char UVPad2;                               // +0x17
};

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

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

template <class _Tp>
__forceinline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
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

template CTVertexLandscapeBackground* vector<CTVertexLandscapeBackground>::erase(CTVertexLandscapeBackground* __first, CTVertexLandscapeBackground* __last);
}