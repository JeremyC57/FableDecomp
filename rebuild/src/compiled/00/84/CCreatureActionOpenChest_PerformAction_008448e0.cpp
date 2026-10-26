// CCreatureAction_OpenChest::PerformAction() -- when the chest thing is alive: open its CTCChest component, tell the creature's interface-8 component (virtual slot 34) to stop with 0, and queue a CEventOpenedChest(chest) on the creature's entity-events component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008448E0, length 332 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PChest +0xa8, PCreature +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// `p = 0; if (HasTC) p = GetTC` lookups: 0x3C = CTCChest, bit 28 of +0x24; 8, bit 8 of +0x20 (`test ch,1`); 0x42 = CTCEntityEvents, bit 2
// of +0x28), CTCChest::Open @ 0x00750540, the interface-8 component's virtual slot 34 (+0x88, one argument), operator new @ 0x00BFEA1A
// (0x10-byte event), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and SetPItem @ 0x00A01B90 (the event's inline
// CIntelligentPointer<CThing> member, vtable 0x01238C6C), CTCEntityEvents::AddEvent(CEventBase*) @ 0x008113C0.
// Vtable: NEntityEvents::CEventOpenedChest 0x01274258 (RTTI).
// Layout: retail CCreatureActionBase is 0xa8 bytes; the chest pointer is this class's first member (+0xa8). The event is
// {vptr, long 0, CIntelligentPointer<CThing> Chest}: its base CEventBase zeroes the long in an inline ctor with no vtable of its own.
// Shape: IS_THING_ALIVE on PChest; every component is a zero-preloaded `if (HasTC) GetTC` (the three `xor reg,reg`); the `new` null
// test selects 0 for AddEvent and the two AddEvent calls are not merged (the landed CCreatureAction_ShowItemToThing 0x00845270 shape).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentThingPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    __forceinline CIntelligentThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCChest { public:
    void Open();                                // 0x00750540
};
struct CTCInterface8 {                          // the creature's interface-8 component
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF(30) VF(31) VF(32) VF(33)
    virtual void Stop(long mode);               // slot 34 (+0x88)
};
struct __declspec(novtable) CEventBase {
    virtual void v00();
    long m_data;                                // +0x04
    CEventBase() : m_data(0) {}
};
namespace NEntityEvents {
struct CEventOpenedChest : CEventBase {         // 0x10 bytes, vtable 0x01274258
    CIntelligentThingPointer Chest;             // +0x08
    CEventOpenedChest(CThing* chest) : Chest(chest) {}
};
}
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);           // 0x008113C0
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};
class CCreatureAction_OpenChest : public CCreatureActionBase { public:
    CBaseIntelligentPointer PChest;             // +0xa8
    void PerformAction();
};

void CCreatureAction_OpenChest::PerformAction()
{
    if (IS_THING_ALIVE(PChest)) {
        CTCChest* chest = 0;
        {
            CThing* chestThing = PChest.GetPItem();
            if (chestThing->HasTC(0x3c))
                chest = chestThing->GetTC<CTCChest>(0x3c);
        }
        chest->Open();
        CTCInterface8* movement = 0;
        {
            CThing* creature = PCreature.GetPItem();
            if (creature->HasTC(8))
                movement = creature->GetTC<CTCInterface8>(8);
        }
        movement->Stop(0);
        CTCEntityEvents* events = 0;
        {
            CThing* creature = PCreature.GetPItem();
            if (creature->HasTC(0x42))
                events = creature->GetTC<CTCEntityEvents>(0x42);
        }
        events->AddEvent(new NEntityEvents::CEventOpenedChest(PChest.GetPItem()));
    }
}