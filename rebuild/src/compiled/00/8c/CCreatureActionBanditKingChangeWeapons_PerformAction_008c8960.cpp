// CCreatureAction_BanditKingChangeWeapons::PerformAction() -- if the creature has a CTCBoss component (interface 173), create its next weapon def for the creature twice
// Leaf: ?PerformAction@CCreatureAction_BanditKingChangeWeapons@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
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
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
struct CTCGraphicAppearance;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_BanditKingChangeWeapons : public CCombatActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_BanditKingChangeWeapons::PerformAction()
{
    CTCBoss* pBoss;
    if (PCreature.GetPItem()->GetTC(TCI_BOSS, &pBoss)) {
        long def = pBoss->GetNextWeapon();      // named: retail reads the def into a callee-saved register before the creature fetch
        CThingCreatureBase::CreateWeaponForThingFromDef(*PCreature.GetPItem(), def);   // one weapon per hand
        long def2 = pBoss->GetNextWeapon();
        CThingCreatureBase::CreateWeaponForThingFromDef(*PCreature.GetPItem(), def2);
    }
}