// CCreatureAction_PutDownJug::PerformAction() -- if the creature carries something in its primary slot, run the table put-down's PerformAction and set the carried jug's "put down" flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A200, length 137 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface twice:
// type 0x46 = bit 6 of the word at +0x28 on the creature, type 0xBB = bit 27 of the word at +0x34 on the carried thing),
// CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CCreatureAction_PutDownThingAtTable::PerformAction @ 0x0084A0A0 (direct base call).
// Shape: both GetInterface results are tested (`je` to the shared epilogue); the carried thing rides in esi across the base call.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCCarrying {                        // interface type 0x46
    CThing* GetThingInPrimarySlot();        // 0x006A1440
};
struct CTCJug {                             // interface type 0xBB
    unsigned char _pad00[0x10];
    bool m_putDown;                         // +0x10 (PickUpJug clears it, PutDownJug sets it)
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
struct CCreatureAction_PutDownThingAtTable : CCreatureActionBase {
    virtual void PerformAction();           // 0x0084A0A0
};
struct CCreatureAction_PutDownJug : CCreatureAction_PutDownThingAtTable {
    virtual void PerformAction();
};

void CCreatureAction_PutDownJug::PerformAction()
{
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
        CThing* thing = carrying->GetThingInPrimarySlot();
        if (thing) {
            CCreatureAction_PutDownThingAtTable::PerformAction();
            CTCJug* jug;
            if (thing->GetInterface(&jug, 0xBB))
                jug->m_putDown = true;
        }
    }
}