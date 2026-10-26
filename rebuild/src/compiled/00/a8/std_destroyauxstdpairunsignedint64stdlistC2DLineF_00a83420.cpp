// std::__destroy_aux<std::pair<unsigned __int64,std::list<C2DLineF> >*>(pair* first, pair* last, const __false_type&) -- STLport `_Destroy(first, last)` over a non-trivial range: run each element's destructor, which clears and frees its list
// Leaf: ??$__destroy_aux@PAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@@std@@YIXPAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83420, length 92 bytes, `ret 4` (the __false_type
//   reference on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Byte twin (another instance, different TU): 0x00519EF0
// Identity: the element is the 16-byte pair<unsigned __int64, list<C2DLineF> > of Ego_r.pdb CNavQuadTree::CNavigationLayer's
//   CVectorMap<unsigned __int64, list<C2DLineF> > SwitchableLineLists (the only Ego_r pair whose list sits at +8; with
//   the PC STLport 4-byte list it is exactly 16 bytes): its callers are the CNavQuadTree TU's layer __destroy_aux
//   0x00A85930 (landed) and that vector's growth / erase bodies 0x00A84C20, 0x00A851E0. The byte twin 0x00519EF0 is the
//   area-update TU's CNavigationAreaUpdate::CSwitchableBlockage instance (same layout, different type); retail has no
//   ICF, so the two bodies are two instances. Landed before as a plain `_global::__destroy_aux(PairU64LineList*, ...)`.
// Callee: free @ 0x00BFEA14 (the CRT import thunk; STLport's malloc allocator).
// Shape: STLport
//     for ( ; __first != __last; ++__first) _Destroy(&*__first);
//   with the element's implicit destructor reducing to the list destructor, inlined: clear() frees every node after the header
//   (`tmp = cur; cur = cur->_M_next; free(tmp)`, the header pointer re-read after each call), relinks the header to itself,
//   then the base destructor frees the header behind a null test.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class C2DLineF;

namespace std {
template <class _Tp> class allocator { };

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list: one header-node pointer
public:
    _List_node_base* _M_node;                           // +0x00

    void clear()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
};

struct __false_type { };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
}

template void __fastcall std::__destroy_aux<std::pair<unsigned __int64, std::list<C2DLineF> >*>(
    std::pair<unsigned __int64, std::list<C2DLineF> >*, std::pair<unsigned __int64, std::list<C2DLineF> >*,
    const std::__false_type&);
