// std::_List_base::clear() -- (_List_base<CMessageEvent, allocator<CMessageEvent> >) STLport 4.6 list clear: walk from the first node to the sentinel, destroy each node's element through its virtual destructor and free the node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@VCMessageEvent@@V?$allocator@VCMessageEvent@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A9CF0, 58 bytes (+ int3 padding), thiscall, `ret`.
// Identity: std::_List_base<CMessageEvent >::clear() -- proven; evidence: CWorld::PrepareForLoad 0x004A0220 calls it on [CWorld+0x60]+4 = PMessageEventManager.Data (Ego_r.pdb CWorld +0x60) -> CMessageEventManager::Messages (+4, list<CMessageEvent>; G071's erase 0x004937CB works on the same list), as do the CMessageEventManager scalar deleting destructor 0x004AD650 (list at +4, then the sentinel and the object freed), the +4 forwarder 0x004AB240, 0x004ADC40 and the unreferenced list destructors 0x004AB610 / 0x004AC250; the element is polymorphic (CMessageEvent RTTI vtable 0x0125BE8C); the PDB name sets hold list<CMessageEvent>::clear
//   Family G073 (seven byte-identical bodies, one COMDAT per T: retail has no ICF); the manifest label "SortTreeRecursively" is
//   propagated across the identical bodies (the landed sibling 0x00882C40 is the same body for a third element type).
// Callees: the element's virtual destructor (vtable slot 0, pushed 0 = destroy only, no delete), free 0x00BFEA14 (the node
//   allocator's deallocate: no null test).
// Body: STLport 4.6 list.c `_List_base<_Tp, _Alloc>::clear()` -- the out-of-line member that list::clear() and
//   ~_List_base() call (the unreferenced list destructors beside each instance call it, then free the sentinel); the
//   sentinel is re-read from `this` after every free.
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

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
    void clear();
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

typedef CMessageEvent _Elem;
template void _List_base<_Elem, allocator<_Elem> >::clear();

} // namespace std