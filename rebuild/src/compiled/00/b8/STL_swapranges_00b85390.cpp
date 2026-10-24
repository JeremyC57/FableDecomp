// _STL::swap_ranges(CEngineSubPrimitiveRenderer::CSortElement* first1, CEngineSubPrimitiveRenderer::CSortElement* last1, CEngineSubPrimitiveRenderer::CSortElement* first2) -- swap the sort elements of [first1, last1) with the range starting at first2; returns the end of the second range
// Leaf: ??$swap_ranges@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B85390, 52 bytes. __fastcall (first1 ecx, last1 edx), `ret 4` (first2 on
//   the stack, returned in eax).
// Callees: none.
// Identity: the manifest label "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is a propagated fold label (a swap,
//   not a copy); STLport `swap_ranges`, used by the random-access __rotate of the in-place stable_sort merge.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace _STL (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `for (; first1 != last1; ++first1, ++first2) iter_swap(first1, first2);` with __iter_swap's
//   `_Tp tmp = *i1; *i1 = *i2; *i2 = tmp;` inlined (landed 0x0070FDC0 form).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* subPrimitive)
            : SortValue(sortValue), SubPrimitive(subPrimitive) {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace _STL {
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2, _Tp*)
{
    _Tp __tmp = *__i1;
    *__i1 = *__i2;
    *__i2 = __tmp;
}
template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 __fastcall swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2)
{
    for ( ; __first1 != __last1; ++__first1, ++__first2)
        iter_swap(__first1, __first2);
    return __first2;
}

template CSortElement* __fastcall swap_ranges<CSortElement*, CSortElement*>(CSortElement*, CSortElement*, CSortElement*);
}