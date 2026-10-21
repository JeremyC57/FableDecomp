// CCreatureAction_PlayerInteractionGreet::PerformAction() -- when both the creature and the greeted thing are alive: queue a CEventTalkedTo(creature) on the thing's entity events and a CEventTalkedToBy(thing) on the creature's, then post message 0x14 (flags 0x10) at the creature's position from the creature to the thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845900, length 356 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8, PTarget +0xa8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// tested GetInterface 0x42 = CTCEntityEvents, bit 2 of +0x28, twice), operator new @ 0x00BFEA1A (0x10-byte events), the event ctors
// @ 0x0084AF90 and @ 0x0084B100 (both take one CThing*; manifest labels "CEventFiredShot" are propagated), CTCEntityEvents::AddEvent
// @ 0x008113C0, CCreatureActionBase::GetPWorld @ 0x00692940, CThing::GetPosition @ 0x006A4D00, CMessageEventManager::AddMessage
// @ 0x00687540 (the world's manager at +0x60).
// Layout: retail CCreatureActionBase is 0xa8 bytes: PCreature +0x08, PTarget +0xa8.
// Shape: the landed CCreatureAction_GiveItemToThing 0x00842F70 message model (creature fetched twice into named locals, the manager
// into a third before the arguments); IS_THING_ALIVE on both pointers; `this` spills to the frame (`mov [esp+0Ch],ebp`) because ebp
// walks on to &PTarget; the `new` null test selects 0 for AddEvent (the landed ShowItemToThing shape).
struct C3DVector;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x006A4D00
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CEventBase { };
namespace NEntityEvents {
struct CEventTalkedTo : CEventBase {            // 0x10 bytes
    unsigned char _pad[0x10];
    CEventTalkedTo(CThing* talker);             // 0x0084AF90
};
struct CEventTalkedToBy : CEventBase {          // 0x10 bytes
    unsigned char _pad[0x10];
    CEventTalkedToBy(CThing* listener);         // 0x0084B100
};
}
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);           // 0x008113C0
};
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority);   // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageEventManager;   // +0x60
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CWorld* GetPWorld() const;                  // 0x00692940
};
struct CCreatureAction_PlayerInteractionGreet : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;            // +0xa8
    void PerformAction();
};

void CCreatureAction_PlayerInteractionGreet::PerformAction()
{
    if (IS_THING_ALIVE(PCreature) && IS_THING_ALIVE(PTarget)) {
        CTCEntityEvents* events;
        if (PTarget.GetPItem()->GetInterface(&events, 0x42))
            events->AddEvent(new NEntityEvents::CEventTalkedTo(PCreature.GetPItem()));
        if (PCreature.GetPItem()->GetInterface(&events, 0x42))
            events->AddEvent(new NEntityEvents::CEventTalkedToBy(PTarget.GetPItem()));
        CThing* pCreatureThing = PCreature.GetPItem();
        CThing* pCreature = PCreature.GetPItem();
        CMessageEventManager* pManager = GetPWorld()->m_pMessageEventManager;
        pManager->AddMessage(0x14, 0x10, &pCreature->GetPosition(), pCreatureThing, PTarget.GetPItem(), 0.0f, 0.0f, 0);
    }
}
