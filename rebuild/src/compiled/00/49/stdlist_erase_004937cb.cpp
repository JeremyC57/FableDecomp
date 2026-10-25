// std::list::erase(iterator position) -- (list<CMessageEvent >) STLport 4.6 single-node erase: unlink the node, destroy its element through the element's virtual destructor, free the node and return the iterator to the following node
// Leaf: ?erase@?$list@VCMessageEvent@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004937CB, 43 bytes (+ int3 padding), thiscall, `ret 8` (hidden iterator result + the
//   iterator by value), returns the result pointer.
// Identity: list<CMessageEvent > -- proven; evidence: its only caller is the landed CMessageEventManager::RemoveMessage 0x00493B5B, erasing from the list at +4 = Ego_r.pdb CMessageEventManager::Messages (list<CMessageEvent>, after World +0); the element destroyed through vtable slot 0 is CMessageEvent (RTTI vtable 0x0125BE8C, 0x58 bytes); the PDB name sets hold list<CMessageEvent>::erase
//   Family G071 (7 byte-identical bodies, each its own COMDAT: retail has no ICF, so each address is one element type); the
//   manifest label "ListNode_Remove" is propagated/behavioural.
// Callees: the element's virtual destructor (vtable slot 0, pushed 0 = destroy only, no delete), free 0x00BFEA14 (the node
//   allocator's deallocate: no null test in retail).
// Shape: STLport 4.6 list `erase(iterator)`: next and prev read from the node, prev->_M_next = next, next->_M_prev = prev,
//   `_Destroy(&n->_M_data)` (explicit ~T() through the vtable), `_M_node.deallocate(n, 1)`, then `return iterator(next)`
//   stored through the hidden pointer; the iterator's user copy constructor makes it a by-value class argument. Size-optimised
//   module without a frame pointer (`pop ecx` cleans free's argument): optimize("s").
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CThing;
class CFaction;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // 8 bytes (Ego_r.pdb): vptr, PData
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
};
class CMessageEvent {                                    // 0x58 bytes, vtable 0x0125BE8C (Ego_r.pdb layout)
public:
    virtual ~CMessageEvent();                            // slot 0 (0x006872B0)
    long Unknown04;                                      // +0x04 (unnamed in the PDB)
    long Type;                                           // +0x08 EMessageEventType
    float Pos[3];                                        // +0x0C C3DVector
    float SightRadius;                                   // +0x18
    float HearingRadius;                                 // +0x1C
    unsigned __int64 OriginatorUID;                      // +0x20
    unsigned __int64 TargetUID;                          // +0x28
    CFaction* PTargetFaction;                            // +0x30
    long TimeStamp;                                      // +0x34
    unsigned int Lifespan;                               // +0x38
    long ExtraData;                                      // +0x3C
    unsigned int MessageID;                              // +0x40
    long ReactionSpeechType;                             // +0x44 EReactionSpeechType
    CIntelligentPointer<CThing> POriginator;             // +0x48
    CIntelligentPointer<CThing> PTarget;                 // +0x50
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };

template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

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

template list<CMessageEvent >::iterator list<CMessageEvent >::erase(iterator);

} // namespace std