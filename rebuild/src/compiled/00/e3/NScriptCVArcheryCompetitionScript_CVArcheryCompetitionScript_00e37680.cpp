// NScript::CV_ArcheryCompetitionScript::~CV_ArcheryCompetitionScript() -- implicit village-script destructor: destroy the ten RandomSpeech strings (inline count-down loop), deregister RandomSpeechTimer then GameTimeLeft, then ~CScriptBase
// Leaf: ??1CV_ArcheryCompetitionScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E37680, 75 bytes (int3 padding follows), thiscall, tail `jmp ~CScriptBase` 0x00CBD510.
// Identity (proven, two sources): (1) the sole caller 0x00E37663 is the scalar deleting destructor 0x00E37660, slot 0 of the RTTI-less
//   vtable 0x012E2EE4, which the landed ctor 0x00E32350 stores; that ctor's only caller is the Alloc 0x00E37630 (`push 0A4h ; call new`)
//   registered for "V_ArcheryCompetition" / "S_VAC" (refs/script_recovery/conversion_queue.tsv), and slots 1..4 of the vtable are the
//   landed RegisterMain 0x00E324A0 / Main 0x00E32540 / Init 0x00E323B0 / OnPersist 0x00E37520 of NScript::CV_ArcheryCompetitionScript;
//   (2) Ego_r.pdb 0x00E86990 / FableWin.pdb 0x03942D10 ??1CV_ArcheryCompetitionScript@NScript@@UAE@XZ. No vtable store of its own:
//   the destructor is compiler-generated. This TU's ??_G grades RELOCATION_MATCH against 0x00E37660 (catalog: CVtable_012e2ee4).
// Callees: ~CCharString 0x0099EAE0 (ten times), CTimer::~CTimer inline (pDSTGame 0x0143E8F8 slot 88 DeregisterTimer, engine/CTimer.h),
//   ~CScriptBase 0x00CBD510 (tail).
// Shape: members destroyed in reverse order (RandomSpeech[10] from the top, RandomSpeechTimer +0x70, GameTimeLeft +0x6C); the
//   CLevelScriptBase<T> dtor is the compiler-generated one (engine/CLevelScriptBase.h FABLE_RETAIL_CLEVELSCRIPTBASE, chunk 713), so the
//   tail goes straight to ~CScriptBase. The PDB ctor is defined (empty body, not graded) so the vtable, ??_G and this ??1 are emitted.
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript members 0x14 lower (retail CScriptBase 0x40; no STL member): Score +0x48 .. ContestState
//   +0xA0, retail sizeof 0xA4 (the Alloc's `push 0A4h`). CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> as the landed archery rows
//   (0x00E37520, 0x00E323B0). No engine/CCharString.h: CCharString is the one-pointer local model (PDB member PStringData).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CScriptBase.h"
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/CLevelScriptBase.h"

class CCharStringData;
class CCharString { public:
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ArcheryCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {   // vtable 0x012E2EE4
public:
    CV_ArcheryCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);  // 0x00E32350
    long Score;                                         // +0x48
    bool IsOwnerDead;                                   // +0x4c
    bool PrizeAlreadyGot;                               // +0x4d
    bool QuestActivated;                                // +0x4e
    bool AttackedByHero;                                // +0x4f
    bool GiveAttackWarning;                             // +0x50
    bool QuestFinished;                                 // +0x51
    bool WelcomeComplete;                               // +0x52
    bool Pickpocketed;                                  // +0x53
    long DummyCounter;                                  // +0x54
    long ArcheryHighScoreInfoID;                        // +0x58
    long ArcheryGameScoreInfoID;                        // +0x5c
    long ArcheryGameTimerID;                            // +0x60
    bool SteppedOut;                                    // +0x64
    bool UsedMagic;                                     // +0x65
    bool ShotOwner;                                     // +0x66
    bool GivenReallyCoolPrize;                          // +0x67
    bool GivenFirstHighScorePrize;                      // +0x68
    bool AddedReallyCoolPrizeToOwner;                   // +0x69
    bool AddedFirstHighScorePrizeToOwner;               // +0x6a
    CTimer GameTimeLeft;                                // +0x6c
    CTimer RandomSpeechTimer;                           // +0x70
    CCharString RandomSpeech[10];                       // +0x74
    enum { SCORE_POWER_LEVEL_0 } ScorePowerLevel;       // +0x9c (unnamed enum in the PDB)
    enum { CONTEST_STATE_0 } ContestState;              // +0xa0 (unnamed enum in the PDB)
};
CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ArcheryCompetitionScript) == 0xA4);