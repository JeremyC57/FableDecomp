// std::__rotate_adaptive(CEngineSubPrimitiveRenderer::CSortElement* first, CSortElement* middle, CSortElement* last, int len1, int len2, CSortElement* buffer, int buffer_size) -- STLport stable-merge helper: rotate [first, middle, last) through the scratch buffer when the shorter side fits, else with the in-place rotate
// Leaf: ??$__rotate_adaptive@PAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@H@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@00HH0H@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B85CD0, 320 bytes (no padding after: the next function starts at once), __fastcall
//   (first ecx, middle edx; last, len1, len2, buffer, buffer_size on the stack), `ret 14h`.
// Identity: its only caller is __merge_adaptive<CSortElement*, int, CSortElement*, less<CSortElement> > 0x00B862C0 (bound
//   spelling, call at 0x00B8636B) -- STLport 4.6 _algo.c `__merge_adaptive` calls `__rotate_adaptive(first_cut, middle,
//   second_cut, len1 - len11, len22, buffer, buffer_size)`; the body is STLport 4.6 `__rotate_adaptive` verbatim with the
//   three copy / copy_backward calls expanded (8-byte two-dword moves, counted loops) and the fallback `rotate` going to the
//   out-of-line __rotate<CSortElement*, int, CSortElement> 0x00B85660 (`push 0 ; push 0`: the (int*)0 / (CSortElement*)0
//   type pointers, as in the landed __merge_without_buffer 0x00B86140). Element: the 8-byte CEngineSubPrimitiveRenderer::
//   CSortElement of that stable_sort chain.
// Callees: __rotate 0x00B85660.
// Shape: copy / copy_backward and their __copy_ptrs / __copy chains need `__forceinline` (plain `inline` leaves all six
//   calls out of line in this large body: 151v320).
class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct __false_type { __false_type() {} };

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
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                         _BidirectionalIter2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __fastcall __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                      _Distance*, _Tp*);                                           // 0x00B85660
template <class _RandomAccessIter, class _Distance>
inline _RandomAccessIter __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                  _Distance* __dis, const random_access_iterator_tag&)
{
    return __rotate(__first, __middle, __last, __dis, (CSortElement*)0);
}
template <class _ForwardIter>
inline _ForwardIter rotate(_ForwardIter __first, _ForwardIter __middle, _ForwardIter __last)
{
    return __rotate(__first, __middle, __last, (int*)0, random_access_iterator_tag());
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter1 __fastcall __rotate_adaptive(_BidirectionalIter1 __first, _BidirectionalIter1 __middle,
                                                 _BidirectionalIter1 __last, _Distance __len1, _Distance __len2,
                                                 _BidirectionalIter2 __buffer, _Distance __buffer_size)
{
    _BidirectionalIter2 __buffer_end;
    if (__len1 > __len2 && __len2 <= __buffer_size) {
        __buffer_end = copy(__middle, __last, __buffer);
        copy_backward(__first, __middle, __last);
        return copy(__buffer, __buffer_end, __first);
    }
    else if (__len1 <= __buffer_size) {
        __buffer_end = copy(__first, __middle, __buffer);
        copy(__middle, __last, __first);
        return copy_backward(__buffer, __buffer_end, __last);
    }
    else
        return rotate(__first, __middle, __last);
}

template CSortElement* __fastcall __rotate_adaptive<CSortElement*, CSortElement*, int>(CSortElement*, CSortElement*,
    CSortElement*, int, int, CSortElement*, int);
}