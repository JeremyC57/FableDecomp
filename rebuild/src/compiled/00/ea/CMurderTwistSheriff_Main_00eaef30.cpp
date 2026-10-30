// CMurderTwistSheriff::Main() -- the Murder Twist sheriff: alive filter on Me, melee-guard brain, scripted control (priority 4, retried each frame), registers himself with the parent quest, then runs his state machine: ask the hero to kill the assassin, remind him, play the being-rumbled cutscene once the hero is within 7 m, grow suspicious, and finally turn on the hero
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAEF30, 675 bytes (+ int3 padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90, ~CIsThingAlive 0x00E241B0 (out of line), SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, the scripted-thing
//   handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / operator= 0x008ABD10, CScriptThing::operator= 0x008AB980,
//   GetSquaredDistanceBetweenThings 0x00CBE583 (__fastcall, float); own state handlers 0x00EAF810, 0x00EB0830, 0x00EB10C0, 0x00EB15C0,
//   0x00EB19C0, 0x00EB1EC0; CV_MurderTwistFinalScript::PlayBeingRumbledCutscene 0x00EAF1E0; GameCalls slots 7, 8, 70, 589.
// Layout: CMurderTwistSheriff (Ego_r.pdb 0x38, matches retail; see the landed Init 0x00EAEE80). The parent quest script
//   CV_MurderTwistFinalScript is 0x14 smaller in retail than in the PDB view (SheriffState PDB +0x5c = retail +0x48,
//   AssassinState +0x60 = +0x4c, Sheriff +0x68 = +0x54, seh_Sheriff +0x84 = +0x6c after the 12-byte Guards vector).
// Shape: generated script code -- `if (IsActiveThreadTerminating()) return;` ahead of each statement that follows a wait.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
};
class CScriptGameResourceObjectScriptedThingBase { public:    // 0x10 bytes
    unsigned char _body[0x10];
    bool IsValid() const;                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                  // 0x00E241B0 (out of line here)
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
enum ESheriffState { SHERIFF_STATE_0, SHERIFF_STATE_1, SHERIFF_STATE_2, SHERIFF_STATE_3 };   // enumerator names not in the PDB dump
enum EAssassinState { ASSASSIN_STATE_0, ASSASSIN_STATE_1, ASSASSIN_STATE_2, ASSASSIN_STATE_3, ASSASSIN_STATE_4 };
struct CV_MurderTwistFinalScript {
    unsigned char _pad00[0x48];
    ESheriffState  SheriffState;                       // +0x48
    EAssassinState AssassinState;                      // +0x4c
    bool           TaskDeclined;                       // +0x50
    bool           PreemptiveAttackInProgress;         // +0x51
    CScriptThing   Sheriff;                            // +0x54
    unsigned char  Guards[0xc];                        // +0x60 vector<CScriptThing> (retail STLport, 12 bytes)
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;   // +0x6c
    void PlayBeingRumbledCutscene();                   // 0x00EAF1E0
};
class CTimer { public: unsigned long _handle; };
struct CMurderTwistSheriff : CEntityScriptBase {
    CV_MurderTwistFinalScript* ParentClass;            // +0x14
    void*                      PMasterData;            // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    CTimer                     AttractTimer;           // +0x2c
    unsigned long              NextLine;               // +0x30
    bool HasGivenReminderToHero;                       // +0x34
    bool HasBeingRumbledCutscenePlayed;                // +0x35
    bool HasExpressedSuspicionToHero;                  // +0x36
    bool HasSaidWellDoneToHero;                        // +0x37
    void Main();
    void DoAskingHeroToKillAssassin();                 // 0x00EAF810
    void DoRemindingHeroToKillAssassin();              // 0x00EB0830
    void DoBecomingSuspiciousOfHero();                 // 0x00EB10C0
    void DoRemindingHeroToKillSheriff();               // 0x00EB15C0 (catalog files it under CMurderTwistAssassin; called on the sheriff here)
    void DoSuspicious();                               // 0x00EB19C0 (catalog: a second DoBecomingSuspiciousOfHero label)
    void DoFightHero();                                // 0x00EB1EC0 (catalog "Init": GiveThingBestEnemyTarget hero, killable, damageable)
};

void CMurderTwistSheriff::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->SetCreatureBrain(Me, "BRAIN_MELEE_GUARD");
    if (MyEntityHandle.IsValid())
        MyEntityHandle.Release();
    while (!Game->StartScriptingEntity(Me, MyEntityHandle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->Sheriff = Me;
    ParentClass->seh_Sheriff = MyEntityHandle;
    while (ParentClass->SheriffState == SHERIFF_STATE_0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        DoAskingHeroToKillAssassin();
    }
    if (IsActiveThreadTerminating()) return;
    while (ParentClass->AssassinState == ASSASSIN_STATE_0 || ParentClass->AssassinState == ASSASSIN_STATE_1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        DoRemindingHeroToKillAssassin();
    }
    if (IsActiveThreadTerminating()) return;
    while (ParentClass->SheriffState == SHERIFF_STATE_1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!HasBeingRumbledCutscenePlayed && GetSquaredDistanceBetweenThings(Me, Game->GetHero()) <= 49.0f) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->PlayBeingRumbledCutscene();
            HasBeingRumbledCutscenePlayed = true;
        }
        if (ParentClass->AssassinState == ASSASSIN_STATE_3 || ParentClass->AssassinState == ASSASSIN_STATE_2) {
            if (IsActiveThreadTerminating()) return;
            DoBecomingSuspiciousOfHero();
        }
        if (ParentClass->AssassinState == ASSASSIN_STATE_4) {
            if (IsActiveThreadTerminating()) return;
            DoRemindingHeroToKillSheriff();
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (ParentClass->SheriffState == SHERIFF_STATE_3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        DoSuspicious();
    }
    if (IsActiveThreadTerminating()) return;
    if (ParentClass->SheriffState == SHERIFF_STATE_2) {
        if (IsActiveThreadTerminating()) return;
        DoFightHero();
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}