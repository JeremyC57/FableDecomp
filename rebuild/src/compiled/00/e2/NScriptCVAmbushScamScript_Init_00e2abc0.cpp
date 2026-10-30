// NScript::CV_AmbushScamScript::Init() -- reset the ambush-scam quest state: every progress flag false, proximity 0, hit count 0, buddy counter 1, and the five random-speech text keys
// Leaf: ?Init@CV_AmbushScamScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2ABC0, 205 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012E2428, which the constructor 0x00E2AB60 stores
//   (landed under the stand-in name CVtable_012E2428::CVtable_012E2428). Its Alloc 0x00E322C0 news 0x94 bytes (Ego_r.pdb
//   CV_AmbushScamScript is 0xA8 = 0x94 + 0x14) and is registered under "S_VAS" at 0x00CD7FD4; the members (twenty bools,
//   a float, seventeen bools, two longs, a CTimer, CCharString[5]) are Ego_r.pdb CV_AmbushScamScript's, the only class with a
//   CCharString[5] at Ego_r +0x94. Level-script slots: 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist.
//   Ego_r.pdb has ?Init@CV_AmbushScamScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_AmbushScamScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString::operator=(const char*) 0x0099EFE0 (five times).
// Stores in retail order (the source order); TricksterApproachProximity's 0.0f is an integer store of the zero register.
class CCharString {
public:
    void* PData;
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
};
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AmbushScamScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFinished;                                 // +0x48 .. +0x5b
    bool QuestComplete;
    bool QuestActivated;
    bool LeftRegion;
    bool MoralitySwitch;
    bool BuddyIntroComplete;
    bool DoTricksterIntro;
    bool DoAmbush;
    bool TricksterApproachComplete;
    bool TricksterSpeechComplete;
    bool TricksterRetreatComplete;
    bool TricksterQuestComplete;
    bool TricksterWhiningComplete;
    bool TricksterDeadOrLeft;
    bool TricksterComplete;
    bool TricksterKilledBeforeAmbush;
    bool TricksterKilledAfterAmbush;
    bool TricksterKilledAfterBeingSpared;
    bool TricksterResetAllegience;
    bool TricksterWaitHere;
    float TricksterApproachProximity;                   // +0x5c
    bool AmbushSpringComplete;                          // +0x60 .. +0x70
    bool AllAmbushersDead;
    bool ExpressionDetectedFollow;
    bool ExpressionDetectedWait;
    bool HeroAttackOnBuddy;
    bool HeroAttackOnTricksterBeforeAmbush;
    bool HeroAttackOnTricksterAfterAmbush;
    bool HeroAttackOnTricksterKillingBlow;
    bool HeroTalkToTrickster;
    bool HeroAttachOnTricksterWarning;
    bool BuddyHelped;
    bool SpeakTime1;
    bool SpeakTime2;
    bool AllDone;
    bool BanditSpoken;
    bool AllBanditsDead;
    bool LeaderDead;
    long HitCount;                                      // +0x74
    long BuddyCounter;                                  // +0x78
    CTimer RandomSpeechTimer;                           // +0x7c
    CCharString RandomSpeech[5];                        // +0x80
    virtual void Init();
};

void CV_AmbushScamScript::Init()
{
    SpeakTime1 = false;
    SpeakTime2 = false;
    AllDone = false;
    BanditSpoken = false;
    AllBanditsDead = false;
    LeaderDead = false;
    QuestFinished = false;
    QuestComplete = false;
    QuestActivated = false;
    LeftRegion = false;
    AllAmbushersDead = false;
    BuddyHelped = false;
    HeroAttackOnBuddy = false;
    HeroAttackOnTricksterBeforeAmbush = false;
    HeroAttackOnTricksterAfterAmbush = false;
    HeroAttackOnTricksterKillingBlow = false;
    HeroTalkToTrickster = false;
    HeroAttachOnTricksterWarning = false;
    MoralitySwitch = false;
    BuddyIntroComplete = false;
    TricksterApproachComplete = false;
    TricksterSpeechComplete = false;
    TricksterRetreatComplete = false;
    TricksterQuestComplete = false;
    TricksterWhiningComplete = false;
    TricksterDeadOrLeft = false;
    TricksterComplete = false;
    TricksterKilledBeforeAmbush = false;
    TricksterKilledAfterAmbush = false;
    TricksterKilledAfterBeingSpared = false;
    TricksterResetAllegience = false;
    TricksterWaitHere = false;
    TricksterApproachProximity = 0.0f;
    AmbushSpringComplete = false;
    DoTricksterIntro = false;
    DoAmbush = false;
    HitCount = 0;
    BuddyCounter = 1;
    RandomSpeech[0] = "TEXT_QST_002_RANDOM_00";
    RandomSpeech[1] = "TEXT_QST_002_RANDOM_10";
    RandomSpeech[2] = "TEXT_QST_002_RANDOM_20";
    RandomSpeech[3] = "TEXT_QST_002_RANDOM_30";
    RandomSpeech[4] = "TEXT_QST_002_RANDOM_40";
}
}