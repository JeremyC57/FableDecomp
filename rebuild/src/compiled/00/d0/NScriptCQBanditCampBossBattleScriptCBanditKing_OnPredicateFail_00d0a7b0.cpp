// NScript::CQ_BanditCampBossBattleScript::CBanditKing::OnPredicateFail() -- when the bandit king was killed, record it (TwinBladeKilled, BanditCampTwinbladeKilled); then hide the quest info and remove the king's health bar
// Leaf: ?OnPredicateFail@CBanditKing@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0A7B0, 96 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012C9474, which the landed
//   NScript::CQ_BanditCampBossBattleScript::CBanditKing::Alloc 0x00D0A700 stores (site 0x00D0A736); Ego_r.pdb has
//   ?OnPredicateFail@CBanditKing@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the literal "" 0x0122D70E), CScriptThing::MsgIsKilledBy (virtual slot 18, +0x48),
//   script interface slots 321 DisplayQuestInfo (+0x504) and 338 RemoveQuestInfoElement (+0x548).
// Layout: Ego_r.pdb CBanditKing: ParentClass +0x14, PMasterData +0x18, HealthBarIndex +0x1c; CQ_BanditCampBossBattleScript::TwinBladeKilled
//   Ego_r +0x78 = retail +0x64; CQ_SunnyvaleMasterData::BanditCampTwinbladeKilled +0x3a (master-data offsets equal retail).
// Shape: the H035 OnPredicateFail shape (landed 0x00D1DD80): the CCharString temporary dies before the result (bl) is tested.
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the virtual calls and the by-value copy need
//   a polymorphic local model (as the landed entity-script rows use).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x3a];
    bool          BanditCampTwinbladeKilled;            // +0x3a
};
class CQ_BanditCampBossBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _pad48[0x64 - 0x48];
    bool          TwinBladeKilled;                      // +0x64
    class CBanditKing;
};
class CQ_BanditCampBossBattleScript::CBanditKing : public CEntityScriptBase {
public:
    CQ_BanditCampBossBattleScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
    long                          HealthBarIndex;   // +0x1c
private:
    virtual void OnPredicateFail();
};

void CQ_BanditCampBossBattleScript::CBanditKing::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
    {
        ParentClass->TwinBladeKilled = true;
        PMasterData->BanditCampTwinbladeKilled = true;
    }
    Game->DisplayQuestInfo(false);
    Game->RemoveQuestInfoElement(HealthBarIndex);
}
}