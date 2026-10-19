// CCreatureAction_Talk::PerformAction() -- when the talk target is an AI creature (thing class byte 1) with an entity-events component (0x42), add a CEventTalkedTo(owner) to it and clear/set its has-information flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846660, length 165 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x42 =
// CTCEntityEvents, bit 2 of +0x28), operator new @ 0x00BFEA1A (0x10 bytes), NEntityEvents::CEventTalkedTo::CEventTalkedTo(CThing*)
// @ 0x0084AF90 (vtable 0x01274228; manifest "CEventFiredShot" is a propagation), CTCEntityEvents::AddEvent @ 0x008113C0,
// CThingAICreature::SetHasInformation(bool, bool, bool, bool) @ 0x00830EB0.
// Shape: the target is tested for presence and its class byte (+0x10 == 1), then fetched into a creature local whose own
// null test precedes the checked out-parameter lookup; the `new` null test selects 0 for AddEvent.
// Layout: owner CIntelligentPointer +8, target +0xa8.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char m_thingClass;                 // +0x10 (1: AI creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingAICreature : CThing {
    void SetHasInformation(bool a, bool b, bool c, bool d);   // 0x00830EB0
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CEventBase { };
struct CEventTalkedTo : CEventBase {
    unsigned char _pad[0x10];
    CEventTalkedTo(CThing* talker);             // 0x0084AF90
};
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);           // 0x008113C0
};

class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};

class CCreatureAction_Talk : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PTarget;            // +0xa8
    virtual void PerformAction();
};

void CCreatureAction_Talk::PerformAction()
{
    if (PTarget.GetPItem() && PTarget.GetPItem()->m_thingClass == 1) {
        CThingAICreature* creature = (CThingAICreature*)PTarget.GetPItem();
        CTCEntityEvents* events;
        if (creature && creature->GetInterface(&events, 0x42)) {
            events->AddEvent(new CEventTalkedTo(PCreature.GetPItem()));
            creature->SetHasInformation(false, false, true, false);
        }
    }
}
