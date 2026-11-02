// NScript::CV_AmbushScamScript::~CV_AmbushScamScript() -- implicit village-script destructor: the five RandomSpeech strings (count-down loop), the RandomSpeechTimer deregistration, then ~CScriptBase
// Leaf: ??1CV_AmbushScamScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E32310, 57 bytes (+ int3 padding), thiscall; tail `jmp ~CScriptBase` 0x00CBD510.
// Identity (proven, two sources): the only caller 0x00E322F3 is the scalar deleting destructor 0x00E322F0, slot 0 of the RTTI-less vtable
//   0x012E2428; that vtable is stored only by the ctor 0x00E2AB60, whose only caller is the Alloc 0x00E322C0 (`push 0x94` ; operator
//   new ; ctor), stored by the script-registration routine next to the save tag "S_VAS" (0x012F778C); refs/script_recovery/
//   conversion_queue.tsv lists that tag and allocator as the village script V_AmbushScam, i.e. NScript::CV_AmbushScamScript (FableWin.pdb
//   ??0CV_AmbushScamScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z and ??1CV_AmbushScamScript@NScript@@UAE@XZ). The body stores
//   no vtable of its own: the compiler-generated destructor. The catalog label is propagated and wrong (NAME_FIXES.tsv).
// Callees: ??1CCharString@@QAE@XZ 0x0099EAE0 (RandomSpeech[5], an inline count-down loop), the inline ~CTimer (engine/CTimer.h:
//   pDSTGame 0x0143E8F8 slot 88 DeregisterTimer(TimerIndex) at +0x160), ??1CScriptBase@@UAE@XZ 0x00CBD510 (tail).
// Layout: Ego_r.pdb members at Ego_r offset - 0x14 (retail CScriptBase 0x40 vs Ego_r 0x54; CLevelScriptBase<T> Game +0x40 /
//   MasterScriptData +0x44) (members from QuestFinished +0x48 to RandomSpeechTimer +0x7C (CTimer) and RandomSpeech[5] +0x80; sizeof 0x94 = the Alloc's
//   `push 94h`). CLevelScriptBase<T> comes from engine/CLevelScriptBase.h with the retail opt-in
//   FABLE_RETAIL_CLEVELSCRIPTBASE (hdr/ overlay, chunk 713): retail's ~CLevelScriptBase<T> is compiler-generated, so the implicit
//   derived dtor tail-jumps ~CScriptBase; with the header's default out-of-line declaration it would jump to ??1?$CLevelScriptBase.
//   T = NScript::CQ_SunnyvaleMasterData as in the landed village-script rows (0x00E376D0, 0x00ED7640); T changes no byte or symbol
//   here and is not separately proven for this script.
// TU: the PDB ctor ??0CV_AmbushScamScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined (emission only, body not
//   reconstructed: the landed row is CVtable_012E2428 0x00E2AB60, a descriptive stand-in for this ctor), which emits the vtable, the scalar deleting dtor and this implicit dtor, as retail's TU does.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "fable_string.h"                     // CCharString (~CCharString 0x0099EAE0)
#define FABLE_RETAIL_CLEVELSCRIPTBASE         // retail's implicit ~CLevelScriptBase<T> (hdr/ overlay, chunk 713)
#include "engine/CLevelScriptBase.h"          // CLevelScriptBase<T> : CScriptBase (Game +0x40, MasterScriptData +0x44)
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AmbushScamScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {   // vtable 0x012E2428 (no RTTI)
public:
    CV_AmbushScamScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool        QuestFinished;                      // +0x48
    bool        QuestComplete;                      // +0x49
    bool        QuestActivated;                     // +0x4a
    bool        LeftRegion;                         // +0x4b
    bool        MoralitySwitch;                     // +0x4c
    bool        BuddyIntroComplete;                 // +0x4d
    bool        DoTricksterIntro;                   // +0x4e
    bool        DoAmbush;                           // +0x4f
    bool        TricksterApproachComplete;          // +0x50
    bool        TricksterSpeechComplete;            // +0x51
    bool        TricksterRetreatComplete;           // +0x52
    bool        TricksterQuestComplete;             // +0x53
    bool        TricksterWhiningComplete;           // +0x54
    bool        TricksterDeadOrLeft;                // +0x55
    bool        TricksterComplete;                  // +0x56
    bool        TricksterKilledBeforeAmbush;        // +0x57
    bool        TricksterKilledAfterAmbush;         // +0x58
    bool        TricksterKilledAfterBeingSpared;    // +0x59
    bool        TricksterResetAllegience;           // +0x5a
    bool        TricksterWaitHere;                  // +0x5b
    float       TricksterApproachProximity;         // +0x5c
    bool        AmbushSpringComplete;               // +0x60
    bool        AllAmbushersDead;                   // +0x61
    bool        ExpressionDetectedFollow;           // +0x62
    bool        ExpressionDetectedWait;             // +0x63
    bool        HeroAttackOnBuddy;                  // +0x64
    bool        HeroAttackOnTricksterBeforeAmbush;  // +0x65
    bool        HeroAttackOnTricksterAfterAmbush;   // +0x66
    bool        HeroAttackOnTricksterKillingBlow;   // +0x67
    bool        HeroTalkToTrickster;                // +0x68
    bool        HeroAttachOnTricksterWarning;       // +0x69
    bool        BuddyHelped;                        // +0x6a
    bool        SpeakTime1;                         // +0x6b
    bool        SpeakTime2;                         // +0x6c
    bool        AllDone;                            // +0x6d
    bool        BanditSpoken;                       // +0x6e
    bool        AllBanditsDead;                     // +0x6f
    bool        LeaderDead;                         // +0x70
    long        HitCount;                           // +0x74
    long        BuddyCounter;                       // +0x78
    CTimer      RandomSpeechTimer;                  // +0x7c
    CCharString RandomSpeech[5];                    // +0x80
};
FABLE_STATIC_ASSERT(sizeof(CV_AmbushScamScript) == 0x94);

CV_AmbushScamScript::CV_AmbushScamScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E2AB60
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}