// std::vector::erase(iterator first, iterator last) -- (vector<CSubtitleRenderer::CEmbeddedGraphic>) STLport range erase: assign the tail [last, finish) down onto first with the out-of-line __copy, destroy the vacated (trivially destructible) tail, finish = the new end; returns first
// Leaf: ?erase@?$vector@VCEmbeddedGraphic@CSubtitleRenderer@@V?$allocator@VCEmbeddedGraphic@CSubtitleRenderer@@@std@@@std@@QAEPAVCEmbeddedGraphic@CSubtitleRenderer@@PAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493E2C, 37 bytes, thiscall `ret 8`; frame-pointer size-optimised TU (`push ebp ; mov ebp,esp`, the empty random_access_iterator_tag homed in the dead argument slot `[ebp+0Bh]`).
// Identity (proven): vector<CSubtitleRenderer::CEmbeddedGraphic>::erase; evidence: CSubtitleRenderer::ProcessEmbeddedGraphicTags 0x00492381 clears this+0xB8 through it (`erase(begin, end)`), and the subtitle text storage path (0x00493FA8 <- 0x00494106 <- CSubtitleRenderer_ManageTextStorage) does the same; the 16-byte stride is Ego_r.pdb CSubtitleRenderer::CEmbeddedGraphic {Graphic, CharIndex, Offset} (EmbeddedGraphics; the sibling CEmbeddedHighlight is 20 bytes). The catalog's CMusicEntry (also 16 bytes) is a propagated label.
// Label: the manifest's "Std_Vector_Insert_Move_CMusicEntry" is descriptive / propagated (family H062: six range erases whose element assignment is not trivial to STLport, one per element).
// Callee: std::__copy<CSubtitleRenderer::CEmbeddedGraphic*, CSubtitleRenderer::CEmbeddedGraphic*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x004936BA (__fastcall, the counted assignment loop).
// Shape: STLport 4.6 `erase(first, last) { pointer i = __copy_ptrs(last, _M_finish, first, _TrivialAss()); _Destroy(i, _M_finish); _M_finish = i;
//   return first; }` -- `__copy_ptrs(..., __false_type)` inline forwarding to the out-of-line `__copy`, the trivial destructor loop empty.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CSubtitleRenderer {
public:
    class CEmbeddedGraphic {                                // Ego_r.pdb, 16 bytes
    public:
        long Graphic;
        long CharIndex;
        float Offset[2];                                    // C2DVector
    };
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

// The out-of-line __copy COMDAT (0x004936BA) with its body in the TU (`auto_inline(off)`, the tag never touched): VC7.1 then homes the
// empty tag temporary over first's argument slot as retail does (GOTCHAS 2026-10-20: COMDAT template instantiations deserve bodies).
#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class allocator { public: allocator() { } };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08 (the _STLP_alloc_proxy's pointer)
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

}

template CSubtitleRenderer::CEmbeddedGraphic* std::vector<CSubtitleRenderer::CEmbeddedGraphic >::erase(CSubtitleRenderer::CEmbeddedGraphic* __first, CSubtitleRenderer::CEmbeddedGraphic* __last);