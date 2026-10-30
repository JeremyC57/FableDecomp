// CQ_HangingTreeEvilScript::Init() -- register the quest's four regions, reset the mission state, switch the regions' creature generators off and register the quest's four boasts
// Leaf: ?Init@CQ_HangingTreeEvilScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D68FF0, length 920 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CCharString::operator=(const char*)
//   0x0099EFE0; through the interface at +0x40: AddQuestRegion (slot 271, +0x43c), SetCreatureGeneratorsEnabled (slot 558,
//   +0x8b8), AddBoast (slot 303, +0x4bc).
// Data: the script def 0x0143E90C (retail = Ego_r.pdb CScriptDef - 4 - 4 per earlier STLport vector): HTEvil boast
//   cost/reward pairs +0x108..+0x11c, HangingTreeProtectBanditsBoastReward +0xf50 / ...Amount +0xf54.
// Layout (retail = Ego_r.pdb CQ_HangingTreeEvilScript - 0x14): bools +0x48..+0x58, ReasonForFailing +0x5c,
//   HTE_State +0x60, CurrentBandits +0x64, BanditsAtNextRegion +0x68, TimerGUIIndex +0x6c, CarryOverGuards +0x70.
// Shape: as the landed CQ_WhiteBalverineKnotholeGladeScript::Init 0x00E13C60 -- each literal goes straight to a
//   `const CCharString&` parameter; the zero (false / 0) is hoisted into ebx for the member stores and the pushes.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad0[0x108];
    long HTEvilNakedBoastCost;                          // +0x108
    long HTEvilNakedBoastReward;                        // +0x10c
    long HTEvilNoDamageBoastCost;                       // +0x110
    long HTEvilNoDamageBoastReward;                     // +0x114
    long HTEvilNoWeaponsBoastCost;                      // +0x118
    long HTEvilNoWeaponsBoastReward;                    // +0x11c
    unsigned char _pad120[0xf50 - 0x120];
    long HangingTreeProtectBanditsBoastReward;          // +0xf50
    long HangingTreeProtectBanditsBoastAmount;          // +0xf54
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool IntroCutscenePlayed;                           // +0x48
    bool BanditsStarted;                                // +0x49
    bool BanditKilled;                                  // +0x4a
    bool PartySuccessfullySetUp;                        // +0x4b
    bool MissionSucceeded;                              // +0x4c
    bool MissionFailed;                                 // +0x4d
    bool BanditsLeftRegion;                             // +0x4e
    bool SpawnHere;                                     // +0x4f
    bool AtWayPoint;                                    // +0x50
    bool SwitchingRegions;                              // +0x51
    bool RemoveBandits;                                 // +0x52
    bool AllEnemiesDead;                                // +0x53
    bool QuestStartScreenShown;                         // +0x54
    bool BanditYelledRecently;                          // +0x55
    bool ExecutionerInterrupted;                        // +0x56
    bool AudienceLeggit;                                // +0x57
    bool ShouldFollowHero;                              // +0x58
    CCharString ReasonForFailing;                       // +0x5c
    long HTE_State;                                     // +0x60
    long CurrentBandits;                                // +0x64
    long BanditsAtNextRegion;                           // +0x68
    long TimerGUIIndex;                                 // +0x6c
    long CarryOverGuards;                               // +0x70
    void Init();
};
} // namespace NScript
using NScript::CQ_HangingTreeEvilScript;

void NScript::CQ_HangingTreeEvilScript::Init()
{
    Game->AddQuestRegion("Q_HangingTreeEvil", "BowerstoneJail");
    Game->AddQuestRegion("Q_HangingTreeEvil", "DesertedFarm");
    Game->AddQuestRegion("Q_HangingTreeEvil", "GibbetWoods");
    Game->AddQuestRegion("Q_HangingTreeEvil", "ExecutionTree");
    CurrentBandits = 4;
    BanditsAtNextRegion = 0;
    CarryOverGuards = 0;
    IntroCutscenePlayed = false;
    MissionFailed = false;
    MissionSucceeded = false;
    BanditsStarted = true;
    BanditsLeftRegion = true;
    SwitchingRegions = false;
    AllEnemiesDead = true;
    SpawnHere = true;
    RemoveBandits = false;
    QuestStartScreenShown = false;
    BanditYelledRecently = false;
    ExecutionerInterrupted = false;
    AudienceLeggit = false;
    ShouldFollowHero = false;
    ReasonForFailing = "";
    Game->SetCreatureGeneratorsEnabled("BowerstoneJail", false);
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", false);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", false);
    Game->SetCreatureGeneratorsEnabled("ExecutionTree", false);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->HTEvilNakedBoastCost,
                   pGlobalScriptDef->HTEvilNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->HTEvilNoDamageBoastCost,
                   pGlobalScriptDef->HTEvilNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->HTEvilNoWeaponsBoastCost,
                   pGlobalScriptDef->HTEvilNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 0x20, pGlobalScriptDef->HangingTreeProtectBanditsBoastAmount,
                   pGlobalScriptDef->HangingTreeProtectBanditsBoastReward, true, "", 0);
}