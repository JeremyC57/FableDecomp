// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData, CMessageEventFilter_OccuredBetween> >(const filter&) const -- the first (most recent) queued message all three filters accept, or null
// Leaf: ??$FindMostRecentMessage@V?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@VCMessageEventFilter_HasExtraData@@VCMessageEventFilter_OccuredBetween@@@@@CMessageEventManager@@QBEPBVCMessageEvent@@ABV?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@VCMessageEventFilter_HasExtraData@@VCMessageEventFilter_OccuredBetween@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ABF20, 85 bytes, thiscall, `ret 4`.
// Identity (proven): the three out-of-line filter calls are CMessageEventFilter_IsOfType / HasExtraData / OccuredBetween::operator() (0x006AF1C0,
//   0x006AF1A0, 0x006AF180: their bodies test the Ego_r.pdb CMessageEvent fields of those filters); Ego_r.pdb names this
//   instantiation (donor 0x008E9D70); only caller the landed CGameScriptInterface::MsgIsBoastMade 0x00893960.
// Shape (family K034, representative 0x008ABE70): STLport list walk from begin() with the end node re-read for each test,
//   CFilter_Bind3's inline operator() `(*F1)(m) && (*F2)(m) && (*F3)(m)` on the filter's member pointers, the matching
//   message as a named pointer local -- the landed Bind4 sibling 0x00959040 with three filters.
class CThingAICreature;
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
class CMessageEvent;
class CMessageEventFilter_IsOfType {                    // Ego_r.pdb
public:
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
    EMessageEventType Type;                              // +0x00
};
class CMessageEventFilter_HasExtraData {                    // Ego_r.pdb
public:
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1A0
    long ExtraData;                                      // +0x00
};
class CMessageEventFilter_OccuredBetween {                    // Ego_r.pdb
public:
    bool operator()(const CMessageEvent& message) const;   // 0x006AF180
    long MinFrame;                                       // +0x00
    long MaxFrame;                                       // +0x04
};
template <class TF1, class TF2, class TF3> class CFilter_Bind3 {   // Ego_r.pdb: F1, F2, F3 (12 bytes)
public:
    TF1* F1;                                             // +0x00
    TF2* F2;                                             // +0x04
    TF3* F3;                                             // +0x08
    bool operator()(const CMessageEvent& message) const { return (*F1)(message) && (*F2)(message) && (*F3)(message); }
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
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData, CMessageEventFilter_OccuredBetween> TFilter;
template const CMessageEvent* CMessageEventManager::FindMostRecentMessage<TFilter>(const TFilter&) const;