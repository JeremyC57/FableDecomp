// CExpression_DigScript::Main() -- the hero "dig" expression quest: register a conscious-hero predicate, mark the quest persistent, wait until the hero is player-controlled, take the hero under scripted control, then inside a cutscene/movie sequence fade out, sheathe weapons, give the hero the spade, play the digging idle loop, look for the nearest enabled digging spot within 3m (face it if found), dig, play the success or failure animation, fade, restore the weapon the hero was wielding, reset the camera, hand over the spot's contents if found and deactivate this quest later
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EEA7C0, length 1506 bytes (+ int3 padding), no EH frame.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 / IsAlive 0x004AB130, Is2DDistanceBetweenThingsUnder 0x00CBE371 (__fastcall),
//   CScriptBase::SetPredicate 0x00CB7920 / IsActiveThreadTerminating 0x00CB7940, CBaseObject ctor 0x0099A380, scripted-thing dtor 0x007E74D0,
//   ClearCommands 0x007E7360 / PlayAnimation 0x007E73D0 / PlayLoopingAnimation 0x007E73F0 / ClearAllActionsIncludingLoopingAnimations 0x007E7410,
//   movie object ctor 0x006E7B60 / dtor 0x006E7B80 (both out of line here), CCharString 0x0099EBF0 / 0x0099EAE0; CGameScriptInterface slots as listed below.
//   Literals "" 0x0122D70E, "OBJECT_SPADE" 0x012F5674, "ST_DIGGING_IDLE" 0x012F5664, "ST_DIGGING_SUCCESS" 0x012F5650, "ST_DIGGING_FAILURE" 0x012F563C.
//   Manifest labels 0x006E7B60 (CCarriedReadableDef), 0x004AA840 (~C3DClothPrimitive), 0x007E74D0 are propagated and wrong for this row.
// Layout: the retail quest-script base (as the landed CExpressionWaitScript_Main 00eea460): CScriptBase 0x40 bytes, the interface pointer at +0x40.
// Shape: the found flag is ONE bool initialised from the && chain and tested by if (found); an if/else assigning true/false places the
//   found = false block inline instead of after the success arm. The discarded GetPos/GetAngleXY calls stay (virtual), the float result popped.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};

class CRGBColour { public:
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) { }
};

class C3DVector { public: float X, Y, Z; };

class CScriptThing { public:                                      // 12 bytes: vfptr + PImp
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;               // slot 6 (+0x18)
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual float GetAngleXY() const;                      // slot 10 (+0x28)
    void* PImp[2];
    CScriptThing(const CScriptThing& other);               // 0x004ABE90
    virtual ~CScriptThing();                                       // 0x004AA840
    bool IsAlive() const;                                  // 0x004AB130
};

bool __fastcall Is2DDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE371

struct CIsThingConscious {                                 // vtable 0x012C2FE8
    CScriptThing Thing;                                    // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingConscious() { }
    virtual bool operator()() const;
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectBase : public CBaseObject { public:       // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() { }
};

struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;                             // +0x08
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();         // 0x007E74D0
    virtual void v01();
    void ClearCommands();                                  // 0x007E7360
    void PlayAnimation(const CCharString& name, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // 0x007E73D0
    void PlayLoopingAnimation(const CCharString& name, long count, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // 0x007E73F0
    void ClearAllActionsIncludingLoopingAnimations();      // 0x007E7410
};

class CScriptGameResourceObjectMovieBase { public:                // 0x10 bytes
    void* _body[4];
    CScriptGameResourceObjectMovieBase();                  // 0x006E7B60
    ~CScriptGameResourceObjectMovieBase();                 // 0x006E7B80
};

extern bool g_bScriptAnimationFlag;                        // 0x01375748 (as the landed CHTEAssaultGuard_Main 00d6aea0)

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                       // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    void SetPredicate(CIsThingConscious& predicate);       // 0x00CB7920
    bool IsActiveThreadTerminating();                      // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* GSI;                             // +0x40
};

struct CExpression_DigScript : CQuestScriptBase {
    void Main();
};

void CExpression_DigScript::Main()
{
    {
        CIsThingConscious predicate(GSI->GetHero());
        SetPredicate(predicate);
    }
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->SetQuestAsPersistent(GSI->GetActiveQuestName(), false);
    while (!GSI->IsHeroControlledByPlayer()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase hero;
    while (!GSI->StartScriptingEntity(GSI->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->SetCutsceneMode(true, false);
    CScriptGameResourceObjectMovieBase movie;
    GSI->StartMovieSequence("", movie);
    GSI->PauseAllEntities(true);
    GSI->FadeScreenOut(0.25f, 0.25f, CRGBColour(0, 0, 0, 255));
    GSI->Pause(0.5f);
    GSI->EntityForceToLookAtNothing(GSI->GetHero());
    GSI->GetHero().GetPos();
    GSI->GetHero().GetAngleXY();
    GSI->GetHero().GetPos();
    bool melee = GSI->IsEntityWieldingMeleeWeapon(GSI->GetHero());
    bool ranged = GSI->IsEntityWieldingRangedWeapon(GSI->GetHero());
    GSI->SheatheHeroWeapons();
    hero.ClearAllActionsIncludingLoopingAnimations();
    GSI->MakeHeroCarryItemInHand("OBJECT_SPADE");
    GSI->FadeScreenIn();
    GSI->Pause(0.5f);
    hero.PlayLoopingAnimation("ST_DIGGING_IDLE", 2, false, false, false, true, g_bScriptAnimationFlag, false, false);
    CScriptThing spot = GSI->GetNearestEnabledDiggingSpot(GSI->GetHero());
    bool found = spot.IsAlive() && Is2DDistanceBetweenThingsUnder(GSI->GetHero(), spot, 3.0f);
    if (found) {
        if (IsActiveThreadTerminating())
            return;
        GSI->EntitySetFacingAngleTowardsThing(GSI->GetHero(), spot, true);
    }
    GSI->HeroGoDigging();
    GSI->Pause(0.25f);
    if (found) {
        if (IsActiveThreadTerminating())
            return;
        hero.ClearCommands();
        hero.PlayAnimation("ST_DIGGING_SUCCESS", false, false, false, true, g_bScriptAnimationFlag, false, false);
        GSI->Pause(0.55f);
    } else {
        if (IsActiveThreadTerminating())
            return;
        hero.ClearCommands();
        hero.PlayAnimation("ST_DIGGING_FAILURE", false, false, false, true, g_bScriptAnimationFlag, false, false);
        GSI->Pause(1.5f);
    }
    GSI->Pause(2.0f);
    GSI->HeroStopDigging();
    GSI->FadeScreenOut(0.25f, 0.25f, CRGBColour(0, 0, 0, 255));
    GSI->Pause(0.25f);
    GSI->MakeHeroCarryItemInHand("");
    if (melee) {
        if (IsActiveThreadTerminating())
            return;
        GSI->EntityUnsheatheMeleeWeapon(GSI->GetHero(), false);
    } else if (ranged) {
        if (IsActiveThreadTerminating())
            return;
        GSI->EntityUnsheatheRangedWeapon(GSI->GetHero(), false);
    }
    GSI->EntityResetForceToLookAt(GSI->GetHero());
    GSI->Pause(0.25f);
    GSI->FadeScreenIn();
    GSI->SetCutsceneMode(false, true);
    GSI->CameraDefault();
    if (found) {
        if (IsActiveThreadTerminating())
            return;
        GSI->GiveHeroItemsFromContainer(spot, false);
    }
    GSI->PauseAllEntities(false);
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}