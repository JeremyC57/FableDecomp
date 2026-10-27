// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatorAlive> >(const filter&) const -- the first (most recent) queued message both filters accept, or null
// Leaf: ??$FindMostRecentMessage@V?$CFilter_Bind2@VCMessageEventFilter_IsOfType@@VCMessageEventFilter_IsOriginatorAlive@@@@@CMessageEventManager@@QBEPBVCMessageEvent@@ABV?$CFilter_Bind2@VCMessageEventFilter_IsOfType@@VCMessageEventFilter_IsOriginatorAlive@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00939C60, 85 bytes, thiscall, `ret 4`.
// Identity (proven): the two out-of-line filter calls are CMessageEventFilter_IsOfType / IsOriginatorAlive::operator() (0x006AF1C0,
//   0x006AF2D0: their bodies test the Ego_r.pdb CMessageEvent fields of those filters); Ego_r.pdb names this
//   instantiation (donor 0x0095CFF0); only caller CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsAnotherGuardAdmonishingPlayer 0x00939D00.
// Shape (family K035, representative 0x008ABED0): STLport list walk from begin() with the end node re-read for each test,
//   CFilter_Bind2's inline operator() `(*F1)(m) && (*F2)(m)` on the filter's member pointers, the matching message as a
//   named pointer local (family K034's Bind3 text with two filters).
class CThingAICreature;
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
class CMessageEvent;
class CMessageEventFilter_IsOfType {                    // Ego_r.pdb
public:
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
    EMessageEventType Type;                              // +0x00
};
class CMessageEventFilter_IsOriginatorAlive {                    // Ego_r.pdb
public:
    bool operator()(const CMessageEvent& message) const;   // 0x006AF2D0
};
template <class TF1, class TF2> class CFilter_Bind2 {   // Ego_r.pdb: F1, F2 (8 bytes)
public:
    TF1* F1;                                             // +0x00
    TF2* F2;                                             // +0x04
    bool operator()(const CMessageEvent& message) const { return (*F1)(message) && (*F2)(message); }
};
class CMessageEvent {                                    // Ego_r.pdb, 88 bytes (only its address is used here)
public:
    unsigned char _body[0x58];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {     // STLport list, 4 bytes
public:
    struct const_iterator {
        _List_node_base* _M_node;
        const_iterator(_List_node_base* __x) : _M_node(__x) { }
        const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& __y) const { return _M_node != __y._M_node; }
    };
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;   // +0x00
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
};
}
class CWorld;
class CMessageEventManager {                             // Ego_r.pdb 16 bytes; retail 8 (4-byte STLport list)
public:
    CWorld* World;                                       // +0x00
    std::list<CMessageEvent> Messages;                   // +0x04
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;
};
template <class TFilter> const CMessageEvent* CMessageEventManager::FindMostRecentMessage(const TFilter& filter) const
{
    for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it) {
        const CMessageEvent* message = &*it;
        if (filter(*message))
            return message;
    }
    return 0;
}
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatorAlive> TFilter;
template const CMessageEvent* CMessageEventManager::FindMostRecentMessage<TFilter>(const TFilter&) const;