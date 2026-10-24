// _STL::vector::erase(CTVertexWaterBackground* first, CTVertexWaterBackground* last) -- (vector<CTVertexWaterBackground>, the water patch tesselator's bridge vertex buffer) move the tail down over [first, last), set the new finish and return first
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7FD70, 112 bytes. __fastcall, `ret 8`.
// Callees: none.
// Callers: vector<CTVertexWaterBackground>::resize(n, x) 0x00B7FD00 (call 0x00B7FD2E: the shrinking `erase(begin() + n, end())`),
//   from CWaterPatchTesselatorBase<CTVertexWaterBackground>::SetBridgeVertexBufferSize.
// Identity: FableWin.pdb `?SetBridgeVertexBufferSize@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEXJ@Z`
//   resizes BridgeVertexBuffer (CArray<CTVertexWaterBackground> at +0x18 in Ego_r.pdb); the divide by 56
//   (imul 0x92492493, sar 5) gives the 56-byte vertex. The manifest label "Vector_Erase_56Byte" is descriptive.
//   STLport `vector<_Tp>::erase(iterator, iterator)`.
// Shape: STLport 4.x `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;`
//   (template shape of the landed vector<C3DVertex2>::erase 0x00A96A90, with copy/__copy_ptrs/__copy `__forceinline`:
//   for a non-power-of-two element the pointer-difference divide makes VC7.1 refuse the plain-inline chain) -- the vertex has no __type_traits
//   specialisation, so copy() takes the __copy_ptrs(__false_type) arm: the random-access `__copy` loop
//   `for (n = last - first; n > 0; --n) *result++ = *first++;` (signed `jle`, the source folded into an offset from
//   the destination); each implicit 56-byte assignment is a 14-dword `rep movsd`; the trivial _Destroy is empty;
//   `this` spilled to the `push ecx` frame word.
class CTVertexWaterBackground {                         // 56 bytes: Ego_r.pdb CGenericVertex<CTVertexWaterBackgroundBase>
public:
    short X;                                            // +0x00
    short Y;                                            // +0x02
    float Z;                                            // +0x04
    float ShoreLookUpArray[12];                         // +0x08
};

namespace _STL {

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

template CTVertexWaterBackground* vector<CTVertexWaterBackground>::erase(CTVertexWaterBackground* __first, CTVertexWaterBackground* __last);
}