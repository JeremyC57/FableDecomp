// _STL::list<CAnimSet>::remove_if(CAnimSet::CIsType pred) -- erase every queued animation set whose Type byte equals the predicate's type (each erased node frees its two vector buffers, then the node)
// Leaf: ??$remove_if@VCIsType@CAnimSet@@@?$list@VCAnimSet@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711800, 88 bytes (+ int3 padding), thiscall, `ret 4` (the one-word predicate).
// Identity: FableWin.pdb `??$remove_if@VCIsType@CAnimSet@@@?$list@VCAnimSet@@V?$allocator@VCAnimSet@@@std@@@std@@QAEXVCIsType@CAnimSet@@@Z`;
//   callers 0x0070D8CC / 0x0070D8D5 (CSpellContainerList::AddChild's catalog row, two lists in a row) and the jmp 0x00711B50.
// Callees: free @ 0x00BFEA14 (InternalAlphas' and Anims' buffers, then the node).
// Layout: Ego_r.pdb CAnimSet with retail 12-byte STLport vectors (PDB offsets - 8 after Anims): Anims +0x04, InternalAlphas
//   +0x10, TypeFlags +0x44, Type +0x50 (uchar); node = next, prev, CAnimSet (+8).
// Shape: STLport 4.6 `remove_if` + 4.6 `erase` (as 0x0073E790); `set.Type == Type` zero-extends the byte against the
//   predicate's long, which stays in ebp; the implicit ~CAnimSet frees InternalAlphas then Anims (`if (start) free(start)`).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace _STL {
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

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }                                           // _Destroy over a trivial element type: nothing
};
}

class CAnimateThing;
enum EAnimUpdateMode { ANIM_UPDATE_MODE_NONE };

class CTransitionBlendAlpha {                               // Ego_r.pdb, 8 bytes
public:
    float CurrentAlpha;                                     // +0x0
    float LastAlpha;                                        // +0x4
};

class CAnimSet {                                            // Ego_r.pdb with the retail 12-byte STLport vectors: 0x54 bytes
public:
    EAnimUpdateMode                        UpdateMode;                 // +0x00
    _STL::vector<short>                    Anims;                      // +0x04
    _STL::vector<CTransitionBlendAlpha>    InternalAlphas;             // +0x10
    float                                  CurrentPercentageComplete;  // +0x1c
    float                                  LastPercentageComplete;     // +0x20
    float                                  Speed;                      // +0x24
    unsigned long                          FrameInternalAlphasUpdated; // +0x28
    float                                  CurrentAlpha;               // +0x2c
    float                                  LastAlpha;                  // +0x30
    float                                  LastUpdateChange;           // +0x34
    float                                  ChangeAlphaBy;              // +0x38
    CAnimateThing*                         PAnimator;                  // +0x3c
    unsigned long                          Flags;                      // +0x40
    unsigned long                          TypeFlags;                  // +0x44
    long                                   AnimDelay;                  // +0x48
    long                                   Priority;                   // +0x4c
    unsigned char                          Type;                       // +0x50
    bool                                   Looping : 1;                // +0x51
    bool                                   Finished : 1;
    bool                                   Forwards : 1;
    bool                                   HadUpdate : 1;
    bool                                   OwnedByMainAnim : 1;

    class CIsType {                                         // Ego_r.pdb: Type +0
    public:
        long Type;
        CIsType(long type) : Type(type) { }
        bool operator()(const CAnimSet& set) const { return set.Type == Type; }
    };
    class CHasTypeFlags {                                   // Ego_r.pdb: Flags +0
    public:
        long Flags;
        CHasTypeFlags(long flags) : Flags(flags) { }
        bool operator()(const CAnimSet& set) const { return (set.TypeFlags & Flags) != 0; }
    };
};

namespace _STL {
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};
}

template void _STL::list<CAnimSet>::remove_if(CAnimSet::CIsType);