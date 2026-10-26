// std::__stable_sort_adaptive(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* last, list<CAnimSet>::const_iterator* buffer, int buffer_size, CAnimSetItPriorityLessThan comp) -- split in halves (recursing while a half exceeds the buffer), sort each with the buffer and merge them adaptively
// Leaf: ??$__stable_sort_adaptive@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711920, length 145 bytes (+ int3 padding). __fastcall (first ecx, last
//         edx), `ret 0Ch`.
// Callees: itself, std::__merge_sort_with_buffer<...> @ 0x007116B0, std::__merge_adaptive<...> @ 0x00711340.
// Identity: the manifest label "CAnimSet_IntrosortRecursive" is wrong (no introsort: a stable merge sort). Called by
//   __stable_sort_aux 0x00711B80 and recursively.
//   The element is list<CAnimSet>::const_iterator (4 bytes, the node pointer; a user constructor makes it non-POD for
//   STLport's __type_traits), the comparator CAnimSetItPriorityLessThan reads CAnimSet::Priority (retail +0x4C, node
//   +0x54). This is the STLport stable_sort chain of the CAnimateThing TU (Ego_r.pdb: the Dinkumware twins _Stable_sort /
//   _Buffered_merge_sort / _Chunked_merge ... over _List_const_iterator<_List_val<CAnimSet> >* with CAnimSetItPriorityLessThan).
//   Retail STLport is in namespace std (RTTI `pair@...@std@@`); the algorithm TU is /Gr, so the templates are __fastcall.
// Shape: STLport `__stable_sort_adaptive` verbatim (`(__last - __first + 1) / 2` as inc ; cdq ; sub ; sar).
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
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __merge_sort_with_buffer(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                         _Distance*, _Compare __comp);                                         // 0x007116B0
template <class _BidirectionalIter, class _Distance, class _Pointer, class _Compare>
void __fastcall __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                 _Distance __len1, _Distance __len2, _Pointer __buffer, _Distance __buffer_size,
                                 _Compare __comp);                                                             // 0x00711340

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

template void __fastcall __stable_sort_adaptive<CAnimSetConstIterator*, CAnimSetConstIterator*, int, CAnimSetItPriorityLessThan>(
    CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetConstIterator*, int, CAnimSetItPriorityLessThan);
}