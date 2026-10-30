// CPrisonGuard::Main() -- the prison guard who starts the prison race: once the prison cutscene has finished take scripted control (or, when the hero stole the prison key, remove the guard), walk to GuardPatrolExit and back home (re-issuing the move whenever the last one finished, until within 2 units), pause, announce the race in three lines, wait for the conversation, pause and flag RaceStarting
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD44C0, length 1241 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0; CScriptThing copy 0x004ABE90 / dtor 0x004AA840), RegisterFilter
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle
//   IsScripting 0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0 / IsPerformingScriptTask 0x007E7450 /
//   GetScriptThing 0x007E7490 / dtor 0x007E74D0, IsDistanceFromThingToPositionOver 0x00CBE45C (__fastcall), CCharString
//   0x0099EBF0 / 0x0099EAE0; GameCalls (+0x04) slots 7, 8, 70, 72, 108, 364-366, 368, 376, 512; CScriptThing slots 6 GetPos,
//   7 GetHomePos.
// Strings: "GuardPatrolExit" 0x012DBDB8, "TEXT_QST_055_PRISON_START_RACE_01/02/03" 0x012DBECC / 0x012DBEA8 / 0x012DBE84.
// Layout: Ego_r.pdb CPrisonGuard (40 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_PrisonEscapeRescueMotherScript, retail = PDB - 0x14: PrisonCutsceneFinished +0x6a, RaceStarting +0x6b), PMasterData
//   +0x18 (CQ_SunnyvaleMasterData, no shift: PrisonKeyStolenByHero +0xee). Entity-script models of the landed
//   CGuardOutsideWoods::Main 0x00D874F0.
namespace NScript { class CPredicateBase; }
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
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual const C3DVector& GetPos() const; virtual C3DVector GetHomePos() const;   // slots 6 (+0x18), 7 (+0x1c) virtual void s008() const;
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
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ECutsceneBehaviour { CUTSCENE_BEHAVIOUR_NONE };
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04 (out-of-line copy 0x004ABE90 / dtor 0x004AA840)
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
struct CQ_PrisonEscapeRescueMotherScript {
    unsigned char _pad[0x6a];
    bool PrisonCutsceneFinished;                        // +0x6a (PDB +0x7e)
    bool RaceStarting;                                  // +0x6b (PDB +0x7f)
};
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0xee];
    bool PrisonKeyStolenByHero;                         // +0xee
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CPrisonGuard : CEntityScriptBase {
    CQ_PrisonEscapeRescueMotherScript* ParentClass;     // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    void Main();
};
void CPrisonGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guard;
    ParentClass->PrisonCutsceneFinished = false;
    while (!ParentClass->PrisonCutsceneFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (PMasterData->PrisonKeyStolenByHero) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
        return;
    }
    Game->EntitySetCutsceneBehaviour(Me, (ECutsceneBehaviour)1);
    {
        C3DVector exit = Game->GetThingWithScriptName("GuardPatrolExit").GetPos();
        while (IsDistanceFromThingToPositionOver(guard.GetScriptThing(), exit, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            guard.MoveToPosition(exit, 0.0f, (EScriptEntityMoveType)0, false, true);
            while (guard.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    {
        C3DVector home = Me.GetHomePos();
        while (IsDistanceFromThingToPositionOver(guard.GetScriptThing(), home, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            guard.MoveToPosition(home, 0.0f, (EScriptEntityMoveType)0, false, true);
            while (guard.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(2.0f);
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_START_RACE_01", false, Me, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_START_RACE_02", false, Me, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_START_RACE_03", false, Me, Game->GetHero());
    while (Game->IsConversationActive(conversation)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(2.0f);
    ParentClass->RaceStarting = true;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}