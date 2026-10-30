// NScript::CV_BanditTollScript::CBlockingBandit::Init() -- the blocking bandit joins FACTION_BANDITS_FRIENDLY and is attached to the active quest's script
// Leaf: ?Init@CBlockingBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E49900, 96 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E4EDC, which the landed
//   NScript::CV_BanditTollScript::CBlockingBandit::Alloc 0x00E49850 stores (site 0x00E49886); Ego_r.pdb has ?Init@CBlockingBandit@CV_BanditTollScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CBlockingBandit (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("FACTION_BANDITS_FRIENDLY" 0x012D218C); interface
//   slots 601 EntitySetInFaction, 465 EntityAttachToScript, 655 GetActiveQuestName.
// Shape: the outer call's vtable is loaded (ebx) before the inner GetActiveQuestName call.
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditTollScript {
public:
    class CBlockingBandit;
};
class CV_BanditTollScript::CBlockingBandit : public CEntityScriptBase {
public:
    CV_BanditTollScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Init();
};

void CV_BanditTollScript::CBlockingBandit::Init()
{
    Game->EntitySetInFaction(Me, "FACTION_BANDITS_FRIENDLY");
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
}
}