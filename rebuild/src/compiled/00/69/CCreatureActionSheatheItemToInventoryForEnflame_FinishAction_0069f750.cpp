// CCreatureAction_SheatheItemToInventory_ForEnflame::FinishAction() -- run the base sheathe-to-inventory finish, then if the creature has the spell component (interface type 0x95) whose flag at +0xc is clear, trigger it (@ 0x007A44C0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F750, length 86 bytes.
// Callees: CCreatureAction_SheatheItemToInventory::FinishAction @ 0x0069F670 (non-virtual base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x95 = bit 21 of the bitset dword at +0x30), the component's trigger @ 0x007A44C0.
// Shape: the landed 0x00849320 lookup with a byte-flag test on the component before the call.
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCSpellComponent_0069f750 {
    unsigned char _pad[0xc];
    unsigned char m_triggered;              // +0xc
    void Trigger();                         // 0x007A44C0
};

struct CCreatureAction_SheatheItemToInventory {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x0069F670
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;      // +0x08
};
struct CCreatureAction_SheatheItemToInventory_ForEnflame : CCreatureAction_SheatheItemToInventory {
    virtual void FinishAction();
};

void CCreatureAction_SheatheItemToInventory_ForEnflame::FinishAction()
{
    CCreatureAction_SheatheItemToInventory::FinishAction();
    CTCSpellComponent_0069f750* pSpell;
    if (PCreature.GetPItem()->GetInterface(&pSpell, 0x95)) {
        if (!pSpell->m_triggered)
            pSpell->Trigger();
    }
}