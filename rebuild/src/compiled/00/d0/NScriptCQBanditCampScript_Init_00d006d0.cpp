// CQ_BanditCampScript::Init() -- register the six Bandit Camp quest regions, clear the quest flags, reset the master
// Leaf: ?Init@CQ_BanditCampScript@NScript@@
// data's bandit-kill boast results and offer the three Bandit Camp boasts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D006D0, length 768 bytes.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's
// interface (+0x40): AddQuestRegion (slot 271, +0x43c) x6 and AddBoast (slot 303, +0x4bc) x3.
// Data: the script def 0x0143E90C (retail offsets: BanditCampNoDamageBoast +0x88/+0x8c = Ego_r.pdb -4;
// KillNoBandits +0x3f4/+0x3f8 and KillManyBandits +0x3fc/+0x400 = Ego_r.pdb -0x34, the debug vectors before them).
// The quest master data (+0x44, CQ_SunnyvaleMasterData): BanditCampKillNoBandits +0x115, BanditCampKillManyBandits
// +0x11a (Ego_r.pdb offsets).
// Layout (Ego_r.pdb - 0x14): the six flags +0x48..+0x4d.
// Shape: each literal is an implicit conversion temporary built right to left before any push and destroyed after its
// call; the clears share one zero register (ebx) that the boasts reuse for their false/0 arguments.
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
    unsigned char _pad_0x000[0x88];
    long BanditCampNoDamageBoastCost;                  // +0x88
    long BanditCampNoDamageBoastReward;                // +0x8c
    unsigned char _pad_0x090[0x3f4 - 0x90];
    long BanditCampKillNoBanditsBoastCost;             // +0x3f4
    long BanditCampKillNoBanditsBoastReward;           // +0x3f8
    long BanditCampKillManyBanditsBoastCost;           // +0x3fc
    long BanditCampKillManyBanditsBoastReward;         // +0x400
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CQ_SunnyvaleMasterData {
    unsigned char _pad_0x000[0x115];
    bool BanditCampKillNoBandits;                      // +0x115
    bool BreakSiegeNoAssistance;                       // +0x116
    bool BreakSiegeKilledLeader;                       // +0x117
    bool OrchardFarmBanditKilled;                      // +0x118
    bool OrchardFarmGuardKilled;                       // +0x119
    bool BanditCampKillManyBandits;                    // +0x11a
};

namespace NScript {
class CQ_BanditCampScript {
public:
    unsigned char           _pad_0x00[0x40];           // CQuestScriptBase / CScriptBase
    CGameScriptInterface*   Game;                      // +0x40
    CQ_SunnyvaleMasterData* Data;                      // +0x44
    bool AttackedOuterGateGuards;                      // +0x48
    bool Gate1Open;                                    // +0x49
    bool BoastBegun;                                   // +0x4a
    bool MissionSucceeded;                             // +0x4b
    bool MissionFailed;                                // +0x4c
    bool ActivatedBanditRaid;                          // +0x4d
    void Init();
};
} // namespace NScript
using NScript::CQ_BanditCampScript;

void NScript::CQ_BanditCampScript::Init()
{
    Game->AddQuestRegion("Q_BanditCamp", "BanditCampPathEntrance");
    Game->AddQuestRegion("Q_BanditCamp", "BanditCampEntrance");
    Game->AddQuestRegion("Q_BanditCamp", "BanditCampPath1");
    Game->AddQuestRegion("Q_BanditCamp", "BanditCampCentre");
    Game->AddQuestRegion("Q_BanditCamp", "BanditCampBoss");
    Game->AddQuestRegion("Q_BanditCamp", "DemonDoor_BanditCampPath");
    AttackedOuterGateGuards = false;
    Gate1Open = false;
    MissionSucceeded = false;
    MissionFailed = false;
    BoastBegun = false;
    ActivatedBanditRaid = false;
    Data->BanditCampKillManyBandits = false;
    Data->BanditCampKillNoBandits = true;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->BanditCampNoDamageBoastCost,
                   pGlobalScriptDef->BanditCampNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLNOBANDITS", 24, pGlobalScriptDef->BanditCampKillNoBanditsBoastCost,
                   pGlobalScriptDef->BanditCampKillNoBanditsBoastReward, true, "", 1);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLMANYBANDITS", 27, pGlobalScriptDef->BanditCampKillManyBanditsBoastCost,
                   pGlobalScriptDef->BanditCampKillManyBanditsBoastReward, true, "", 1);
}