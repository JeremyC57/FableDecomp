// CCreatureAction_AddRealObjectToInventory::PerformAction() -- if the object exists and is not flagged (bit 0 of its byte at +0x91), add it to the creature's inventory component (interface type 203) as a real item
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844B40, length 118 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PObject at +0xa8 three times, PCreature at +8 once),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 203 = bit 11 of the bitset dword at +0x38,
// `test ch,8`), the inventory component's AddItemToInventory(CThing*, bool) @ 0x007CAB30 (manifest label
// "TCHeroReceiveItems_AddItemToInventory" is a propagated guess; the result is dropped).
// Shape: one `push ecx` frame for the lookup key; the object pointer is fetched afresh for every use (three GetPItem calls);
// the `push 1` precedes the third fetch (arguments right to left). Retail CCreatureActionBase is 0xa8 bytes (PObject at
// +0xa8; no engine header for this class), so `this` is modelled locally.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CTCPhysicsBase;
struct CTCGraphicAppearance;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x14];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }                           // inline accessors: the member is
    CTCGraphicAppearance* GetGraphicAppearance() const { return m_pGraphicAppearance; }   //   loaded straight into ecx
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {            // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCInventory {                       // the interface-203 component
    void AddItemToInventory(CThing* item, bool real);   // 0x007CAB30
};
struct CThingFlags91 {                      // the byte at +0x91 of a thing: bit 0 tested
    unsigned char _pad[0x91];
    unsigned char m_isBeingRemoved : 1;
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};

struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {
    CBaseIntelligentPointer PObject;        // +0xa8 CIntelligentPointer<CThing>: the real object to add
    virtual void PerformAction();
};

void CCreatureAction_AddRealObjectToInventory::PerformAction()
{
    if (PObject.GetPItem()) {
        if (!((CThingFlags91*)PObject.GetPItem())->m_isBeingRemoved) {
            CTCInventory* pInventory;
            if (PCreature.GetPItem()->GetInterface(&pInventory, 203))
                pInventory->AddItemToInventory(PObject.GetPItem(), true);
        }
    }
}