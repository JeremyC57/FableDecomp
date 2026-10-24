// std::__chunk_insertion_sort(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* last, int chunk_size, CAnimSetItPriorityLessThan comp) -- insertion-sort [first, last) in chunks of chunk_size
// Leaf: ??$__chunk_insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007114C0, length 83 bytes (+ int3 padding). __fastcall (first ecx, last
//         edx), `ret 8`.
// Callees: std::__insertion_sort<...> @ 0x00711190 (landed as STL_insertionsort_00711190).
// Identity: the manifest label "CAnimSet_InsertionSortChunked" is descriptive. Single caller __merge_sort_with_buffer
//   0x007116B0 (chunk size 7).
//   The element is list<CAnimSet>::const_iterator (4 bytes, the node pointer; a user constructor makes it non-POD for
//   STLport's __type_traits), the comparator CAnimSetItPriorityLessThan reads CAnimSet::Priority (retail +0x4C, node
//   +0x54). This is the STLport stable_sort chain of the CAnimateThing TU (Ego_r.pdb: the Dinkumware twins _Stable_sort /
//   _Buffered_merge_sort / _Chunked_merge ... over _List_const_iterator<_List_val<CAnimSet> >* with CAnimSetItPriorityLessThan).
//   Retail STLport is in namespace std (RTTI `pair@...@std@@`); the algorithm TU is /Gr, so the templates are __fastcall.
// Shape: STLport `__chunk_insertion_sort` verbatim (`last` kept in the `push ecx` frame word).
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

struct CAnimSetItPriorityLessThan {                     // empty functor (a 4-byte argument slot)
    bool operator()(const CAnimSetConstIterator& a, const CAnimSetConstIterator& b) const { return (*a).Priority < (*b).Priority; }
};

namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);        // 0x00711190

template <class _RandomAccessIter, class _Distance, class _Compare>
void __fastcall __chunk_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Distance __chunk_size,
                                       _Compare __comp)
{
    while (__last - __first >= __chunk_size) {
        __insertion_sort(__first, __first + __chunk_size, __comp);
        __first += __chunk_size;
    }
    __insertion_sort(__first, __last, __comp);
}

template void __fastcall __chunk_insertion_sort<CAnimSetConstIterator*, int, CAnimSetItPriorityLessThan>(
    CAnimSetConstIterator*, CAnimSetConstIterator*, int, CAnimSetItPriorityLessThan);
}