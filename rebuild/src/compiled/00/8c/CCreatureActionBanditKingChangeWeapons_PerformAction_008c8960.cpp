// CCreatureAction_BanditKingChangeWeapons::PerformAction() -- if the creature has a CTCBoss component (interface 173), create its next weapon def for the creature twice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C8960, length 108 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound @ 0x0040F020 (the inlined
// GetInterface lookup for type 173 = bit 13 of the bitset dword at +0x34, `test ch,20h`), CThingCreatureBase::CreateWeaponForThingFromDef(CThing&, long)
// @ 0x00663990 (static `__fastcall`: ecx thing, edx def; twice).
// Identity of the component (sweep3, FINDINGS chunk 481 "interface 0xAD has two names"): interface 173 = 0xAD is CTCBoss
//   (CTCBoss::GetInterfaceType 0x007AFCD0 returns 0xAD; CTCBoss vtable 0x0126D494), and +0x1c is Ego_r CTCBoss::NextWeapon
//   (PBossDef +0x0C, GenericCounter, StrikesSinceHit, currentBossPhase, NextWeapon +0x1C); FableWin ?GetNextWeapon@CTCBoss@@QAEJXZ
//   is a header inline. The creator is Ego_r/FableWin ?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z.
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
class CThing {
public:
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
class CTCBoss {                             // interface 173 (0xAD)
public:
    unsigned char _pad[0x1c];               // CTCBase (vptr, ParentThing, flags), PBossDef +0x0C, GenericCounter, StrikesSinceHit, currentBossPhase
    long NextWeapon;                        // +0x1c
    long GetNextWeapon() { return NextWeapon; }
};
class CThingCreatureBase {
public:
    static void __fastcall CreateWeaponForThingFromDef(CThing& thing, long def);   // 0x00663990
};

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
    CTCBoss* pBoss;
    if (PCreature.GetPItem()->GetInterface(&pBoss, 173)) {
        long def = pBoss->GetNextWeapon();      // named: retail reads the def into a callee-saved register before the creature fetch
        CThingCreatureBase::CreateWeaponForThingFromDef(*PCreature.GetPItem(), def);   // one weapon per hand
        long def2 = pBoss->GetNextWeapon();
        CThingCreatureBase::CreateWeaponForThingFromDef(*PCreature.GetPItem(), def2);
    }
}