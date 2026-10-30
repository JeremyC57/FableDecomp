// CQ_HangingTreeGoodScript::Init() -- register the quest's four regions, reset the mission state, switch the regions' creature generators off and register the quest's four boasts
// Leaf: ?Init@CQ_HangingTreeGoodScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6F020, length 917 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; through the interface at +0x40:
//   AddQuestRegion (slot 271, +0x43c), SetCreatureGeneratorsEnabled (slot 558, +0x8b8), AddBoast (slot 303, +0x4bc).
// Data: the script def 0x0143E90C (retail = Ego_r.pdb CScriptDef - 4 - 4 per earlier STLport vector): HTGood boast
//   cost/reward pairs +0x120..+0x134, HangingTreeProtectGuardsBoastReward +0xf3c / ...Amount +0xf40.
// Layout (retail = Ego_r.pdb CQ_HangingTreeGoodScript - 0x14): bools +0x48..+0x59, HTG_State +0x5c,
//   CurrentWayPoint +0x60, CurrentGuards +0x64.
// Shape: twin of CQ_HangingTreeEvilScript::Init 0x00D68FF0 (each literal goes straight to a `const CCharString&`
//   parameter; the zero is hoisted into ebx for the member stores and the pushes).
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
class CScriptDef { public:
    unsigned char _pad0[0x120];
    long HTGoodNakedBoastCost;                          // +0x120
    long HTGoodNakedBoastReward;                        // +0x124
    long HTGoodNoDamageBoastCost;                       // +0x128
    long HTGoodNoDamageBoastReward;                     // +0x12c
    long HTGoodNoWeaponsBoastCost;                      // +0x130
    long HTGoodNoWeaponsBoastReward;                    // +0x134
    unsigned char _pad138[0xf3c - 0x138];
    long HangingTreeProtectGuardsBoastReward;           // +0xf3c
    long HangingTreeProtectGuardsBoastAmount;           // +0xf40
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool LeaderSpokenTo;                                // +0x48
    bool PartySuccessfullySetUp;                        // +0x49
    bool MissionSucceeded;                              // +0x4a
    bool MissionFailed;                                 // +0x4b
    bool SwitchingRegions;                              // +0x4c
    bool ReassignLeaderNow;                             // +0x4d
    bool PartyDefeated;                                 // +0x4e
    bool SpawnHere;                                     // +0x4f
    bool AtWayPoint;                                    // +0x50
    bool HeroKilledPrisoner;                            // +0x51
    bool QuestStartScreenShown;                         // +0x52
    bool PartyReadyToTransition;                        // +0x53
    bool BanditYelledOut;                               // +0x54
    bool ExecutionerBeenAttacked;                       // +0x55
    bool ExecutionerBeenAttackedByHero;                 // +0x56
    bool BanditsFlee;                                   // +0x57
    bool PrisonerEscaping;                              // +0x58
    bool ExitCutsceneStart;                             // +0x59
    long HTG_State;                                     // +0x5c
    long CurrentWayPoint;                               // +0x60
    long CurrentGuards;                                 // +0x64
    void Init();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

void NScript::CQ_HangingTreeGoodScript::Init()
{
    Game->AddQuestRegion("Q_HangingTreeGood", "BowerstoneJail");
    Game->AddQuestRegion("Q_HangingTreeGood", "DesertedFarm");
    Game->AddQuestRegion("Q_HangingTreeGood", "GibbetWoods");
    Game->AddQuestRegion("Q_HangingTreeGood", "ExecutionTree");
    CurrentWayPoint = 1;
    CurrentGuards = 2;
    MissionFailed = false;
    MissionSucceeded = false;
    LeaderSpokenTo = true;
    SwitchingRegions = false;
    PartySuccessfullySetUp = false;
    ReassignLeaderNow = false;
    PartyDefeated = false;
    SpawnHere = true;
    AtWayPoint = false;
    HeroKilledPrisoner = false;
    QuestStartScreenShown = false;
    PartyReadyToTransition = false;
    BanditYelledOut = false;
    ExecutionerBeenAttacked = false;
    ExecutionerBeenAttackedByHero = false;
    BanditsFlee = false;
    PrisonerEscaping = false;
    ExitCutsceneStart = false;
    Game->SetCreatureGeneratorsEnabled("BowerstoneJail", false);
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", false);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", false);
    Game->SetCreatureGeneratorsEnabled("ExecutionTree", false);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->HTGoodNakedBoastCost,
                   pGlobalScriptDef->HTGoodNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->HTGoodNoDamageBoastCost,
                   pGlobalScriptDef->HTGoodNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->HTGoodNoWeaponsBoastCost,
                   pGlobalScriptDef->HTGoodNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS", 0x21, pGlobalScriptDef->HangingTreeProtectGuardsBoastAmount,
                   pGlobalScriptDef->HangingTreeProtectGuardsBoastReward, false, "", 0);
}