// NScript::CV_BeardyBaldyScript::~CV_BeardyBaldyScript() -- implicit village-script destructor: the ten RandomSpeech strings (count-down loop), the RandomSpeechTimer deregistration, the three required-style strings, then ~CScriptBase
// Leaf: ??1CV_BeardyBaldyScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E542B0, 81 bytes (+ int3 padding), thiscall; tail `jmp ~CScriptBase` 0x00CBD510.
// Identity (proven, two sources): the only caller 0x00E54293 is the scalar deleting destructor 0x00E54290, slot 0 of the RTTI-less vtable
//   0x012E5790; that vtable is stored only by the ctor 0x00E4F5B0, whose only caller is the Alloc 0x00E54260 (`push 0xA4` ; operator
//   new ; ctor), stored by the script-registration routine next to the save tag "S_VBB" (0x012F7764); refs/script_recovery/
//   conversion_queue.tsv lists that tag and allocator as the village script V_BeardyBaldy, i.e. NScript::CV_BeardyBaldyScript (FableWin.pdb
//   ??0CV_BeardyBaldyScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z and ??1CV_BeardyBaldyScript@NScript@@UAE@XZ). The body stores
//   no vtable of its own: the compiler-generated destructor. The catalog label is propagated and wrong (NAME_FIXES.tsv).
// Callees: ??1CCharString@@QAE@XZ 0x0099EAE0 (RandomSpeech[10] by an inline count-down loop, then RequiredTash, RequiredBeard,
//   RequiredHairdo), the inline ~CTimer (pDSTGame 0x0143E8F8 slot 88 DeregisterTimer), ??1CScriptBase@@UAE@XZ 0x00CBD510 (tail).
// Layout: Ego_r.pdb members at Ego_r offset - 0x14 (retail CScriptBase 0x40 vs Ego_r 0x54; CLevelScriptBase<T> Game +0x40 /
//   MasterScriptData +0x44) (QuestFinished +0x48 .. RequiredHairdo/Beard/Tash +0x58/+0x5C/+0x60, RandomSpeechTimer +0x74, RandomSpeech[10] +0x78,
//   BeardyBaldyPhase +0xA0 (an unnamed enum in Ego_r; enumerators not recovered); sizeof 0xA4 = the Alloc's `push 0A4h`). CLevelScriptBase<T> comes from engine/CLevelScriptBase.h with the retail opt-in
//   FABLE_RETAIL_CLEVELSCRIPTBASE (hdr/ overlay, chunk 713): retail's ~CLevelScriptBase<T> is compiler-generated, so the implicit
//   derived dtor tail-jumps ~CScriptBase; with the header's default out-of-line declaration it would jump to ??1?$CLevelScriptBase.
//   T = NScript::CQ_SunnyvaleMasterData as in the landed village-script rows (0x00E376D0, 0x00ED7640); T changes no byte or symbol
//   here and is not separately proven for this script.
// TU: the PDB ctor ??0CV_BeardyBaldyScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined (emission only, body not
//   reconstructed: the landed row is NScript::CV_BeardyBaldyScript 0x00E4F5B0), which emits the vtable, the scalar deleting dtor and this implicit dtor, as retail's TU does.
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
class CV_BeardyBaldyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {  // vtable 0x012E5790 (no RTTI)
public:
    CV_BeardyBaldyScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool        QuestFinished;                      // +0x48
    bool        QuestComplete;                      // +0x49
    bool        QuestActivated;                     // +0x4a
    long        QuestPhase;                         // +0x4c
    bool        InitialisePhase;                    // +0x50
    bool        IntroComplete;                      // +0x51
    bool        Phase1RequirementsComplete;         // +0x52
    bool        Phase2RequirementsComplete;         // +0x53
    bool        Phase3RequirementsComplete;         // +0x54
    CCharString RequiredHairdo;                     // +0x58
    CCharString RequiredBeard;                      // +0x5c
    CCharString RequiredTash;                       // +0x60
    bool        AcquiredNewHairdo;                  // +0x64
    bool        AcquiredNewBeard;                   // +0x65
    bool        AcquiredNewTash;                    // +0x66
    bool        AllHairChanged;                     // +0x67
    bool        BeardyBaldyKilled;                  // +0x68
    bool        BarberLadyKilled;                   // +0x69
    bool        BeardyBaldyLeft;                    // +0x6a
    long        IncorrectHairComboCount;            // +0x6c
    bool        AttackedByHero;                     // +0x70
    CTimer      RandomSpeechTimer;                  // +0x74
    CCharString RandomSpeech[10];                   // +0x78
    enum { } BeardyBaldyPhase;                      // +0xa0 (Ego_r: <unnamed-type-BeardyBaldyPhase>; enumerators not recovered)
};
FABLE_STATIC_ASSERT(sizeof(CV_BeardyBaldyScript) == 0xa4);

CV_BeardyBaldyScript::CV_BeardyBaldyScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E4F5B0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}