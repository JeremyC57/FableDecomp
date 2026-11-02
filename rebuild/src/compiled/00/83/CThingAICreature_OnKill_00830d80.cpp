// CThingAICreature::OnKill() -- a killed AI creature leaves combat (deregistered from its combat manager while in melee), drops its quest-info entries in the player GUI and kills its AI brain; then the creature-base OnKill
// Leaf: ?OnKill@CThingAICreature@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830D80, 67 bytes (int3 padding to 0x00830DD0), virtual, tail `jmp` to the base OnKill.
// Identity: the only reference is slot 12 (CThing OnKill: CThing 0x01239ABC slot 12 = the protected CThing::OnKill 0x004C7CF0) of vtable 0x0127293C (RTTI
//   CThingAICreature). Ego_r.pdb + FableWin.pdb ?OnKill@CThingAICreature@@UAEXXZ. The catalog label is right.
// Callees: CThingCreatureBase::IsInMelee() const 0x00661E80 (both PDBs), CThingCreatureBase::GetCombatManager() 0x00661ED0 (both PDBs
//   ?GetCombatManager@CThingCreatureBase@@QAEAAVCCombatManager@@XZ), CCombatManager::DeregisterDestroyedCombatant(CThingCreatureBase&)
//   0x006ED9E0 (both PDBs), NPlayerGui::CDrawQuestInfo::RemoveElementsForThing(const CThing&) 0x006473B9 (Ego_r), CAIBrain::OnKill()
//   0x0088B6F0 (both PDBs), CThingCreatureBase::OnKill() 0x00664370 (both PDBs ?OnKill@CThingCreatureBase@@UAEXXZ, slot 12 of the
//   CThingPlayerCreature vtable 0x012457FC that inherits it; tail jump).
// Global: CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass 0x013B8790 (private static, read through the inline Get(), as the landed
//   0x005FAEE0 / 0x0061A1F0); `[eax+1C8h]` is CPlayerGui::PDrawQuestInfo.Data (Ego_r +0x1D0, CCountedPointer<NPlayerGui::CDrawQuestInfo>),
//   read through FableWin's inline ?GetQuestInfo@CPlayerGui@@QAEAAVCDrawQuestInfo@NPlayerGui@@XZ.
// Shape: `*this` is pushed before the GetCombatManager call (arguments first); the brain is CThingAICreature::PBrain, a
//   CCountedPointer<CAIBrain> at +0x1A8 (engine/CThingAICreature.h: PBrain_Data +0x1a8) tested with IsNull().
// Model: CThing : CClass : CBaseObject from engine/CBaseObject.h (retail opt-in) and engine/CClass.h under the layout-rename include;
//   CCountedPointer from engine/CCountedPointer.h. engine/CThingAICreature.h / CThingCreatureBase.h / CPlayerGui.h / CAIBrain.h are flattened
//   generated layouts (vptr as data, no member functions), so the classes are declared here with the header offsets.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (Validate slot 1, IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
    unsigned long GetType() const { return Type; }      // FableWin.pdb ?GetType@CClass@@QBEKXZ (inline)
};

#define VF(n) virtual void v##n();
class CThing : public CClass {                         // RTTI CThing : CClass (vtable 0x01239ABC)
public:
    // slots 2..11 (not called here)
    VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11)
protected:
    virtual void OnKill();                              // slot 12 0x004C7CF0 (both PDBs ?OnKill@CThing@@MAEXXZ, protected)
};
#undef VF

class CThingCreatureBase;
class CCombatManager {
public:
    void DeregisterDestroyedCombatant(CThingCreatureBase& creature);   // 0x006ED9E0
};
class CThingCreatureBase : public CThing {
public:
    virtual void OnKill();                              // 0x00664370
    bool IsInMelee() const;                             // 0x00661E80 ?IsInMelee@CThingCreatureBase@@QBE_NXZ
    CCombatManager& GetCombatManager();                 // 0x00661ED0
};

class CAIBrain {
public:
    void OnKill();                                      // 0x0088B6F0 ?OnKill@CAIBrain@@QAEXXZ
};

namespace NPlayerGui {
class CDrawQuestInfo {
public:
    void RemoveElementsForThing(const CThing& thing);   // 0x006473B9 ?RemoveElementsForThing@CDrawQuestInfo@NPlayerGui@@QAEXABVCThing@@@Z
};
}
class CPlayerGui {
public:
    unsigned char _pad00[0x1c8];
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;   // +0x1c8
    NPlayerGui::CDrawQuestInfo& GetQuestInfo() { return *PDrawQuestInfo; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {          // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                            // 0x013B8790 (private static)
public:
    static T& __fastcall Get() { return *PClass; }               // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};

class CThingAICreature : public CThingCreatureBase {   // vtable 0x0127293C
public:
    unsigned char _pad14[0x1a8 - 0x14];
    CCountedPointer<CAIBrain> PBrain;                   // +0x1a8
    virtual void OnKill();
};

void CThingAICreature::OnKill()
{
    if (IsInMelee())
        GetCombatManager().DeregisterDestroyedCombatant(*this);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetQuestInfo().RemoveElementsForThing(*this);
    if (!PBrain.IsNull())
        PBrain->OnKill();
    CThingCreatureBase::OnKill();
}