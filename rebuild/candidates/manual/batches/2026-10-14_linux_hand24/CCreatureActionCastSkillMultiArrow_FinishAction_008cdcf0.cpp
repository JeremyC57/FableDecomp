// CCreatureAction_CastSkillMultiArrow::FinishAction() -- base FinishAction, then (when the creature has the type-0xA7 component) clears that component's casting flag and level
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CDCF0, length 80 bytes. FinishAction is vtable slot 4 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup: type 0xA7 = bit 7 of the bitset
// dword at +0x34, which VC tests as the sign of the low byte, `mov cl ; test cl,cl ; jns`; landed shape of 0x00849320).
// Shape: one `push ecx` frame for the lookup key; the component rides in esi; its byte at +0xc and dword at +0x10 are
// cleared through the shared zero register. No engine header matches the retail layout (CCreatureActionBase is 0xa8
// bytes in retail, 0xb4 in the PDB), so `this` is modelled locally.
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

struct CTCSpellCastState {                  // the type-0xA7 component (name descriptive): a casting flag and a level
    unsigned char _pad0[0x0c];
    unsigned char m_isCasting;              // +0x0c
    long          m_castLevel;              // +0x10
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;            // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void FinishAction();            // 0x00859A20 (retail slot 4; the slot is immaterial to this direct call)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_CastSkillMultiArrow : CCombatActionBase {   // vtable 0x01284BDC
    virtual void FinishAction();
};

void CCreatureAction_CastSkillMultiArrow::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCSpellCastState* pState;
    if (PCreature.GetPItem()->GetInterface(&pState, 0xa7)) {
        pState->m_isCasting = 0;
        pState->m_castLevel = 0;
    }
}
