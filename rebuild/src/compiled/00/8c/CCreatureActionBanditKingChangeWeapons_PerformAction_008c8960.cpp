// CCreatureAction_BanditKingChangeWeapons::PerformAction() -- if the creature has an interface-173 component, create its weapon def (the word at +0x1c of the component) for the creature twice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C8960, length 108 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 173 = bit 13 of the bitset dword at +0x34, `test ch,20h`), CreateWeaponForThingFromDef(CThing*, long)
// @ 0x00663990 (`__fastcall`: ecx thing, edx def; twice).
// Shape: `this` is never saved (only the PCreature address, in edi); ebx/esi are pushed inside the lookup block; for each
// `__fastcall` the def word is read first (right to left) into a callee-saved register, then the creature fetched into ecx.
// Retail CCreatureActionBase/CCombatActionBase place PCreature at +8; no engine header for this class, so `this` is modelled locally.
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
struct CTCBanditKingWeapons {               // the interface-173 component: its weapon def index at +0x1c
    unsigned char _pad[0x1c];
    long m_weaponDef;                       // +0x1c
};
void __fastcall CreateWeaponForThingFromDef(CThing* thing, long def);   // 0x00663990

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_BanditKingChangeWeapons : CCombatActionBase {
    virtual void PerformAction();
};

void CCreatureAction_BanditKingChangeWeapons::PerformAction()
{
    CTCBanditKingWeapons* pWeapons;
    if (PCreature.GetPItem()->GetInterface(&pWeapons, 173)) {
        long def = pWeapons->m_weaponDef;       // named: retail reads the def into a callee-saved register before the creature fetch
        CreateWeaponForThingFromDef(PCreature.GetPItem(), def);   // one weapon per hand
        long def2 = pWeapons->m_weaponDef;
        CreateWeaponForThingFromDef(PCreature.GetPItem(), def2);
    }
}