// CGuardOutsideWoods::Main() -- a guard outside the Guildmaster's woods: take scripted control, mark Me on the minimap, then every frame play the warning cutscene once when the hero first comes within 35, give three escalating warnings (within 30, 18, 10 units: "close", "closer" + HALT, "too close" -> attack), and attack the hero when hit by him or when the outside-wood guards already attack
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D874F0, 1418 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation
//   0x007E73D0 / dtor 0x007E74D0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CQ_HeroSoulsGuildmasterScript::
//   PlayCutscene(CCharString) 0x00D872D0 (label "Script_InitializeComponents" is wrong; name by analogy with the landed
//   CQ_AwakeningTheOracleScript::PlayCutscene 0x00CFF9A0), CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8, 70, 350,
//   352, 364-366, 451; CScriptThing slots 21/41/42 (hit-by messages). Global bool 0x01375748 (animation default).
// Layout: Ego_r.pdb CGuardOutsideWoods (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_HeroSoulsGuildmasterScript, retail = PDB - 0x14: OutsideWoodGuardsAttack +0x4b, GiveWarningCutscene +0x4c),
//   PMasterData +0x18, NumberOfWarnings +0x1c. Models from the landed CMuteGuardOutsideWoods::Main 0x00D86150.
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
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CScriptThingCopy : CBase {                       // CScriptThing as the filter holds it: out-of-line copy and destructor
    virtual void s001();
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThingCopy(const CScriptThing& other);        // 0x004ABE90
    ~CScriptThingCopy();                                // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThingCopy m_thing;                           // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern bool g_bScriptAnimationFlag;                     // 0x01375748
namespace NScript {
class CQ_HeroSoulsGuildmasterScript { public:
    unsigned char _pad[0x4b];
    bool OutsideWoodGuardsAttack;                       // +0x4b (PDB +0x5f)
    bool GiveWarningCutscene;                           // +0x4c (PDB +0x60)
    void PlayCutscene(CCharString name);                // 0x00D872D0
};
}
using NScript::CQ_HeroSoulsGuildmasterScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CGuardOutsideWoods : CEntityScriptBase {
    CQ_HeroSoulsGuildmasterScript* ParentClass;         // +0x14
    void* PMasterData;                                  // +0x18
    long NumberOfWarnings;                              // +0x1c
    void Main();
};

void CGuardOutsideWoods::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    while (!IsActiveThreadTerminating()) {
        if (ParentClass->GiveWarningCutscene == true && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 35.0f)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GiveWarningCutscene = false;
            ParentClass->PlayCutscene("CS_WARNEDBYGUARDS");
        }
        switch (NumberOfWarnings) {
        case 0:
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 30.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GUILDWOODSGUARD_CLOSE_10", false, Me, Game->GetHero());
                ++NumberOfWarnings;
                guard.PlayAnimation("HALT", false, false, false, false, g_bScriptAnimationFlag, false, false);
            }
            break;
        case 1:
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 18.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GUILDWOODSGUARD_CLOSER_10", false, Me, Game->GetHero());
                ++NumberOfWarnings;
                guard.PlayAnimation("HALT", false, false, false, false, g_bScriptAnimationFlag, false, false);
            }
            break;
        case 2:
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GUILDWOODSGUARD_TOO_CLOSE_10", false, Me, Game->GetHero());
                ParentClass->OutsideWoodGuardsAttack = true;
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                Game->MiniMapRemoveMarker(Me);
                if (guard.IsValid())
                    guard.Release();
                do {
                    Game->NewScriptFrame();
                } while (!IsActiveThreadTerminating());
                return;
            }
            break;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))
            || ParentClass->OutsideWoodGuardsAttack) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->OutsideWoodGuardsAttack = true;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            Game->MiniMapRemoveMarker(Me);
            if (guard.IsValid())
                guard.Release();
            do {
                Game->NewScriptFrame();
            } while (!IsActiveThreadTerminating());
            return;
        }
        Game->NewScriptFrame();
    }
}