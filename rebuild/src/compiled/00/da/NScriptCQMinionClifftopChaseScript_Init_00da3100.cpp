// CQ_MinionClifftopChaseScript::Init() -- register the quest's four regions, reset the mission state and both timers, and register the quest's three boasts
// Leaf: ?Init@CQ_MinionClifftopChaseScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA3100, length 660 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; through the interface at +0x40:
//   AddQuestRegion (slot 271, +0x43c), AddBoast (slot 303, +0x4bc); the timer service (global 0x0143E8F8) SetTimer
//   (slot 89, +0x164) through the inline CTimer::Set of the landed CQ_HeroSoulsMotherScript::Init 0x00D83470.
// Data: the script def 0x0143E90C (retail = Ego_r.pdb CScriptDef - 4): MCC boast cost/reward pairs +0x138..+0x14c.
// Layout (retail = Ego_r.pdb CQ_MinionClifftopChaseScript - 0x14): bools +0x48..+0x4e, KilledGuardingMinions +0x50,
//   RockTrollAppeared +0x54, MCC4Taken +0x55, MakeUniqueComment +0x58 / MissionTimer +0x5c (CTimer),
//   MissionTimerID +0x60, ReasonForFailure +0x64, MinionCount +0x68.
// Shape: as CQ_HangingTreeEvilScript::Init 0x00D68FF0 (literals straight to `const CCharString&` parameters; the zero
//   hoisted into ebx).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    void Set(long time) { const long& index = Index; g_pGameScriptInterface->SetTimer(index, time); }
};
class CScriptDef { public:
    unsigned char _pad0[0x138];
    long MCCNakedBoastCost;                             // +0x138
    long MCCNakedBoastReward;                           // +0x13c
    long MCCNoDamageBoastCost;                          // +0x140
    long MCCNoDamageBoastReward;                        // +0x144
    long MCCNoWeaponsBoastCost;                         // +0x148
    long MCCNoWeaponsBoastReward;                       // +0x14c
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_MinionClifftopChaseScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool MapKeeperSaidDialog;                           // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionSucceeded;                              // +0x4a
    bool QuestStartScreened;                            // +0x4b
    bool KilledMinionFieldGenerator;                    // +0x4c
    bool IsIntroCSFinished;                             // +0x4d
    bool PointedToPrisonPath;                           // +0x4e
    long KilledGuardingMinions;                         // +0x50
    bool RockTrollAppeared;                             // +0x54
    bool MCC4Taken;                                     // +0x55
    CTimer MakeUniqueComment;                           // +0x58
    CTimer MissionTimer;                                // +0x5c
    long MissionTimerID;                                // +0x60
    long ReasonForFailure;                              // +0x64
    long MinionCount;                                   // +0x68
    void Init();
};
} // namespace NScript
using NScript::CQ_MinionClifftopChaseScript;

void NScript::CQ_MinionClifftopChaseScript::Init()
{
    Game->AddQuestRegion("Q_MinionClifftopChase", "BowerstoneJail");
    Game->AddQuestRegion("Q_MinionClifftopChase", "DesertedFarm");
    Game->AddQuestRegion("Q_MinionClifftopChase", "GibbetWoods");
    Game->AddQuestRegion("Q_MinionClifftopChase", "PrisonPath");
    MapKeeperSaidDialog = false;
    MissionFailed = false;
    MissionSucceeded = false;
    QuestStartScreened = false;
    KilledMinionFieldGenerator = false;
    IsIntroCSFinished = false;
    PointedToPrisonPath = false;
    RockTrollAppeared = false;
    KilledGuardingMinions = 0;
    MCC4Taken = false;
    MakeUniqueComment.Set(0);
    MissionTimer.Set(0);
    MissionTimerID = 0;
    ReasonForFailure = 0;
    MinionCount = 0;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->MCCNakedBoastCost,
                   pGlobalScriptDef->MCCNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->MCCNoDamageBoastCost,
                   pGlobalScriptDef->MCCNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->MCCNoWeaponsBoastCost,
                   pGlobalScriptDef->MCCNoWeaponsBoastReward, false, "", 0);
}