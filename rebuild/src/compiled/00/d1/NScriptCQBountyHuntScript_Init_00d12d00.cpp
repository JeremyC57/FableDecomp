// CQ_BountyHuntScript::Init() -- register the four Bounty Hunt quest regions, clear the quest flags, arm the boast
// Leaf: ?Init@CQ_BountyHuntScript@NScript@@
// timer, reset the master data's bounty-hunt boast results and offer the four Bounty Hunt boasts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D12D00, length 876 bytes (padded to 880).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's
// interface (+0x40): AddQuestRegion (slot 271, +0x43c) x4 and AddBoast (slot 303, +0x4bc) x4; the global interface
// 0x0143E8F8 SetTimer (slot 89, +0x164) through CTimer.
// Data: the script def 0x0143E90C: BountyHuntNaked/NoDamage boasts +0x98..+0xa4 (Ego_r.pdb -4); the boast time limit
// and the time-limit/decapitation boast values +0xb60..+0xb70 (Ego_r.pdb -0xa8). The quest master data (+0x44,
// CQ_SunnyvaleMasterData): BountyHuntWithinTimeLimit +0xd8, BountyHuntDecapitation +0xd9,
// BountyHuntTimeLimitExceeded +0xda (Ego_r.pdb offsets).
// Layout (Ego_r.pdb - 0x14): flags +0x48..+0x70, BanditGruntTextCounter +0x74, the two boast flags +0x78/+0x79,
// BoastTimerID +0x7c, BoastTime (CTimer) +0x80, BoastQuestInfoDisplayed +0x84.
// Shape: literals are implicit conversion temporaries; every clear and the boasts' false/0 arguments share ebx.
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

extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }

class CScriptDef { public:
    unsigned char _pad_0x000[0x98];
    long BountyHuntNakedBoastCost;                     // +0x98
    long BountyHuntNakedBoastReward;                   // +0x9c
    long BountyHuntNoDamageBoastCost;                  // +0xa0
    long BountyHuntNoDamageBoastReward;                // +0xa4
    unsigned char _pad_0x0a8[0xb60 - 0xa8];
    long BountyHuntBoastTimeLimit;                     // +0xb60
    long BountyHuntBoastCostTimeLimit;                 // +0xb64
    long BountyHuntBoastRewardTimeLimit;               // +0xb68
    long BountyHuntBoastCostDecapitation;              // +0xb6c
    long BountyHuntBoastRewardDecapitation;            // +0xb70
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CQ_SunnyvaleMasterData {
    unsigned char _pad_0x000[0xd8];
    bool BountyHuntWithinTimeLimit;                    // +0xd8
    bool BountyHuntDecapitation;                       // +0xd9
    bool BountyHuntTimeLimitExceeded;                  // +0xda
};

namespace NScript {
class CQ_BountyHuntScript {
public:
    unsigned char           _pad_0x00[0x40];           // CQuestScriptBase / CScriptBase
    CGameScriptInterface*   Game;                      // +0x40
    CQ_SunnyvaleMasterData* Data;                      // +0x44
    bool QuestFailed;                            // +0x48
    bool QuestSucceeded;                         // +0x49
    bool QuestOver;                              // +0x4a
    bool QuestStarted;                           // +0x4b
    bool FishermanDeactivated;                   // +0x4c
    bool VillagerKilled;                         // +0x4d
    bool VillagerIntroDone;                      // +0x4e
    bool LeaderKilled;                           // +0x4f
    bool Deputy1Killed;                          // +0x50
    bool Deputy2Killed;                          // +0x51
    bool Deputy1IntroComplete;                   // +0x52
    bool Deputy2IntroComplete;                   // +0x53
    bool Hostage1Killed;                         // +0x54
    bool Hostage2Killed;                         // +0x55
    bool Hostage1Safe;                           // +0x56
    bool Hostage2Safe;                           // +0x57
    bool Deputy1Escaped;                         // +0x58
    bool LeaderEscaped;                          // +0x59
    bool SpawnDeputy1Grunts;                     // +0x5a
    bool SpawnDeputy2Grunts;                     // +0x5b
    bool SpawnLeaderGrunts;                      // +0x5c
    bool AllDeputy1GruntsKilled;                 // +0x5d
    bool AllDeputy2GruntsKilled;                 // +0x5e
    bool AllLeaderGruntsKilled;                  // +0x5f
    bool AllLeaderChargeGruntsKilled;            // +0x60
    bool AllGruntsKilled;                        // +0x61
    bool FishComplete;                           // +0x62
    bool LeaderIntro1Complete;                   // +0x63
    bool LeaderIntro2Complete;                   // +0x64
    bool LeaderAttackingHostage;                 // +0x65
    bool LeaderAttackingHero;                    // +0x66
    bool LeaderRansomPaid;                       // +0x67
    bool LeaderRansomDenied;                     // +0x68
    bool LeaderLeftHostage;                      // +0x69
    bool LeaderReachedMoney;                     // +0x6a
    bool LeaderAttackedAfterRansomPaid;          // +0x6b
    bool LeaderChargeComplete;                   // +0x6c
    bool LeaderCutsceneComplete;                 // +0x6d
    bool LakeComplete;                           // +0x6e
    bool BanditCutsceneDone;                     // +0x6f
    bool BountyHuntIntroComplete;                // +0x70
    long BanditGruntTextCounter;                       // +0x74
    bool BoastActiveTimeLimit;                         // +0x78
    bool BoastActiveDecapitation;                      // +0x79
    long BoastTimerID;                                 // +0x7c
    CTimer BoastTime;                                  // +0x80
    bool BoastQuestInfoDisplayed;                      // +0x84
    void Init();
};
} // namespace NScript
using NScript::CQ_BountyHuntScript;

void NScript::CQ_BountyHuntScript::Init()
{
    Game->AddQuestRegion("Q_BountyHunt", "LookoutPoint");
    Game->AddQuestRegion("Q_BountyHunt", "GreatwoodEntrance");
    Game->AddQuestRegion("Q_BountyHunt", "GreatwoodLake");
    Game->AddQuestRegion("Q_BountyHunt", "Fisherman");
    QuestFailed = false;
    QuestSucceeded = false;
    QuestOver = false;
    QuestStarted = false;
    FishermanDeactivated = false;
    VillagerKilled = false;
    VillagerIntroDone = false;
    LeaderKilled = false;
    Deputy1Killed = false;
    Deputy2Killed = false;
    Hostage1Killed = false;
    Hostage2Killed = false;
    Hostage1Safe = false;
    Hostage2Safe = false;
    Deputy1IntroComplete = false;
    Deputy2IntroComplete = false;
    Deputy1Escaped = false;
    LeaderEscaped = false;
    SpawnDeputy1Grunts = false;
    SpawnDeputy2Grunts = false;
    SpawnLeaderGrunts = false;
    AllDeputy1GruntsKilled = false;
    AllDeputy2GruntsKilled = false;
    AllLeaderGruntsKilled = false;
    AllLeaderChargeGruntsKilled = false;
    AllGruntsKilled = false;
    LeaderIntro1Complete = false;
    LeaderIntro2Complete = false;
    LeaderAttackingHostage = false;
    LeaderAttackingHero = false;
    LeaderRansomPaid = false;
    LeaderRansomDenied = false;
    LeaderLeftHostage = false;
    LeaderReachedMoney = false;
    LeaderAttackedAfterRansomPaid = false;
    LeaderChargeComplete = false;
    LeaderCutsceneComplete = false;
    BanditCutsceneDone = false;
    FishComplete = false;
    LakeComplete = false;
    BountyHuntIntroComplete = false;
    BanditGruntTextCounter = 0;
    BoastTime = pGlobalScriptDef->BountyHuntBoastTimeLimit;
    BoastQuestInfoDisplayed = false;
    BoastActiveTimeLimit = false;
    BoastActiveDecapitation = false;
    Data->BountyHuntWithinTimeLimit = false;
    Data->BountyHuntTimeLimitExceeded = false;
    Data->BountyHuntDecapitation = false;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->BountyHuntNakedBoastCost,
                   pGlobalScriptDef->BountyHuntNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->BountyHuntNoDamageBoastCost,
                   pGlobalScriptDef->BountyHuntNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TIMELIMIT", 15, pGlobalScriptDef->BountyHuntBoastCostTimeLimit,
                   pGlobalScriptDef->BountyHuntBoastRewardTimeLimit, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_DECAPITATELEADER", 16, pGlobalScriptDef->BountyHuntBoastCostDecapitation,
                   pGlobalScriptDef->BountyHuntBoastRewardDecapitation, false, "", 0);
}