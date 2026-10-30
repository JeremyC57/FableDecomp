// NScript::CV_LostTraderScript::CLostTraderBrother::OnPredicateFail() -- when the brother was killed (by anyone), the quest fails for reason 1 and the brother is flagged killed
// Leaf: ?OnPredicateFail@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA50E0, 71 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012EE034, which the landed
//   NScript::CV_LostTraderScript::CLostTraderBrother Alloc 0x00EA4FF0 (new 0x2c; the catalog folds it into the landed
//   CLostTraderTrader::MakeRandomComment span, which really ends at 0x00EA4FF0); slot 2 of the vtable is the landed
//   CLostTraderBrother::Init 0x00EA50A0 stores (site 0x00EA5024); Ego_r.pdb has ?OnPredicateFail@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CLostTraderBrother (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("" 0x0122D70E); CScriptThing::MsgIsKilledBy (slot 18, 0x004AAAF0).
// Shape (family L137): the bool result is kept in bl across the temporary's destructor.
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
class CV_LostTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool g_bLostTraderKilled;                           // +0x48
    bool g_bTraderBrotherKilled;                        // +0x49
    bool g_bQuestFailed;                                // +0x4a
    bool g_bTraderHit;                                  // +0x4b
    bool ReachedTrader;                                 // +0x4c
    long DistanceForBrotherToReunite;                   // +0x50
    long ReasonForFailure;                              // +0x54
    class CLostTraderBrother;
};
class CV_LostTraderScript::CLostTraderBrother : public CEntityScriptBase {
public:
    CV_LostTraderScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long stateBrain;                                    // +0x1c
    bool HasToldHeroWhereBrotherIs;                     // +0x20
    long TalkCounter;                                   // +0x24
    bool DoneIntro;                                     // +0x28
private:
    virtual void OnPredicateFail();
};

void CV_LostTraderScript::CLostTraderBrother::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->ReasonForFailure = 1;
        ParentClass->g_bTraderBrotherKilled = true;
    }
}
}