// CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, interface/owner pair, vtable 0x012E2EE4, two registered timers, ten empty random-speech strings
// Leaf: ??0CV_ArcheryCompetitionScript
// Retail: Fable.exe @ 0x00E32350, 93 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8 (twice),
// CCharString::CCharString() @ 0x0099E4B0 (ten times, inline counted loop).
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript minus 0x14 (the retail script base is 0x14 bytes smaller):
// GameTimeLeft (CTimer) +0x6c, RandomSpeechTimer (CTimer) +0x70, RandomSpeech (CCharString[10]) +0x74.
// Shape (landed 0x00E68F70 model): the arguments are stored before the vtable by an inlined
// `__declspec(novtable)` intermediate base (interface at +0x40, owner at +0x44); each CTimer member registers
// itself in its inline constructor; the string array is built by an inline `dec ebx ; jne` loop (no EH module).
#include "fable_string.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

struct CV_ArcheryCompetitionScript : CScriptObject { // vtable 0x012E2EE4
    long Score;                                      // +0x48
    bool IsOwnerDead;                                // +0x4c
    bool PrizeAlreadyGot;                            // +0x4d
    bool QuestActivated;                             // +0x4e
    bool AttackedByHero;                             // +0x4f
    bool GiveAttackWarning;                          // +0x50
    bool QuestFinished;                              // +0x51
    bool WelcomeComplete;                            // +0x52
    bool Pickpocketed;                               // +0x53
    long DummyCounter;                               // +0x54
    long ArcheryHighScoreInfoID;                     // +0x58
    long ArcheryGameScoreInfoID;                     // +0x5c
    long ArcheryGameTimerID;                         // +0x60
    bool SteppedOut;                                 // +0x64
    bool UsedMagic;                                  // +0x65
    bool ShotOwner;                                  // +0x66
    bool GivenReallyCoolPrize;                       // +0x67
    bool GivenFirstHighScorePrize;                   // +0x68
    bool AddedReallyCoolPrizeToOwner;                // +0x69
    bool AddedFirstHighScorePrizeToOwner;            // +0x6a
    CTimer GameTimeLeft;                             // +0x6c
    CTimer RandomSpeechTimer;                        // +0x70
    CCharString RandomSpeech[10];                    // +0x74
    long ScorePowerLevel;                            // +0x9c
    long ContestState;                               // +0xa0
    virtual void v00();
    CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game);
};

CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}