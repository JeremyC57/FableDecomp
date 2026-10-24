// _STL::__destroy_aux(CLineList* first, CLineList* last, const __false_type&) -- destroy a range of lists of lines: free every node of each list, re-link its sentinel to itself, then free the sentinel
// Leaf: ??$__destroy_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A827D0, 80 bytes, `ret 4` (/Gr: first in ecx, last in
//   edx; the tag reference on the stack, never read).
// Callees: free 0x00BFEA14 (the 0x18-byte nodes, then the sentinel).
// Identity: element = list<C2DLineF> -- CNavigationLayer::LineLists (Ego_r.pdb vector<list<C2DLineF> >); C2DLineF is
//   trivially destructible, so only the node frees remain. Catalog "DestroyBinaryTreeRange" is descriptive (wrong).
// Shape: STLport 4.6 verbatim -- `for (; first != last; ++first) _Destroy(&*first);` with the list destructor
//   (`~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }`) and `_List_base::clear()` inline: the
//   sentinel is re-read from the list after every free and every link store; node frees carry no null test (the node
//   was just dereferenced), the sentinel's deallocate keeps it.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class C2DVector {
public:
    float X;                                                // +0
    float Y;                                                // +4
};
class C2DLineF {                                            // 16 bytes
public:
    C2DVector Start;                                        // +0x0
    C2DVector End;                                          // +0x8
};

namespace _STL {
class __false_type { public: __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
};
}

typedef _STL::list<C2DLineF> CLineList;                     // 4 bytes (0x18-byte nodes)

namespace _STL {
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template void __fastcall __destroy_aux(CLineList*, CLineList*, const __false_type&);
}