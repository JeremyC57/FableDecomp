// std::list<NPlayerGui::CStatBar::CChangeText>::erase(iterator position) -- unlink one node, destroy its change text, free the node, return the next position
// Leaf: ?erase@?$list@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00655A52, 44 bytes, thiscall `ret 8` (hidden iterator result + the iterator by
// value; `this` is never read). Only caller: NPlayerGui::CStatBar::UpdateChangeTexts @ 0x00646C30 (the
// `it = ChangeTexts.erase(it)` of a finished change text; Ego_r.pdb: CStatBar::ChangeTexts is a
// list<CStatBar::CChangeText> at +0x6C).
// Callees: CChangeText's `scalar deleting destructor' @ 0x0043F750 (given 0: the value's IMPLICIT destructor, which runs
// ~CWideString on Text +0x10 -- landed as CRecordWithWideStringAt10), free @ 0x00BFEA14 (the node allocator).
// Label correction: the catalog "CLandscapeBackground_RemoveNode" is propagated; this is STLport list::erase.
// Shape: STLport 4.x verbatim: next/prev read from the node, `prev->_M_next = next ; next->_M_prev = prev`,
// `_Destroy(&n->_M_data)`, `_M_node.deallocate(n, 1)`, `return iterator(next)` (an iterator with user constructors is
// returned through the hidden pointer). An explicit destructor call on a class whose destructor is implicit and
// non-trivial goes through its `??_G` with 0 (GOTCHAS, 0x579D18).
// Layout: node next +0, prev +4, CChangeText +8 (0x24 bytes: OldOffset, Offset, Text +0x10, Value, Colour,
// StartToEnd, PercentageThrough).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class C2DVector { public: float X, Y; };
class CRGBColour { public: unsigned long Colour; };
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                  // 0x0099B510
};

namespace NPlayerGui {
class CStatBar {
public:
    class CChangeText {                              // Ego_r.pdb, 0x24 bytes; implicit destructor
    public:
        C2DVector   OldOffset;                       // +0x00
        C2DVector   Offset;                          // +0x08
        CWideString Text;                            // +0x10
        long        Value;                           // +0x14
        CRGBColour  Colour;                          // +0x18
        bool        StartToEnd;                      // +0x1c
        float       PercentageThrough;               // +0x20
    };
};
}

namespace std {

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                        // +0x00
    _List_node_base* _M_prev;                        // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                     // +0x08
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x00 sentinel

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
};

template class list<NPlayerGui::CStatBar::CChangeText>;
}