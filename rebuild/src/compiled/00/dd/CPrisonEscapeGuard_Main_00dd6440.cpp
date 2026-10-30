// CPrisonEscapeGuard::Main() -- a prison guard during the escape: whenever its alert timer has run out and it sees the hero or is hit by him (any hit except special ability 14), it raises the alarm once (take scripted control, face the hero, play ALARM, blow the race whistle, wait, then sound the bandit alarm on the quest) and shouts one of two "seen criminal" lines (picked at random once), restarting the 30-second timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD6440, length 1116 bytes (+ int3 padding), plain `ret`.
// Callees: rand 0x00BFEB16, IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing
//   handle IsScripting 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450 / dtor
//   0x007E74D0, ~CTimer 0x00CD4470, CCharString 0x0099EBF0 / 0x0099EAE0; g_pGameScriptInterface 0x0143E8F8 slots 87-90 (the
//   inline CTimer); GameCalls (+0x04) slots 7, 8, 70, 364-366, 376, 475, 632, 690, 692; CScriptThing slots 21/41/42.
// Strings: "SCRIPT_NAME_HERO" 0x0125D1C8, "ALARM" 0x012737BC, "SND_RACEWHISTLE" 0x012DC0F8, "SND_BANDIT_ALARM" 0x012DC0E4,
//   "TEXT_AI_GUARD_SEEN_CRIMINAL" 0x012DC0C8, "TEXT_AI_GUARD_SEEN_CRIMINAL_2" 0x012DC0A8. Global bool 0x01375748.
// Layout: Ego_r.pdb CPrisonEscapeGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_PrisonEscapeRescueMotherScript, retail = PDB - 0x14: AlarmSounded +0x79), PMasterData +0x18. Entity-script models
//   of the landed CGuardOutsideWoods::Main 0x00D874F0; CTimer of the landed CHobbeLeader::Main 0x00D8FA30.
// Shape: the CTimer destructor (DeregisterTimer) is inline at the exits outside the alarm block's IsPerformingScriptTask
//   wait and out of line (0x00CD4470) from that wait on (in-body inline_depth(0)).
enum EHeroAbility { HERO_ABILITY_NONE };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const;
    virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const;
    virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const;
    virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const;
    virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    virtual ~CScriptThing();                                    // 0x004AA840
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CScriptThingCopy : CBase {                       // CScriptThing as the filter holds it: out-of-line copy and destructor
    virtual void s001();
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThingCopy(const CScriptThing& other);        // 0x004ABE90
    ~CScriptThingCopy();                                // 0x004AA840
};
struct CIsThingAlive {                                  // vtable 0x012C32F0
    virtual void v00();
    CScriptThingCopy m_thing;                           // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void PlayAnimation(const CCharString& anim, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // 0x007E73D0
    bool IsPerformingScriptTask() const;               // 0x007E7450
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
extern "C" int __cdecl rand();                          // 0x00BFEB16
extern bool g_bScriptAnimationFlag;                     // 0x01375748
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }   // out-of-line copy 0x00CD4450
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }      // out-of-line copy 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
struct CQ_PrisonEscapeRescueMotherScript {
    unsigned char _pad[0x79];
    bool AlarmSounded;                                  // +0x79 (PDB +0x8d)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CPrisonEscapeGuard : CEntityScriptBase {
    CQ_PrisonEscapeRescueMotherScript* ParentClass;     // +0x14
    void* PMasterData;                                  // +0x18
    void Main();
};
void CPrisonEscapeGuard::Main()
{
    long line = rand() % 2 + 1;
    CTimer timer;
    while (!IsActiveThreadTerminating()) {
        if (timer.GetTime() == 0
            && (Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) || Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating())
                return;
            if (!ParentClass->AlarmSounded) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectScriptedThingBase guard;
                if (guard.IsScripting())
                    guard.Release();
                while (!Game->StartScriptingEntity(Me, guard, (EScriptAIPriority)4)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                guard.PlayAnimation("ALARM", false, false, false, true, g_bScriptAnimationFlag, false, false);
                Game->Pause(2.5f);
                Game->PlaySoundOnThing(Me, "SND_RACEWHISTLE");
                while (guard.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                }
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                if (!ParentClass->AlarmSounded) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    ParentClass->AlarmSounded = true;
                    Game->Play2DSound("SND_BANDIT_ALARM");
                }
            }
            timer.SetTime(30);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (line == 1) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_AI_GUARD_SEEN_CRIMINAL", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_AI_GUARD_SEEN_CRIMINAL_2", false, Me, Game->GetHero());
            }
        }
        Game->NewScriptFrame();
    }
}