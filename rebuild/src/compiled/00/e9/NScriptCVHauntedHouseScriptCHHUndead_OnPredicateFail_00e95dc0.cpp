// NScript::CV_HauntedHouseScript::CHHUndead::OnPredicateFail() -- an undead is gone: when the hero killed it, raise the house's undead bar by four, three, two or one times its amount (bar below 50 / 100 / 200 / above) and count one undead fewer
// Leaf: ?OnPredicateFail@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95DC0, 147 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012ECD3C, which the landed
//   NScript::CV_HauntedHouseScript::CHHUndead::Alloc 0x00E95CD0 stores (site 0x00E95D06); Ego_r.pdb has ?OnPredicateFail@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CHHUndead (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slot 18 MsgIsKilledBy ("" 0x0122D70E temporary); CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0. Float pool: 50.0f 0x0124E7F8, 4.0f 0x0123088C, 100.0f 0x01230A08, 3.0f 0x0122DED4, 200.0f
//   0x0125A168 (the doubling is `fadd st(0),st(0)`).
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
class CGameScriptInterface;
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HauntedHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float UndeadBar;                                    // +0x48
    long GuiCounter;                                    // +0x4c
    bool CellarDoorOpen;                                // +0x50
    bool HeroBeenToChest;                               // +0x51
    bool QuestCardGiven;                                // +0x52
    bool InHouse;                                       // +0x53
    long UndeadAlive;                                   // +0x54
    long MaxUndead;                                     // +0x58
    class CHHUndead;
};
class CV_HauntedHouseScript::CHHUndead : public CEntityScriptBase {
public:
    CV_HauntedHouseScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    float RaiseUndeadBarAmount;                         // +0x1c
private:
    virtual void OnPredicateFail();
};

void CV_HauntedHouseScript::CHHUndead::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        float amount;
        if (ParentClass->UndeadBar < 50.0f)
            amount = RaiseUndeadBarAmount * 4.0f;
        else if (ParentClass->UndeadBar < 100.0f)
            amount = RaiseUndeadBarAmount * 3.0f;
        else if (ParentClass->UndeadBar < 200.0f)
            amount = RaiseUndeadBarAmount * 2.0f;
        else
            amount = RaiseUndeadBarAmount;
        ParentClass->UndeadBar += amount;
        --ParentClass->UndeadAlive;
    }
}
}