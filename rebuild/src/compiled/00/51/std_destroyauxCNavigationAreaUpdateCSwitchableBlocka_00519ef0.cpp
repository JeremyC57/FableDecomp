// std::__destroy_aux<CNavigationAreaUpdate::CSwitchableBlockage*>(CSwitchableBlockage* first, CSwitchableBlockage* last, const __false_type&) -- STLport `_Destroy(first, last)` over a non-trivial range: run each element's destructor, which clears and frees its list
// Leaf: ??$__destroy_aux@PAVCSwitchableBlockage@CNavigationAreaUpdate@@@std@@YIXPAVCSwitchableBlockage@CNavigationAreaUpdate@@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00519EF0, length 92 bytes (cut 96 with padding), `ret 4` (the __false_type
//   reference on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Byte twin (another instance, different TU): 0x00A83420
// Identity: the element is Ego_r.pdb CNavigationAreaUpdate::CSwitchableBlockage {unsigned __int64 UniqueID +0;
//   list<C2DLineF> LineList +8} (16 bytes with the PC STLport 4-byte list), NOT the CNavQuadTree pair: every caller is
//   in the area-update TU -- the landed __destroy_aux<CNavigationLayerUpdate> 0x0051CD80 runs it over a layer's
//   SwitchableBlockages vector (+0xC), the landed vector<CSwitchableBlockage>::operator= 0x0051BCD0 and the
//   SwitchableBlockages push_back inlined in 0x00509A20 (layer +0xC) call it, as does that vector's _M_insert_overflow
//   0x0051C1F0. Its byte twin 0x00A83420 sits in the CNavQuadTree TU and is the pair<unsigned __int64, list<C2DLineF> >
//   instance (CNavigationLayer's CVectorMap, landed 0x00A85930); retail has no ICF and one symbol's COMDATs fold, so
//   the two bodies are two instances. Landed before as a plain `_global::__destroy_aux(PairU64LineList*, ...)`.
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

class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {
    public:
        unsigned __int64        UniqueID;               // +0x00
        std::list<C2DLineF>     LineList;               // +0x08 (16 bytes with the +0xC padding)
    };
};

template void __fastcall std::__destroy_aux<CNavigationAreaUpdate::CSwitchableBlockage*>(
    CNavigationAreaUpdate::CSwitchableBlockage*, CNavigationAreaUpdate::CSwitchableBlockage*, const std::__false_type&);
