// CCreatureAction_FinishFireballSpell::PerformAction() -- if the creature has the type-0x9D (fireball spell) component, cast the fireball at the action's target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BABE0, length 96 bytes. Vtable slot 3 (PerformAction) of CCreatureAction_FinishFireballSpell (0x0127E1FC).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, then PTarget at +0xa8 for the argument),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x9D = bit 29 of the bitset dword at +0x30;
// landed shape of 0x00849320), CTCFireballSpell::CastFireball(CThing*) @ 0x007C89F0.
// Shape: one `push ecx` frame for the lookup key; `this` rides in edi, the component in esi. Retail CCreatureActionBase is
// 0xa8 bytes (PDB 0xb4), so PTarget is at +0xa8 and `this` is modelled locally.
struct CThing;

struct CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CThingCreatureBase {                 // engine/CThingCreatureBase.h exists but is not used: only the interface lookup is modelled
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

struct CTCFireballSpell_Calls {             // the type-0x9D component; engine/CTCFireballSpell.h exists but only this call is modelled
    void CastFireball(CThing* target);      // 0x007C89F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

struct CCreatureAction_FinishFireballSpell {   // engine/CCreatureAction_FinishFireballSpell.h exists but is not used: retail CCreatureActionBase is 0xa8 bytes, the PDB build's 0xb4
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CBaseIntelligentPointer PTarget;        // +0xa8 CIntelligentPointer<CThing>
};

void CCreatureAction_FinishFireballSpell::PerformAction()
{
    CTCFireballSpell_Calls* pSpell;
    if (PCreature.GetPItem()->GetInterface(&pSpell, 0x9d))
        pSpell->CastFireball((CThing*)PTarget.GetPItem());
}
