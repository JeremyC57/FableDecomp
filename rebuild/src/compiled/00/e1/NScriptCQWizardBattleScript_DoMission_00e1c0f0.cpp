// CQ_WizardBattleScript::DoMission() -- waits for HookCoast, sets the quest-card objective, hides the CS_KEY marker and puts HookCoastVillage in limbo, spawns the WatchForLeaving and CutsceneScreamers threads, records the lighthouse top and maze teleport positions, then runs phase one, phase two and the mission end unless the mission failed
// Leaf: DoMission
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_WizardBattleScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    bool VillagersTurnOnHero;                           // +0x48
    bool VillagersChased;                               // +0x49
    bool FightersStart;                                 // +0x4a
    bool MissionSucceeded;                              // +0x4b
    bool MissionFailed;                                 // +0x4c
    bool MissionOver;                                   // +0x4d
    bool QuestStartScreenShown;                         // +0x4e
    bool MazeDefeated;                                  // +0x4f
    long MazePhase;                                     // +0x50
    long NumberOfScreamersAlive;                        // +0x54
    long MazeConversation;                              // +0x58
    CScriptThing Maze;                                  // +0x5c
    CScriptThing TopOfLighthouse;                       // +0x68
    CScriptThing MiddleOfLighthouse;                    // +0x74
    void DoMission();
    void WatchForLeaving();                             // 0x00E1C4D0
    void PhaseTwo();                                    // 0x00E1C580
    void CutsceneScreamers();                           // 0x00E1CD40
    void PhaseOne();                                    // 0x00E1CFD0
    void EndMission();                                  // 0x00E1D640
};

void CQ_WizardBattleScript::DoMission()
{
    while (!GSI->IsLevelLoaded("HookCoast")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_WIZARD_BATTLE_OBJECTIVE_01", "HookCoast", "HookCoast");
    CScriptThing key = GSI->GetThingWithScriptName("CS_KEY");
    GSI->EntitySetAlpha(key, 0.0f, true);
    CScriptThing village = GSI->GetThingWithScriptName("HookCoastVillage");
    GSI->SetVillageLimbo(village, true);
    AddSpawnedFunction(new CSpawnedFunc<CQ_WizardBattleScript>("WatchForLeaving", 0, this, &CQ_WizardBattleScript::WatchForLeaving), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_WizardBattleScript>("CutsceneScreamers", 0, this, &CQ_WizardBattleScript::CutsceneScreamers), CCharString(""));
    TopOfLighthouse = GSI->GetThingWithScriptName("TopOfLighthouse");
    MiddleOfLighthouse = GSI->GetThingWithScriptName("MazeTeleportPos3");
    PhaseOne();
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed)
        return;
    PhaseTwo();
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed)
        return;
    MazeDefeated = true;
    EndMission();
}
}