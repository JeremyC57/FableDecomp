// CMessageEventManager::FindMostRecentMessage<CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatorAlive, unary_negate<CMessageEventFilter_IsOriginatedByThing>, CMessageEventFilter_HasExtraData> >(const Filter& filter) const -- the most recent queued message of the type, from a live originator that is not the given thing, carrying extra data (0 if none)
// Leaf: FindMostRecentMessage
// Retail: Fable.exe @ 0x00959040, 96 bytes, `__fastcall`, `ret 4`.
// Callees (all out of line, each on its filter pointer in ecx): CMessageEventFilter_IsOfType::operator() 0x006AF1C0,
// CMessageEventFilter_IsOriginatorAlive::operator() 0x006AF2D0, CMessageEventFilter_IsOriginatedByThing::operator() 0x006AF200
// (negated through std::unary_negate: `jne skip`), CMessageEventFilter_HasExtraData::operator() 0x006AF1A0. (The manifest names
// 0x006AF1C0 "HasExtraData" -- propagated; the landed Bind4 sibling 0x0090E2E0 uses 0x006AF1C0 as IsOfType.)
// Layout / shape: as the landed sibling CMessageEventManager_FindMostRecentMessageBind40090e2e0 0x0090E2E0: the retail
// one-pointer message list at +0x04 (node next +0, prev +4, value +8), the filter's four member pointers, the matching
// message as a named pointer local; the head re-read for each end test. std::unary_negate holds its predicate by value, so
// its address is the PF3 pointer itself.
struct CMessageEvent {
    unsigned char _pad00[0x48];
    unsigned char POriginator[8];               // +0x48 CIntelligentPointer<CThing>
};
struct CMessageEventFilter_IsOfType {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
};
struct CMessageEventFilter_IsOriginatorAlive {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF2D0
};
struct CMessageEventFilter_IsOriginatedByThing {
    typedef CMessageEvent argument_type;
    bool operator()(const CMessageEvent& message) const;   // 0x006AF200
};
struct CMessageEventFilter_HasExtraData {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1A0
};
namespace std {
template <class Predicate> class unary_negate {             // STLport: the predicate by value (empty unary_function base)
protected:
    Predicate _M_pred;
public:
    explicit unary_negate(const Predicate& x) : _M_pred(x) { }
    bool operator()(const typename Predicate::argument_type& x) const { return !_M_pred(x); }
};
}
template <class F1, class F2, class F3, class F4> struct CFilter_Bind4 {
    F1* PF1;                                    // +0x00
    F2* PF2;                                    // +0x04
    F3* PF3;                                    // +0x08
    F4* PF4;                                    // +0x0C
    __forceinline bool operator()(const CMessageEvent& message) const
    {
        return (*PF1)(message) && (*PF2)(message) && (*PF3)(message) && (*PF4)(message);
    }
};
struct CMessageList {                            // retail one-pointer list
    struct node { node* next; node* prev; CMessageEvent value; };
    node* head;
};
struct CWorld;
struct CMessageEventManager {
    CWorld*      World;                         // +0x00
    CMessageList Messages;                      // +0x04
    template <class Filter> const CMessageEvent* FindMostRecentMessage(const Filter& filter) const;
};

template <class Filter> const CMessageEvent* CMessageEventManager::FindMostRecentMessage(const Filter& filter) const
{
    for (CMessageList::node* n = Messages.head->next; n != Messages.head; n = n->next) {
        const CMessageEvent* message = &n->value;
        if (filter(*message))
            return message;
    }
    return 0;
}
typedef CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatorAlive,
    std::unary_negate<CMessageEventFilter_IsOriginatedByThing>, CMessageEventFilter_HasExtraData> CMostRecentFilter;
template const CMessageEvent* CMessageEventManager::FindMostRecentMessage<CMostRecentFilter>(const CMostRecentFilter&) const;