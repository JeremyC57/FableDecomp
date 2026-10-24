// std::_Temporary_buffer::_M_allocate_buffer() -- (the buffer of list<CAnimSet>::const_iterator elements) remember the requested length, clamp it to INT_MAX / 4 and try malloc, halving the length until a buffer is found or it reaches 0
// Leaf: ?_M_allocate_buffer@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070F410, length 75 bytes. __fastcall, plain `ret`.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E.
// Identity: the manifest label "Vector_Reallocate_DownsizingLoop" is descriptive. this = {_M_original_len, _M_len, _M_buffer};
//   called from the inlined _Temporary_buffer constructor in __stable_sort_aux 0x00711B80 and from 0x00710774.
//   The element is list<CAnimSet>::const_iterator (4 bytes, the node pointer; a user constructor makes it non-POD for
//   STLport's __type_traits), the comparator CAnimSetItPriorityLessThan reads CAnimSet::Priority (retail +0x4C, node
//   +0x54). This is the STLport stable_sort chain of the CAnimateThing TU (Ego_r.pdb: the Dinkumware twins _Stable_sort /
//   _Buffered_merge_sort / _Chunked_merge ... over _List_const_iterator<_List_val<CAnimSet> >* with CAnimSetItPriorityLessThan).
//   Retail STLport is in namespace std (RTTI `pair@...@std@@`); the algorithm TU is /Gr, so the templates are __fastcall.
// Shape: STLport `_M_allocate_buffer` verbatim (`_M_len > (ptrdiff_t)(INT_MAX / sizeof(_Tp))` -> 0x1FFFFFFF, `_M_len /= 2`
//   as cdq ; sub ; sar).
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
template <class _ForwardIterator, class _Tp>
class _Temporary_buffer {
private:
    int _M_original_len;
    int _M_len;
    _Tp* _M_buffer;
    void _M_allocate_buffer()
    {
        _M_original_len = _M_len;
        _M_buffer = 0;
        if (_M_len > (int)(0x7FFFFFFF / sizeof(_Tp)))   // INT_MAX / sizeof(_Tp)
            _M_len = 0x7FFFFFFF / sizeof(_Tp);
        while (_M_len > 0) {
            _M_buffer = (_Tp*)malloc(_M_len * sizeof(_Tp));
            if (_M_buffer)
                break;
            _M_len /= 2;
        }
    }
};

template class _Temporary_buffer<CAnimSetConstIterator*, CAnimSetConstIterator>;
}