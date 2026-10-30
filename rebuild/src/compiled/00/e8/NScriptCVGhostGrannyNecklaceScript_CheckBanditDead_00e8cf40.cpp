// CV_GhostGrannyNecklaceScript::CheckBanditDead(CScriptThing bandit) -- wait until the bandit has been killed, flag it
// and set the third objective (OrchardFarm)
// Leaf: CheckBanditDead
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8CF40, 341 bytes (padded to 352), `ret 0xc` (the handle by value).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete @ 0x00BFE9BC,
//          ~CBaseClass @ 0x0099A2E0), CScriptThing::MsgIsKilledBy inline (game-side slot 18, +0x48);
//          CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 296 SetQuestCardObjective (+0x4a0), 655
//          GetActiveQuestName (+0xa3c).
// Layout (retail = Ego_r.pdb CV_GhostGrannyNecklaceScript - 0x14): BanditKilled +0x4b.
// Shape: the callee destroys its by-value handle: inline on the loop's terminating return, out of line elsewhere.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};
namespace NScript {
class CV_GhostGrannyNecklaceScript : public CQuestScriptBase {
public:
    bool          WifeNeededForCutscene;                 // +0x48
    bool          HeroKnowsAboutNecklace;                // +0x49
    bool          NecklaceReturned;                      // +0x4a
    bool          BanditKilled;                          // +0x4b
    bool          WarnedGettingCold;                     // +0x4c
    bool          GuidedToBanditLocation;                // +0x4d
    bool          GhostToRest;                           // +0x4e
    bool          QuestCardGiven;                        // +0x4f
    void CheckBanditDead(CScriptThing bandit);
};
} // namespace NScript
using NScript::CV_GhostGrannyNecklaceScript;

void NScript::CV_GhostGrannyNecklaceScript::CheckBanditDead(CScriptThing bandit)
{
    while (!bandit.MsgIsKilledBy("")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    BanditKilled = true;
    GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_GHOST_GRANNY_NECKLACE_OBJECTIVE_03", "OrchardFarm", "");
}