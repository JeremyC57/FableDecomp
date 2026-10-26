// std::__merge_sort_with_buffer(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* last, list<CAnimSet>::const_iterator* buffer, int*, CAnimSetItPriorityLessThan comp) -- chunked insertion sort, then merge passes ping-ponging between the range and the buffer
// Leaf: ??$__merge_sort_with_buffer@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007116B0, length 89 bytes (+ int3 padding). __fastcall (first ecx, last
//         edx), `ret 0Ch`.
// Callees: std::__chunk_insertion_sort<...> @ 0x007114C0, std::__merge_sort_loop<...> @ 0x007107D0.
// Identity: the manifest label "CAnimSet_InsertionSort" is descriptive. Called twice by __stable_sort_adaptive 0x00711920.
//   The element is list<CAnimSet>::const_iterator (4 bytes, the node pointer; a user constructor makes it non-POD for
//   STLport's __type_traits), the comparator CAnimSetItPriorityLessThan reads CAnimSet::Priority (retail +0x4C, node
//   +0x54). This is the STLport stable_sort chain of the CAnimateThing TU (Ego_r.pdb: the Dinkumware twins _Stable_sort /
//   _Buffered_merge_sort / _Chunked_merge ... over _List_const_iterator<_List_val<CAnimSet> >* with CAnimSetItPriorityLessThan).
//   Retail STLport is in namespace std (RTTI `pair@...@std@@`); the algorithm TU is /Gr, so the templates are __fastcall.
// Shape: STLport `__merge_sort_with_buffer` verbatim: `__stl_chunk_size` = 7 (`mov esi,7`), `__buffer_last` recomputed at
//   its use (`lea edx,[ecx+edi*4]`), the step doubled twice per pass.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CAnimSet {                                        // retail layout (Ego_r.pdb minus 8: 12-byte vectors)
public:
    unsigned char _before[0x4C];                        // UpdateMode, Anims, InternalAlphas, ... AnimDelay
    long Priority;                                      // +0x4C
    unsigned char _after[0x08];                         // Type, the flag bits
};

namespace std {
template <class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base() {}
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
};
}
typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetConstIterator;   // list<CAnimSet>::const_iterator

class CAnimSetItPriorityLessThan { public:                     // empty functor (a 4-byte argument slot)
    bool operator()(const CAnimSetConstIterator& a, const CAnimSetConstIterator& b) const { return (*a).Priority < (*b).Priority; }
};

namespace std {
const int __stl_chunk_size = 7;

template <class _RandomAccessIter, class _Distance, class _Compare>
void __fastcall __chunk_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Distance __chunk_size,
                                       _Compare __comp);                                                       // 0x007114C0
template <class _RandomAccessIter1, class _RandomAccessIter2, class _Distance, class _Compare>
void __fastcall __merge_sort_loop(_RandomAccessIter1 __first, _RandomAccessIter1 __last, _RandomAccessIter2 __result,
                                  _Distance __step_size, _Compare __comp);                                     // 0x007107D0

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

template void __fastcall __merge_sort_with_buffer<CAnimSetConstIterator*, CAnimSetConstIterator*, int, CAnimSetItPriorityLessThan>(
    CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*, int*, CAnimSetItPriorityLessThan);
}