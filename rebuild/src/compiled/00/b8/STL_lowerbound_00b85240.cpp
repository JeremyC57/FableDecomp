// std::__lower_bound(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, const CSortElement& val, less<CSortElement> comp, int*) -- STLport bisection for the first sort element not below val
// Leaf: ??$__lower_bound@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B85240, 54 bytes (+ int3 padding). __fastcall (first ecx -> eax, last
//   edx), `ret 0Ch` (&val, the empty comparator by value and the null distance-type pointer on the stack).
// Callers: 0x00B854AC (__rotate-side caller 0x00B85430), __merge_without_buffer 0x00B86140 (0x00B8619D), __merge_adaptive
//   0x00B862C0 (0x00B86310).
// Identity: the manifest label "BinarySearch_Lower" is descriptive; STLport `__lower_bound` (lower_bound(first, last, val,
//   comp) front end inlined).
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace std (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport bisection (landed 0x008174B0 form): `len = last - first`, `half = len >> 1`, middle = first + half,
//   `comp(*middle, val)` (unsigned `cmp [eax+edx*8],ecx ; jae`) -> first = middle + 1, `len = len - half - 1` (the
//   `or edi,-1 ; sub edi,edx` form), else len = half; val.SortValue is loaded once ahead of the loop.
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
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp,
                                      _Distance*)
{
    _Distance __len = __last - __first;
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        __middle += __half;
        if (__comp(*__middle, __val)) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
            __len = __half;
    }
    return __first;
}

template CSortElement* __fastcall __lower_bound<CSortElement*, CSortElement, less<CSortElement>, int>(CSortElement*,
    CSortElement*, const CSortElement&, less<CSortElement>, int*);
}