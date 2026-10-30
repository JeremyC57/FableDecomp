// CQ_HobbeToothContestScript::Init() -- register the quest region, reset the contest state and the master-data boast flag, register the quest's four boasts and reset the Whisper comment timer
// Leaf: ?Init@CQ_HobbeToothContestScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D99E80, length 530 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; through the interface at +0x40:
//   AddQuestRegion (slot 271, +0x43c), AddBoast (slot 303, +0x4bc); the timer service (global 0x0143E8F8) SetTimer
//   (slot 89, +0x164) through an inline CTimer::Set.
// Data: the script def 0x0143E90C (retail = Ego_r.pdb CScriptDef - 4 - 4 per earlier STLport vector): HTContest boast
//   cost/reward pairs +0xf0..+0x104, HOB_KillMoreHobbesBoastCost/Reward +0xf34/+0xf38.
// Layout (retail = Ego_r.pdb CQ_HobbeToothContestScript - 0x14): PMasterData +0x44 (CQ_SunnyvaleMasterData,
//   HobbeContestKillMoreHobbes +0x11b), counters +0x48..+0x54, bools +0x58..+0x5d, WhisperCommentTimer +0x60,
//   QuestStartScreenShown +0x64.
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
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
class CScriptDef { public:
    unsigned char _pad0[0xf0];
    long HTContestNakedBoastCost;                       // +0xf0
    long HTContestNakedBoastReward;                     // +0xf4
    long HTContestNoDamageBoastCost;                    // +0xf8
    long HTContestNoDamageBoastReward;                  // +0xfc
    long HTContestNoWeaponsBoastCost;                   // +0x100
    long HTContestNoWeaponsBoastReward;                 // +0x104
    unsigned char _pad108[0xf34 - 0x108];
    long HOB_KillMoreHobbesBoastCost;                   // +0xf34
    long HOB_KillMoreHobbesBoastReward;                 // +0xf38
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x11b];
    bool HobbeContestKillMoreHobbes;                    // +0x11b
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HobbeToothContestScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    long NumberOfHobbesInTotal;                         // +0x48
    long NumberOfHobbeTeethCollectedByHero;             // +0x4c
    long NumberOfHobbeTeethCollectedByWhisper;          // +0x50
    long NumberOfHobbeTeethNeeded;                      // +0x54
    bool CS_IntroDone;                                  // +0x58
    bool CS_OutroBegin;                                 // +0x59
    bool DrinkingHobbesNotHit;                          // +0x5a
    bool MissionSucceeded;                              // +0x5b
    bool MissionFailed;                                 // +0x5c
    bool WhisperOrFarmerHit;                            // +0x5d
    CTimer WhisperCommentTimer;                         // +0x60
    bool QuestStartScreenShown;                         // +0x64
    void Init();
};
} // namespace NScript
using NScript::CQ_HobbeToothContestScript;

void NScript::CQ_HobbeToothContestScript::Init()
{
    Game->AddQuestRegion("Q_HobbeToothContest", "OrchardFarm");
    WhisperOrFarmerHit = false;
    NumberOfHobbesInTotal = 0;
    NumberOfHobbeTeethCollectedByHero = 0;
    NumberOfHobbeTeethCollectedByWhisper = 0;
    NumberOfHobbeTeethNeeded = 8;
    CS_IntroDone = false;
    CS_OutroBegin = false;
    DrinkingHobbesNotHit = true;
    MissionSucceeded = false;
    MissionFailed = false;
    QuestStartScreenShown = false;
    PMasterData->HobbeContestKillMoreHobbes = false;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->HTContestNakedBoastCost,
                   pGlobalScriptDef->HTContestNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->HTContestNoDamageBoastCost,
                   pGlobalScriptDef->HTContestNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->HTContestNoWeaponsBoastCost,
                   pGlobalScriptDef->HTContestNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLMOREHOBBES", 0x1c, pGlobalScriptDef->HOB_KillMoreHobbesBoastCost,
                   pGlobalScriptDef->HOB_KillMoreHobbesBoastReward, false, "", 0);
    WhisperCommentTimer = 0;
}