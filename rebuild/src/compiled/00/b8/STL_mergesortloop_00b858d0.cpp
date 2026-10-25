// std::__merge_sort_loop(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CEngineSubPrimitiveRenderer::CSortElement* result, int step_size, less<CSortElement> comp) -- merge adjacent step-size runs of [first, last) into result
// Leaf: ??$__merge_sort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B858D0, 197 bytes. __fastcall (first ecx, last edx), `ret 0Ch` (result,
//   step_size and the empty comparator on the stack).
// Callees: std::merge<CSortElement*, CSortElement*, CSortElement*, less<CSortElement> > @ 0x00B85510 (fastcall, `ret 10h`).
// Callers: __merge_sort_with_buffer 0x00B86260 (twice per pass).
// Identity: the manifest label "std_introsort_merge" is descriptive; STLport `__merge_sort_loop`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace std (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `__merge_sort_loop` verbatim (landed 0x007107D0 form): `__two_step = 2 * __step_size`, the
//   strength-reduced loop, then `(min)(_Distance(__last - __first), __step_size)` by const reference -- the difference is
//   stored into the dead `result` argument slot and the address of the winner selected (`lea ecx,[esp+20h] / [esp+1Ch]`).
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

namespace std {
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

namespace std {
template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }

template <class _InputIter1, class _InputIter2, class _OutputIter, class _Compare>
_OutputIter __fastcall merge(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result, _Compare __comp);                               // 0x00B85510

template <class _RandomAccessIter1, class _RandomAccessIter2, class _Distance, class _Compare>
void __fastcall __merge_sort_loop(_RandomAccessIter1 __first, _RandomAccessIter1 __last, _RandomAccessIter2 __result,
                                  _Distance __step_size, _Compare __comp)
{
    _Distance __two_step = 2 * __step_size;

    while (__last - __first >= __two_step) {
        __result = merge(__first, __first + __step_size, __first + __step_size, __first + __two_step, __result, __comp);
        __first += __two_step;
    }
    __step_size = (min)(_Distance(__last - __first), __step_size);

    merge(__first, __first + __step_size, __first + __step_size, __last, __result, __comp);
}

template void __fastcall __merge_sort_loop<CSortElement*, CSortElement*, int, less<CSortElement> >(CSortElement*,
    CSortElement*, CSortElement*, int, less<CSortElement>);
}