// std::vector::erase(iterator first, iterator last) -- (vector<CSubtitleRenderer::CEmbeddedHighlight >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy the vacated tail through the out-of-line __destroy_aux, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCEmbeddedHighlight@CSubtitleRenderer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004940AC, 55 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 8`, returns first.
// Identity: std::vector<CSubtitleRenderer::CEmbeddedHighlight >::erase(iterator, iterator) -- proven; evidence: its callers are CSubtitleRenderer::ProcessEmbeddedHighlightTags 0x00491AD2, which erases [begin, end) of the vector at +0xC4 = Ego_r.pdb CSubtitleRenderer::EmbeddedHighlights (vector<CSubtitleRenderer::CEmbeddedHighlight>; the larger STLport deque and the smaller vectors cancel at that offset), and the clear() 0x004941B5; the 0x14-byte element is the PDB CEmbeddedHighlight {CWideString Text; long CharIndex; C2DVector Offset; bool Split} (operator= 0x004936EC: CWideString::operator= 0x0099B7D0, three dwords, one byte; the destroy loop runs ~CWideString at +0); the PDB name sets hold vector<CSubtitleRenderer::CEmbeddedHighlight>::erase
//   Family G072 (seven byte-identical bodies, one COMDAT per T: retail has no ICF). Catalogue label "Std_Vector_Insert_CopyHighlights" is propagated/behavioural.
// Callees: std::__copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00493A5A (`__fastcall`: first in
//   ecx, last in edx, result / tag reference / null distance pointer on the stack, `ret 0Ch`; 0x14-byte elements, the element's
//   operator= 0x004936EC); std::__destroy_aux<T*>(first, last, const __false_type&) 0x00440BA0 (`__fastcall`, `ret 4`; the implicit destructor's ~CWideString 0x0099B510 on each element).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the family G067 body with __copy left out of line: both callees are same-TU noinline
//   `__fastcall` bodies that never touch their tag, so VC7.1 homes both tags in the live `first` slot ([ebp+0Bh]) as retail does.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
class CWideString {                                          // 4 bytes (Ego_r.pdb)
public:
    void* PStringData;                                       // +0x0 CWideStringData*
    const CWideString& operator=(const CWideString& other);        // 0x0099B7D0
    ~CWideString();                                          // 0x0099B510
};
class C2DVector { public: float X; float Y; };                // 8 bytes
class CSubtitleRenderer {
public:
    class CEmbeddedHighlight {                               // 0x14 bytes (Ego_r.pdb)
    public:
        CWideString Text;                                    // +0x00
        long        CharIndex;                               // +0x04
        C2DVector   Offset;                                  // +0x08
        bool        Split;                                   // +0x10
    };
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

#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>                   // 0x00493A5A (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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

template <class _Tp>
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>                                                        // 0x00440BA0 (same-TU body)
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

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

typedef CSubtitleRenderer::CEmbeddedHighlight _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);

} // namespace std