// CV_ExposeMayorScript::PhaseTwo() -- make EM_SuspiciousVillager persistent and, once, hand the hero the Expose Mayor
// quest card with its first objective (BarrowFields)
// Leaf: PhaseTwo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7A5C0, 380 bytes (padded to 384).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete @ 0x00BFE9BC,
//          ~CBaseClass @ 0x0099A2E0); CGameScriptInterface slots 72 GetThingWithScriptName (+0x120), 295
//          GiveHeroQuestCardDirectly (+0x49c), 296 SetQuestCardObjective (+0x4a0), 477 SetThingPersistent (+0x774),
//          655 GetActiveQuestName (+0xa3c).
// Layout (retail = Ego_r.pdb CV_ExposeMayorScript - 0x14): QuestCardGiven +0x54.
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
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
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
class CV_ExposeMayorScript : public CQuestScriptBase {
public:
    long          Path;                                  // +0x48 (PATH_ID)
    long          StageNumber;                           // +0x4c (STAGE_IN_PATH)
    bool          HeroFlashedLightsCorrectly;            // +0x50
    bool          PrisonerTalkedAboutLamp;               // +0x51
    bool          ResetLamp;                             // +0x52
    bool          SaidYesToBecomeMayor;                  // +0x53
    bool          QuestCardGiven;                        // +0x54
    bool          MayorExposeGhostCutscenePlayed;        // +0x55
    void PhaseTwo();
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;

void NScript::CV_ExposeMayorScript::PhaseTwo()
{
    CScriptThing villager = GSI->GetThingWithScriptName("EM_SuspiciousVillager");
    GSI->SetThingPersistent(villager, true);
    if (!QuestCardGiven) {
        if (IsActiveThreadTerminating())
            return;
        GSI->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_EXPOSE_MAYOR", GSI->GetActiveQuestName(), false);
        GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_01", "BarrowFields", "");
        QuestCardGiven = true;
    }
}