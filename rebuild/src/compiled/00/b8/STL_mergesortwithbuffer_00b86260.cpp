// std::__merge_sort_with_buffer(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CEngineSubPrimitiveRenderer::CSortElement* buffer, int*, less<CSortElement> comp) -- bottom-up merge sort of [first, last) through the temporary buffer: insertion-sort 7-element chunks, then merge back and forth with doubling steps
// Leaf: ??$__merge_sort_with_buffer@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86260, 89 bytes. __fastcall (first ecx, last edx), `ret 0Ch` (buffer,
//   the distance-type tag pointer and the empty comparator on the stack).
// Callees: std::__chunk_insertion_sort<...> @ 0x00B85B90, std::__merge_sort_loop<...> @ 0x00B858D0.
// Callers: __stable_sort_adaptive 0x00B865E0 (twice).
// Identity: the manifest label "std_introsort_heapsort_phase" is descriptive and wrong (no heap); STLport
//   `__merge_sort_with_buffer`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace std (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `__merge_sort_with_buffer` verbatim (landed 0x007116B0 form): `__stl_chunk_size` = 7 (`mov esi,7`),
//   `__buffer_last` recomputed at its use (`lea edx,[ecx+edi*8]`), the step doubled twice per pass.
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
const int __stl_chunk_size = 7;

template <class _RandomAccessIter, class _Distance, class _Compare>
void __fastcall __chunk_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Distance __chunk_size,
                                       _Compare __comp);                                           // 0x00B85B90
template <class _RandomAccessIter1, class _RandomAccessIter2, class _Distance, class _Compare>
void __fastcall __merge_sort_loop(_RandomAccessIter1 __first, _RandomAccessIter1 __last, _RandomAccessIter2 __result,
                                  _Distance __step_size, _Compare __comp);                         // 0x00B858D0

template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __merge_sort_with_buffer(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                         _Distance*, _Compare __comp)
{
    _Distance __len = __last - __first;
    _Pointer __buffer_last = __buffer + __len;

    _Distance __step_size = __stl_chunk_size;
    __chunk_insertion_sort(__first, __last, __step_size, __comp);

    while (__step_size < __len) {
        __merge_sort_loop(__first, __last, __buffer, __step_size, __comp);
        __step_size *= 2;
        __merge_sort_loop(__buffer, __buffer_last, __first, __step_size, __comp);
        __step_size *= 2;
    }
}

template void __fastcall __merge_sort_with_buffer<CSortElement*, CSortElement*, int, less<CSortElement> >(CSortElement*,
    CSortElement*, CSortElement*, int*, less<CSortElement>);
}