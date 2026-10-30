// CQ_BanditCampBossBattleScript::Init() -- register the six Bandit Camp quest regions, then clear the quest's state flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D03610, length 514 bytes (padded to 528).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (x6 each),
// CGameScriptInterface::AddQuestRegion (slot 271, +0x43c) on the script's own interface (+0x40) x6.
// Layout (Ego_r.pdb offsets - 0x14, the debug-only CScriptBase containers): flags +0x48..+0x54, KingHealth +0x58,
// flags +0x5c..+0x65, AngryBanditNeeded +0x68, BanditsNeededForCutscene +0x6c, PubGameChatted +0x6d. Init leaves
// AttackedOuterGateGuards (+0x48) and Gate1Open (+0x4b) alone.
// Shape: each literal is an implicit conversion temporary built right to left (region first) and destroyed after the
// call; the clears share one zero register and are stored in source order.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_BanditCampBossBattleScript {
public:
    unsigned char         _pad_0x00[0x40];             // CQuestScriptBase / CScriptBase
    CGameScriptInterface* Game;                        // +0x40
    unsigned char         _pad_0x44[4];
    bool AttackedOuterGateGuards;                      // +0x48
    bool AttackedGate2OuterGuard;                      // +0x49
    bool AttackedGate3OuterGuard;                      // +0x4a
    bool Gate1Open;                                    // +0x4b
    bool Gate2Open;                                    // +0x4c
    bool Gate3Open;                                    // +0x4d
    bool HostagesRescued;                              // +0x4e
    bool HostageKilled;                                // +0x4f
    bool ItsAllOver;                                   // +0x50
    bool TheresaFrescoShown;                           // +0x51
    bool BanditKingFightStarted;                       // +0x52
    bool BanditKingFightEnded;                         // +0x53
    bool BanditKingCrowdSpawn;                         // +0x54
    int  KingHealth;                                   // +0x58
    bool SpokenToSecondGuard;                          // +0x5c
    bool MissionSucceeded;                             // +0x5d
    bool MissionFailed;                                // +0x5e
    bool Gate1Passed;                                  // +0x5f
    bool AssassinsUnderAttack;                         // +0x60
    bool AssassinCutsceneTriggered;                    // +0x61
    bool TalkedToAssassin;                             // +0x62
    bool PlayGuardTooCloseCutscene;                    // +0x63
    bool TwinBladeKilled;                              // +0x64
    bool TwinBladeAttacked;                            // +0x65
    long AngryBanditNeeded;                            // +0x68
    bool BanditsNeededForCutscene;                     // +0x6c
    bool PubGameChatted;                               // +0x6d
    void Init();
};
} // namespace NScript
using NScript::CQ_BanditCampBossBattleScript;

void NScript::CQ_BanditCampBossBattleScript::Init()
{
    Game->AddQuestRegion("Q_BanditCampBossBattle", "BanditCampPathEntrance");
    Game->AddQuestRegion("Q_BanditCampBossBattle", "BanditCampEntrance");
    Game->AddQuestRegion("Q_BanditCampBossBattle", "BanditCampPath1");
    Game->AddQuestRegion("Q_BanditCampBossBattle", "BanditCampCentre");
    Game->AddQuestRegion("Q_BanditCampBossBattle", "BanditCampBoss");
    Game->AddQuestRegion("Q_BanditCampBossBattle", "DemonDoor_BanditCampPath");
    AttackedGate2OuterGuard = false;
    AttackedGate3OuterGuard = false;
    Gate2Open = false;
    Gate3Open = false;
    HostagesRescued = false;
    HostageKilled = false;
    ItsAllOver = false;
    BanditKingCrowdSpawn = false;
    BanditKingFightStarted = false;
    BanditKingFightEnded = false;
    TheresaFrescoShown = false;
    KingHealth = 0;
    SpokenToSecondGuard = false;
    MissionSucceeded = false;
    MissionFailed = false;
    Gate1Passed = false;
    AssassinsUnderAttack = false;
    TalkedToAssassin = false;
    AssassinCutsceneTriggered = false;
    PlayGuardTooCloseCutscene = false;
    TwinBladeKilled = false;
    TwinBladeAttacked = false;
    AngryBanditNeeded = 0;
    BanditsNeededForCutscene = false;
    PubGameChatted = false;
}