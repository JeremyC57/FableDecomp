// std::__stable_sort_adaptive(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CEngineSubPrimitiveRenderer::CSortElement* buffer, int buffer_size, less<CSortElement> comp) -- stable sort with a temporary buffer: merge-sort each half through the buffer (recursing while a half exceeds it), then merge the halves adaptively
// Leaf: ??$__stable_sort_adaptive@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B865E0, 145 bytes. __fastcall (first ecx, last edx), `ret 0Ch` (buffer,
//   buffer_size and the empty comparator on the stack).
// Callees: itself, std::__merge_sort_with_buffer<...> @ 0x00B86260, std::__merge_adaptive<...> @ 0x00B862C0.
// Callers: __stable_sort_aux 0x00B86710 and itself.
// Identity: the manifest label "std_introsort_heapsort_phase" is descriptive and wrong (no heap); STLport
//   `__stable_sort_adaptive`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace std (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `__stable_sort_adaptive` verbatim (landed 0x00711920 form): `(__last - __first + 1) / 2` as
//   inc ; cdq ; sub ; sar.
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
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __merge_sort_with_buffer(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                         _Distance*, _Compare __comp);                             // 0x00B86260
template <class _BidirectionalIter, class _Distance, class _Pointer, class _Compare>
void __fastcall __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                 _Distance __len1, _Distance __len2, _Pointer __buffer, _Distance __buffer_size,
                                 _Compare __comp);                                                 // 0x00B862C0

template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __stable_sort_adaptive(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                       _Distance __buffer_size, _Compare __comp)
{
    _Distance __len = (__last - __first + 1) / 2;
    _RandomAccessIter __middle = __first + __len;
    if (__len > __buffer_size) {
        __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size, __comp);
        __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size, __comp);
    }
    else {
        __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0, __comp);
        __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0, __comp);
    }
    __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), _Distance(__last - __middle), __buffer,
                     __buffer_size, __comp);
}

template void __fastcall __stable_sort_adaptive<CSortElement*, CSortElement*, int, less<CSortElement> >(CSortElement*,
    CSortElement*, CSortElement*, int, less<CSortElement>);
}