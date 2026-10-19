// CMessageEventManager::FindMostRecentMessage<CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> >>(const Filter& filter) -- the most recent queued message of the type whose originating creature passes the IsValidGossipee, In2DRange and nav line-of-sight thing filters (0 if none)
// Leaf: FindMostRecentMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090E2E0, length 225 bytes (`ret 4`).
// Label correction: the catalog name says CFilter_Bind3<IsOfType, ThingSearchFilter<In2DRange>, ThingSearchFilter<IsValidGossipee>> (a
// truncated propagated name); the filter has four members (+0x0C is read) and the Ego_r.pdb CFilter_Bind4 with this member order exists.
// Callees: CMessageEventFilter_IsOfType::operator() @ 0x006AF1C0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three per inlined thing
// filter), CThingFilter_IsValidGossipee::operator() @ 0x0090C900, CThingFilter_In2DRange::operator() @ 0x006A76B0, the out-of-line
// CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>::operator() @ 0x0090DFB0.
// Layout: CMessageEvent::POriginator (CIntelligentPointer<CThing>) at +0x48 (Ego_r.pdb); CThing type flags at +0x6C (creature mask 0x1F).
// Shape: the typed intelligent-pointer getter `GetPItem() && IsThingOfType<T>(GetPItem()) ? (T*)GetPItem() : 0` (mask in a local); the
// fourth thing filter is called out of line (its own TSF instantiation; the others are `__forceinline`); the matching message's
// address is a named pointer local (kept in ebx and returned).
struct CThing {
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;               // +0x6c
};
struct CThingAICreature : CThing {
    enum { ThingTypeMask = 0x1f };
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return !(~thing->ThingTypeFlags & mask);
}
struct CBaseObject;
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PData;
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    template <class U> U* GetTypedPItem() const
    {
        return GetPItem() && IsThingOfType<U>((U*)GetPItem()) ? (U*)GetPItem() : 0;
    }
};
struct CMessageEvent {
    unsigned char _pad00[0x48];
    CIntelligentPointer<CThing> POriginator;    // +0x48
};
struct CMessageEventFilter_IsOfType {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
};
struct CThingFilter_IsValidGossipee {
    bool operator()(CThingAICreature* creature) const;     // 0x0090C900
};
struct CThingFilter_In2DRange {
    bool operator()(CThing* thing) const;                  // 0x006A76B0
};
struct CThingFilter_IsNavLineOfSightToThing {
    bool operator()(CThing* thing) const;                  // 0x006A7640
};
template <class F> struct CMessageEventFilter_ThingSearchFilter {
    F* Filter;
    __forceinline bool operator()(const CMessageEvent& message) const
    {
        CThingAICreature* creature = message.POriginator.GetTypedPItem<CThingAICreature>();
        if (!creature)
            return false;
        return (*Filter)(creature);
    }
};
template <> struct CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> {
    CThingFilter_IsNavLineOfSightToThing* Filter;
    bool operator()(const CMessageEvent& message) const;   // 0x0090DFB0 (out of line)
};
template <class F1, class F2, class F3, class F4> struct CFilter_Bind4 {
    F1* PF1;
    F2* PF2;
    F3* PF3;
    F4* PF4;
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
template const CMessageEvent* CMessageEventManager::FindMostRecentMessage<CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> > >(const CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>, CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> >&) const;
