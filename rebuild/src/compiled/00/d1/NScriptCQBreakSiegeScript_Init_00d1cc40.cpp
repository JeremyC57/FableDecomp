// CQ_BreakSiegeScript::Init() -- register the two Break Siege quest regions, clear the quest flags, reset the master
// Leaf: ?Init@CQ_BreakSiegeScript@NScript@@
// data's siege boast results and offer the five Break Siege boasts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1CC40, length 696 bytes (padded to 704).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's
// interface (+0x40): AddQuestRegion (slot 271, +0x43c) x2 and AddBoast (slot 303, +0x4bc) x5.
// Data: the script def 0x0143E90C: BreakSiegeNaked/NoDamage/NoWeapons boasts +0xa8..+0xbc (Ego_r.pdb -4); the
// hire-no-guards / kill-bandit-leader boast values +0xb94..+0xba0 are TLC additions Ego_r.pdb lacks (names
// descriptive). The quest master data (+0x44, CQ_SunnyvaleMasterData): BreakSiegeNoAssistance +0x116,
// BreakSiegeKilledLeader +0x117 (Ego_r.pdb offsets).
// Layout (Ego_r.pdb - 0x14): flags +0x48..+0x56, ReasonForFailure +0x58.
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

class CScriptDef { public:
    unsigned char _pad_0x000[0xa8];
    long BreakSiegeNakedBoastCost;                     // +0xa8
    long BreakSiegeNakedBoastReward;                   // +0xac
    long BreakSiegeNoDamageBoastCost;                  // +0xb0
    long BreakSiegeNoDamageBoastReward;                // +0xb4
    long BreakSiegeNoWeaponsBoastCost;                 // +0xb8
    long BreakSiegeNoWeaponsBoastReward;               // +0xbc
    unsigned char _pad_0x0c0[0xb94 - 0xc0];
    long BreakSiegeHireNoGuardsBoastCost;              // +0xb94
    long BreakSiegeKillBanditLeaderBoastCost;          // +0xb98
    long BreakSiegeHireNoGuardsBoastReward;            // +0xb9c
    long BreakSiegeKillBanditLeaderBoastReward;        // +0xba0
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CQ_SunnyvaleMasterData {
    unsigned char _pad_0x000[0x116];
    bool BreakSiegeNoAssistance;                       // +0x116
    bool BreakSiegeKilledLeader;                       // +0x117
};

namespace NScript {
class CQ_BreakSiegeScript {
public:
    unsigned char           _pad_0x00[0x40];           // CQuestScriptBase / CScriptBase
    CGameScriptInterface*   Game;                      // +0x40
    CQ_SunnyvaleMasterData* Data;                      // +0x44
    bool MissionSucceeded;                             // +0x48
    bool MissionFailed;                                // +0x49
    bool BanditDemandsKnown;                           // +0x4a
    bool ChiefRefusesDemands;                          // +0x4b
    bool BanditLeaderFree;                             // +0x4c
    bool BanditSiegeStart;                             // +0x4d
    bool HasFirstWaveAttacked;                         // +0x4e
    bool StartSecondWave;                              // +0x4f
    bool BanditsAttackHero;                            // +0x50
    bool HeroLetInTown;                                // +0x51
    bool GuardFollowingHero;                           // +0x52
    bool EndCutsceneStarts;                            // +0x53
    bool EndCutsceneDone;                              // +0x54
    bool AttackedBanditLeader;                         // +0x55
    bool QuestStartScreened;                           // +0x56
    long ReasonForFailure;                             // +0x58
    void Init();
};
} // namespace NScript
using NScript::CQ_BreakSiegeScript;

void NScript::CQ_BreakSiegeScript::Init()
{
    Game->AddQuestRegion("Q_BreakSiege", "KnotholeGlade");
    Game->AddQuestRegion("Q_BreakSiege", "DemonDoor_KnotholeGlade");
    MissionSucceeded = false;
    MissionFailed = false;
    BanditDemandsKnown = false;
    ChiefRefusesDemands = false;
    BanditLeaderFree = false;
    BanditSiegeStart = false;
    BanditsAttackHero = false;
    HasFirstWaveAttacked = false;
    StartSecondWave = false;
    HeroLetInTown = false;
    GuardFollowingHero = false;
    EndCutsceneStarts = false;
    EndCutsceneDone = false;
    AttackedBanditLeader = false;
    QuestStartScreened = false;
    ReasonForFailure = 0;
    Data->BreakSiegeNoAssistance = true;
    Data->BreakSiegeKilledLeader = false;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->BreakSiegeNakedBoastCost,
                   pGlobalScriptDef->BreakSiegeNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->BreakSiegeNoDamageBoastCost,
                   pGlobalScriptDef->BreakSiegeNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->BreakSiegeNoWeaponsBoastCost,
                   pGlobalScriptDef->BreakSiegeNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_HIRENOGUARDS", 25, pGlobalScriptDef->BreakSiegeHireNoGuardsBoastCost,
                   pGlobalScriptDef->BreakSiegeHireNoGuardsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLBANDITLEADER", 26, pGlobalScriptDef->BreakSiegeKillBanditLeaderBoastCost,
                   pGlobalScriptDef->BreakSiegeKillBanditLeaderBoastReward, false, "", 0);
}