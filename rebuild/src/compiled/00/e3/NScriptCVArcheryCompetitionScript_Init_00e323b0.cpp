// NScript::CV_ArcheryCompetitionScript::Init() -- reset the archery contest: state 0, every flag false, dummy counter 0, the game timer set to 0, and the ten random-speech text keys
// Leaf: ?Init@CV_ArcheryCompetitionScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E323B0, 231 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012E2EE4, which the landed NScript::CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript 0x00E32350 stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_ArcheryCompetitionScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CTimer::operator=(long) inline: g_pGameScriptInterface (0x0143E8F8) slot 89 SetTimer(TimerIndex, 0);
//   CCharString::operator=(const char*) 0x0099EFE0 (ten times).
class CCharString {
public:
    void* PData;
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ArcheryCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
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
    virtual void Init();
};

void CV_ArcheryCompetitionScript::Init()
{
    ContestState = CONTEST_STATE_0;
    IsOwnerDead = false;
    PrizeAlreadyGot = false;
    WelcomeComplete = false;
    Pickpocketed = false;
    AttackedByHero = false;
    GiveAttackWarning = false;
    QuestActivated = false;
    QuestFinished = false;
    SteppedOut = false;
    ShotOwner = false;
    UsedMagic = false;
    GivenReallyCoolPrize = false;
    GivenFirstHighScorePrize = false;
    AddedReallyCoolPrizeToOwner = false;
    AddedFirstHighScorePrizeToOwner = false;
    DummyCounter = 0;
    GameTimeLeft = 0;
    RandomSpeech[0] = "TEXT_QST_004_RANDOM_00";
    RandomSpeech[1] = "TEXT_QST_004_RANDOM_10";
    RandomSpeech[2] = "TEXT_QST_004_RANDOM_20";
    RandomSpeech[3] = "TEXT_QST_004_RANDOM_30";
    RandomSpeech[4] = "TEXT_QST_004_RANDOM_40";
    RandomSpeech[5] = "TEXT_QST_004_RANDOM_50";
    RandomSpeech[6] = "TEXT_QST_004_RANDOM_60";
    RandomSpeech[7] = "TEXT_QST_004_RANDOM_70";
    RandomSpeech[8] = "TEXT_QST_004_RANDOM_80";
    RandomSpeech[9] = "TEXT_QST_004_RANDOM_90";
}
}