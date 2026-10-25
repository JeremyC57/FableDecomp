// std::__copy_ptrs<CInputEvent*,CInputEvent*>(CInputEvent* first, CInputEvent* last, CInputEvent* result, const __false_type&) -- STLport assignment copy of input events: assign [first, last) onto result in order (each 52-byte event copied as a 13-dword block) and return the advanced result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F63B0, length 60 bytes, `ret 8` (/Gr `__fastcall`: first in ecx, last in edx,
//   result and the empty tag reference on the stack).
// Leaf: ??$__copy_ptrs@
// Callers: vector<CInputEvent>::operator= @ 0x009F6890 (landed as CElement34Vector::operator=, call sites 0x009F6935 / 0x009F695C).
//   The chunk label "CInputEvent_Copy" is descriptive.
// Element: CInputEvent, 0x34 bytes, user copy constructor out of line (0x009E4850) but the IMPLICIT assignment, which VC7.1
//   lowers to `mov ecx,0Dh ; rep movsd`.
// Shape: STLport `__copy_ptrs(first, last, result, __false_type) { return __copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0); }` with the counted `__copy` inlined: the count is the signed
//   `(last - first) / 0x34` (0x4EC4EC4F multiply, `sar edx,4`), the source walk strength-reduced to `[result + (first - result)]`.
namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

}

class CInputEvent {                                   // 0x34 bytes
public:
    long m_words[13];
    CInputEvent(const CInputEvent& other);            // 0x009E4850
};

template CInputEvent* __fastcall std::__copy_ptrs<CInputEvent*, CInputEvent*>(CInputEvent* first, CInputEvent* last,
    CInputEvent* result, const std::__false_type&);