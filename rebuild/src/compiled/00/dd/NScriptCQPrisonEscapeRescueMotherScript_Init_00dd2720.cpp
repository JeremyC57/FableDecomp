// CQ_PrisonEscapeRescueMotherScript::Init() -- register the prison regions of the quest, add the unclothed / no-damage / no-weapons boasts, reset the task, conversation and escape state, clear the master-data prison flags, make the guards ignore crimes and reset the hero's combat multiplier
// Leaf: ?Init@CQ_PrisonEscapeRescueMotherScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD2720, 1184 bytes.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0; Game slots 134 SetGuardsIgnoreCrimes, 151 ResetPlayerCreatureCombatMultiplier,
//   271 AddQuestRegion, 303 AddBoast; script def pGlobalScriptDef 0x0143E90C (prison boast cost/reward pairs +0x198..+0x1ac).
// Layout (retail = Ego_r.pdb - 0x14): CurrentTask +0x48, TasksDone +0x4c, TargetNumber +0x58, HeroAgreesToTask +0x64 ..
//   RaceStarting +0x6b, YearsPassedInCell +0x6c, PrisonerConversationIndex +0x70, ConversationIndex +0x74, HeroEscapedCell
//   +0x78, AlarmSounded +0x79, MotherRescuedFromPrisonAutoSaveCheckPoint +0x7a. CScriptDef = Ego_r.pdb - 4 (as the landed
//   CQ_HobbeCaveScript::Init). CQ_SunnyvaleMasterData (no shift): MotherDodgingKraken +0x84, PrisonTorturedBefore +0xf2,
//   PrisonGameDoneBefore +0xf3.
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
    unsigned char _pad0[0x198];
    long PrisonNakedBoastCost;                          // +0x198
    long PrisonNakedBoastReward;                        // +0x19c
    long PrisonNoDamageBoastCost;                       // +0x1a0
    long PrisonNoDamageBoastReward;                     // +0x1a4
    long PrisonNoWeaponsBoastCost;                      // +0x1a8
    long PrisonNoWeaponsBoastReward;                    // +0x1ac
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x84];
    bool MotherDodgingKraken;                           // +0x84
    unsigned char _pad85[0xf2 - 0x85];
    bool PrisonTorturedBefore;                          // +0xf2
    bool PrisonGameDoneBefore;                          // +0xf3
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_PrisonEscapeRescueMotherScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    long CurrentTask;                                   // +0x48
    long TasksDone[3];                                  // +0x4c
    long TargetNumber[3];                               // +0x58
    bool HeroAgreesToTask;                              // +0x64
    bool HeroHasFoundWeapons;                           // +0x65
    bool MotherCellDoorOpen;                            // +0x66
    bool MotherFollowing;                               // +0x67
    bool MotherDied;                                    // +0x68
    bool WatchingHero;                                  // +0x69
    bool PrisonCutsceneFinished;                        // +0x6a
    bool RaceStarting;                                  // +0x6b
    long YearsPassedInCell;                             // +0x6c
    long PrisonerConversationIndex;                     // +0x70
    long ConversationIndex;                             // +0x74
    bool HeroEscapedCell;                               // +0x78
    bool AlarmSounded;                                  // +0x79
    bool MotherRescuedFromPrisonAutoSaveCheckPoint;     // +0x7a
    void Init();
};
} // namespace NScript
using NScript::CQ_PrisonEscapeRescueMotherScript;

void NScript::CQ_PrisonEscapeRescueMotherScript::Init()
{
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "GraveyardSecretPassage_1");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "KrakenChamber");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonSecretPassage");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonSecretPassageExterior");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonTortureChamber");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonCourtyard");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonCells_1");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonCells_2");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonCells_3");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonBarracks");
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "PrisonOffice");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->PrisonNakedBoastCost, pGlobalScriptDef->PrisonNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->PrisonNoDamageBoastCost, pGlobalScriptDef->PrisonNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->PrisonNoWeaponsBoastCost, pGlobalScriptDef->PrisonNoWeaponsBoastReward, false, "", 0);
    HeroAgreesToTask = false;
    HeroHasFoundWeapons = false;
    MotherCellDoorOpen = false;
    MotherFollowing = false;
    MotherDied = false;
    WatchingHero = false;
    PrisonCutsceneFinished = false;
    RaceStarting = false;
    YearsPassedInCell = 0;
    CurrentTask = 0;
    PrisonerConversationIndex = -1;
    ConversationIndex = -1;
    HeroEscapedCell = false;
    AlarmSounded = false;
    MotherRescuedFromPrisonAutoSaveCheckPoint = false;
    PMasterData->MotherDodgingKraken = false;
    PMasterData->PrisonTorturedBefore = false;
    PMasterData->PrisonGameDoneBefore = false;
    Game->SetGuardsIgnoreCrimes(true);
    Game->ResetPlayerCreatureCombatMultiplier();
}