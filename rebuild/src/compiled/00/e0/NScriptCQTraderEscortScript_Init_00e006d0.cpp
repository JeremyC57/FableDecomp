// CQ_TraderEscortScript::Init() -- register the Darkwood / Barrow Fields quest regions, switch off the creature generators along the route, reset the escort state, read the incubation times from the script def, clear the trader to talk, mark all traders alive in the master data and add the unclothed / no-damage / all-traders-alive boasts
// Leaf: ?Init@CQ_TraderEscortScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E006D0, 1352 bytes (int3 padded to 1360).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0; Game slots 271 AddQuestRegion,
//   303 AddBoast, 558 SetCreatureGeneratorsEnabled; script def pGlobalScriptDef 0x0143E90C.
// Layout (retail = Ego_r.pdb - 0x14): CommentTimer +0x48, BalverinesToSurpriseHeroNeeded +0x4c, TradersStillAliveCounter
//   +0x50, InfectedTraderCanGetUp +0x54 .. SavedNearEnd +0x5a, IncubationTimeLow/Medium/High +0x5c/+0x60/+0x64,
//   TraderPickpocketedCount +0x68, MissionSucceeded +0x6c .. ShownEarthTroll +0x71, TimesBantered +0x74, TraderToTalk +0x78.
//   CScriptDef = Ego_r.pdb - 4. CQ_SunnyvaleMasterData (no shift): DarkwoodAllTradersAlive +0x10b,
//   DarkwoodPickpocketedAllTraders +0x120.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad0[0x1f8];
    long DarkwoodNakedBoastCost;                        // +0x1f8
    long DarkwoodNakedBoastReward;                      // +0x1fc
    long DarkwoodNoDamageBoastCost;                     // +0x200
    long DarkwoodNoDamageBoastReward;                   // +0x204
    unsigned char _pad208[0xdf8 - 0x208];
    long TraderEscortIncubationTimeLow;                 // +0xdf8
    long TraderEscortIncubationTimeMedium;              // +0xdfc
    long TraderEscortIncubationTimeHigh;                // +0xe00
    long AllTradersAliveBoastCost;                      // +0xe04
    long AllTradersAliveBoastReward;                    // +0xe08
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x10b];
    bool DarkwoodAllTradersAlive;                       // +0x10b
    unsigned char _pad10c[0x120 - 0x10c];
    long DarkwoodPickpocketedAllTraders;                // +0x120
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_TraderEscortScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    long CommentTimer;                                  // +0x48 (CTimer)
    long BalverinesToSurpriseHeroNeeded;                // +0x4c
    long TradersStillAliveCounter;                      // +0x50
    bool InfectedTraderCanGetUp;                        // +0x54
    bool TradersShouldBeScared;                         // +0x55
    bool IntroFinished;                                 // +0x56
    bool FollowInfoGiven;                               // +0x57
    bool EndStarted;                                    // +0x58
    bool SavedInMiddle;                                 // +0x59
    bool SavedNearEnd;                                  // +0x5a
    long IncubationTimeLow;                             // +0x5c
    long IncubationTimeMedium;                          // +0x60
    long IncubationTimeHigh;                            // +0x64
    long TraderPickpocketedCount;                       // +0x68
    bool MissionSucceeded;                              // +0x6c
    bool MissionFailed;                                 // +0x6d
    bool QuestStartScreened;                            // +0x6e
    bool GoneWrongWay;                                  // +0x6f
    bool ShownBalverine;                                // +0x70
    bool ShownEarthTroll;                               // +0x71
    long TimesBantered;                                 // +0x74
    CCharString TraderToTalk;                           // +0x78
    void Init();
};
} // namespace NScript
using NScript::CQ_TraderEscortScript;

void NScript::CQ_TraderEscortScript::Init()
{
    Game->AddQuestRegion("Q_TraderEscort", "Darkwood1");
    Game->AddQuestRegion("Q_TraderEscort", "Darkwood2");
    Game->AddQuestRegion("Q_TraderEscort", "DemonDoor_DarkwoodSporeSwamp");
    Game->AddQuestRegion("Q_TraderEscort", "Darkwood3");
    Game->AddQuestRegion("Q_TraderEscort", "Darkwood4");
    Game->AddQuestRegion("Q_TraderEscort", "DarkwoodChapel");
    Game->AddQuestRegion("Q_TraderEscort", "Darkwood5");
    Game->AddQuestRegion("Q_TraderEscort", "Darkwood6");
    Game->AddQuestRegion("Q_TraderEscort", "BarrowFields");
    Game->AddQuestRegion("Q_TraderEscort", "DemonDoor_Barrowfields");
    Game->SetCreatureGeneratorsEnabled("Darkwood1", false);
    Game->SetCreatureGeneratorsEnabled("Darkwood2", false);
    Game->SetCreatureGeneratorsEnabled("Darkwood3", false);
    Game->SetCreatureGeneratorsEnabled("Darkwood5", false);
    Game->SetCreatureGeneratorsEnabled("Darkwood6", false);
    TraderPickpocketedCount = 0;
    BalverinesToSurpriseHeroNeeded = 0;
    TradersStillAliveCounter = 0;
    InfectedTraderCanGetUp = false;
    TradersShouldBeScared = false;
    IntroFinished = false;
    FollowInfoGiven = false;
    EndStarted = false;
    SavedInMiddle = false;
    SavedNearEnd = false;
    MissionFailed = false;
    MissionSucceeded = false;
    QuestStartScreened = false;
    GoneWrongWay = false;
    ShownBalverine = false;
    ShownEarthTroll = false;
    TimesBantered = 0;
    IncubationTimeLow = pGlobalScriptDef->TraderEscortIncubationTimeLow;
    IncubationTimeMedium = pGlobalScriptDef->TraderEscortIncubationTimeMedium;
    IncubationTimeHigh = pGlobalScriptDef->TraderEscortIncubationTimeHigh;
    TraderToTalk = "";
    PMasterData->DarkwoodAllTradersAlive = true;
    PMasterData->DarkwoodPickpocketedAllTraders = 1;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->DarkwoodNakedBoastCost, pGlobalScriptDef->DarkwoodNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->DarkwoodNoDamageBoastCost, pGlobalScriptDef->DarkwoodNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_ALLTRADERSALIVE", 9, pGlobalScriptDef->AllTradersAliveBoastCost, pGlobalScriptDef->AllTradersAliveBoastReward, false, "", 0);
}