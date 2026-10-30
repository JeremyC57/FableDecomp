// NScript::CV_AmbushScamScript::OnPersist(CPersistContext& context) -- save/load the Ambush Scam quest script's state: thirty flags, the trickster approach proximity and the hit / buddy counters, each under its own member name with a zero default
// Leaf: ?OnPersist@CV_AmbushScamScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2AD50, 819 bytes, `ret 4`; slot 4 of the quest-script vtable 0x012E2428 stored by
//   the CV_AmbushScamScript ctor at 0x00E2AB7A (slot 1 the landed RegisterMain 0x00E2AC90, slot 3 the landed Init
//   0x00E2ABC0); Ego_r.pdb ?OnPersist@CV_AmbushScamScript@NScript@@UAEXAAVCPersistContext@@@Z. Catalog label: sub_.
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<float> 0x00410620, Transfer<long> 0x00410BE0
//   (Ego_r.pdb ??$Transfer@_N/@M/@J@CPersistContext@@QAEXPBDAA_NAB_N@Z etc.: name, value, const default).
// Strings: every persisted name is the member's own Ego_r.pdb name ("QuestFinished" 0x012C6FBC .. "BuddyCounter" 0x012E24D0).
// Layout (retail = Ego_r.pdb - 0x14, 0x94 bytes): Game +0x40, PMasterData +0x44, flags +0x48..+0x70 with
//   TricksterApproachProximity +0x5C, HitCount +0x74, BuddyCounter +0x78, RandomSpeechTimer +0x7C, RandomSpeech[5] +0x80;
//   every persisted offset matches the member its string names.
// Shape: one Transfer call per member in retail's order; each default is a temporary built in the dead argument slot.
class CCharString { public: void* PStringData; };
#define FABLE_RETAIL_NSCRIPT
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0 <bool>, 0x00410620 <float>, 0x00410BE0 <long>
};
class CGameScriptInterfaceBase;
class CScriptBase { public:                             // retail 0x40 bytes
    virtual ~CScriptBase();
    virtual void RegisterMain();                        // slot 1
    virtual void Main() = 0;                            // slot 2
    virtual void Init();                                // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
namespace NScript {
class CV_AmbushScamScript : public CScriptBase { public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*       PMasterData;                            // +0x44
    bool        QuestFinished;                       // +0x48
    bool        QuestComplete;                       // +0x49
    bool        QuestActivated;                      // +0x4a
    bool        LeftRegion;                          // +0x4b
    bool        MoralitySwitch;                      // +0x4c
    bool        BuddyIntroComplete;                  // +0x4d
    bool        DoTricksterIntro;                    // +0x4e
    bool        DoAmbush;                            // +0x4f
    bool        TricksterApproachComplete;           // +0x50
    bool        TricksterSpeechComplete;             // +0x51
    bool        TricksterRetreatComplete;            // +0x52
    bool        TricksterQuestComplete;              // +0x53
    bool        TricksterWhiningComplete;            // +0x54
    bool        TricksterDeadOrLeft;                 // +0x55
    bool        TricksterComplete;                   // +0x56
    bool        TricksterKilledBeforeAmbush;         // +0x57
    bool        TricksterKilledAfterAmbush;          // +0x58
    bool        TricksterKilledAfterBeingSpared;     // +0x59
    bool        TricksterResetAllegience;            // +0x5a
    bool        TricksterWaitHere;                   // +0x5b
    float       TricksterApproachProximity;          // +0x5c
    bool        AmbushSpringComplete;                // +0x60
    bool        AllAmbushersDead;                    // +0x61
    bool        ExpressionDetectedFollow;            // +0x62
    bool        ExpressionDetectedWait;              // +0x63
    bool        HeroAttackOnBuddy;                   // +0x64
    bool        HeroAttackOnTricksterBeforeAmbush;   // +0x65
    bool        HeroAttackOnTricksterAfterAmbush;    // +0x66
    bool        HeroAttackOnTricksterKillingBlow;    // +0x67
    bool        HeroTalkToTrickster;                 // +0x68
    bool        HeroAttachOnTricksterWarning;        // +0x69
    bool        BuddyHelped;                         // +0x6a
    bool        SpeakTime1;                          // +0x6b
    bool        SpeakTime2;                          // +0x6c
    bool        AllDone;                             // +0x6d
    bool        BanditSpoken;                        // +0x6e
    bool        AllBanditsDead;                      // +0x6f
    bool        LeaderDead;                          // +0x70
    long        HitCount;                            // +0x74
    long        BuddyCounter;                        // +0x78
    CTimer      RandomSpeechTimer;                  // +0x7c
    CCharString RandomSpeech[5];                    // +0x80
    virtual void Main();
    virtual void OnPersist(CPersistContext& context);
};
}

void NScript::CV_AmbushScamScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestFinished", QuestFinished, false);
    context.Transfer("QuestComplete", QuestComplete, false);
    context.Transfer("QuestActivated", QuestActivated, false);
    context.Transfer("LeftRegion", LeftRegion, false);
    context.Transfer("MoralitySwitch", MoralitySwitch, false);
    context.Transfer("AllAmbushersDead", AllAmbushersDead, false);
    context.Transfer("BuddyHelped", BuddyHelped, false);
    context.Transfer("HeroAttackOnBuddy", HeroAttackOnBuddy, false);
    context.Transfer("HeroAttackOnTricksterAfterAmbush", HeroAttackOnTricksterAfterAmbush, false);
    context.Transfer("HeroAttackOnTricksterBeforeAmbush", HeroAttackOnTricksterBeforeAmbush, false);
    context.Transfer("HeroAttackOnTricksterKillingBlow", HeroAttackOnTricksterKillingBlow, false);
    context.Transfer("HeroAttachOnTricksterWarning", HeroAttachOnTricksterWarning, false);
    context.Transfer("HeroTalkToTrickster", HeroTalkToTrickster, false);
    context.Transfer("BuddyIntroComplete", BuddyIntroComplete, false);
    context.Transfer("TricksterApproachComplete", TricksterApproachComplete, false);
    context.Transfer("TricksterSpeechComplete", TricksterSpeechComplete, false);
    context.Transfer("TricksterRetreatComplete", TricksterRetreatComplete, false);
    context.Transfer("TricksterQuestComplete", TricksterQuestComplete, false);
    context.Transfer("TricksterDeadOrLeft", TricksterDeadOrLeft, false);
    context.Transfer("TricksterWhiningComplete", TricksterWhiningComplete, false);
    context.Transfer("TricksterComplete", TricksterComplete, false);
    context.Transfer("TricksterKilledBeforeAmbush", TricksterKilledBeforeAmbush, false);
    context.Transfer("TricksterKilledAfterAmbush", TricksterKilledAfterAmbush, false);
    context.Transfer("TricksterKilledAfterBeingSpared", TricksterKilledAfterBeingSpared, false);
    context.Transfer("TricksterResetAllegience", TricksterResetAllegience, false);
    context.Transfer("TricksterWaitHere", TricksterWaitHere, false);
    context.Transfer("TricksterApproachProximity", TricksterApproachProximity, 0.0f);
    context.Transfer("AmbushSpringComplete", AmbushSpringComplete, false);
    context.Transfer("DoTricksterIntro", DoTricksterIntro, false);
    context.Transfer("DoAmbush", DoAmbush, false);
    context.Transfer("HitCount", HitCount, 0L);
    context.Transfer("BuddyCounter", BuddyCounter, 0L);
}