// CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>::operator()(const CMessageEvent&) const -- pass a message whose originator is a creature that is a valid gossipee
// Leaf: ??R?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsValidGossipee@@@@QBE_NABVCMessageEvent@@@Z
// Retail: Fable.exe @ 0x0090E120, 69 bytes, thiscall `ret 4`, bool in al.
// Identity (proven): its two callers, 0x0090E2B3 (in the uncatalogued 0x0090E1F0) and 0x0090EBC2 (in the uncatalogued
// 0x0090EB70), are FindMostRecentMessage-shaped walks that inline IsOfType and the In2DRange thing filter (0x006A76B0) and
// call this one out of line through PF3 (+8) with the message: the member order of FableWin.pdb's CFilter_Bind3<
// CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>,
// CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee> > (the name the catalog propagates to 0x0090E2E0/0x0090EBE0). The body is the typed originator fetch the landed 0x0090E2E0 inlines for the same filter, then
// CThingFilter_IsValidGossipee::operator() 0x0090C900 through the filter pointer at +0. Catalog "_gapscan::sub_0090e120".
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThingFilter_IsValidGossipee::operator()
// (CThingAICreature*) const @ 0x0090C900.
// Layout: CMessageEvent::POriginator (CIntelligentPointer<CThing>) at +0x48 (Ego_r.pdb); CThing type flags at +0x6C, the
// creature mask 0x1F; local models as the landed 0x0090E2E0 (no engine header for the filter or the message).
// Shape: `GetPItem() && IsThingOfType<T>(GetPItem()) ? (T*)GetPItem() : 0` with the mask in a local, then the null test.
class CThing {
public:
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                           // +0x6C
};
class CThingAICreature : public CThing {
public:
    enum { ThingTypeMask = 0x1f };
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return !(~thing->ThingTypeFlags & mask);
}
class CBaseObject;
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PData;
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    template <class U> U* GetTypedPItem() const
    {
        return GetPItem() && IsThingOfType<U>((U*)GetPItem()) ? (U*)GetPItem() : 0;
    }
};
class CMessageEvent {
public:
    unsigned char _pad00[0x48];
    CIntelligentPointer<CThing> POriginator;                // +0x48
};
class CThingFilter_IsValidGossipee {
public:
    bool operator()(CThingAICreature* creature) const;     // 0x0090C900
};
template <class F> class CMessageEventFilter_ThingSearchFilter {
public:
    bool operator()(const CMessageEvent& message) const;
    F* Filter;                                              // +0x00
};

template <class F> bool CMessageEventFilter_ThingSearchFilter<F>::operator()(const CMessageEvent& message) const
{
    CThingAICreature* creature = message.POriginator.GetTypedPItem<CThingAICreature>();
    if (!creature)
        return false;
    return (*Filter)(creature);
}

template bool CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>::operator()(const CMessageEvent&) const;