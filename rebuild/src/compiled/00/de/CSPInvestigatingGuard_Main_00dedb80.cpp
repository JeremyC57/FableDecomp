// CSP_InvestigatingGuard::Main() -- the secret-passage investigating guard: register an alive-filter over Me, take Me under scripted control, then until the hero is seen or heard keep walking to the hero (now and then calling out the investigating-noise line), wait until the hero is seen, and set the hero as its best enemy target
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEDB80, length 697 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over CScriptThing copy ctor 0x004ABE90 / ~CScriptThing 0x004AA840,
// RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30; scripted-thing handle (vtable 0x0127094C) inline
// ctor over CBaseObject 0x0099A380, IsValid 0x00CD23B9, Release 0x00CD2770, IsPerformingScriptTask 0x007E7450,
// MoveToPosition 0x007E72F0, dtor 0x007E74D0; rand 0x00BFEB16; CCharString 0x0099EBF0 / 0x0099EAE0; CScriptThing slot 6
// GetPosition; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 364 AddNewConversation,
// 366 AddLineToConversation, 451 GiveThingBestEnemyTarget, 632 CanThingBe_Seen_ByOtherThing, 635
// CanThingBe_Heard_ByOtherThing. Literal "TEXT_QST_055_GUARD_INVESTIGATING_NOISE" 0x012DDE00.
// Layout: Ego_r.pdb CSP_InvestigatingGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18 (neither used here).
// Shape: the landed CHTE_Audience::Main 0x00D6B240 handle (inline ctor, out-of-line dtor at every exit) and the
// CRockTrollTrigger::Main 0x00E0AD40 scoped filter.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual const C3DVector& GetPosition() const;      // slot 6 (+0x18)
    void* PImp[2];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CCountedPointerWords { void* Data; void* Info; };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
enum EScriptEntityMoveType;
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    virtual void MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool);   // 0x007E72F0
    virtual bool IsPerformingScriptTask() const;               // 0x007E7450
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern "C" int __cdecl rand();                         // 0x00BFEB16
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CEntityScriptBase { public:
    virtual void e00();
    class CGameScriptInterface* Game;                 // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CSP_InvestigatingGuard : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CQ_SecretPassageScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CSP_InvestigatingGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) && !Game->CanThingBe_Heard_ByOtherThing(Game->GetHero(), Me)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!handle.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            handle.MoveToPosition(Game->GetHero().GetPosition(), 5.0f, (EScriptEntityMoveType)1, false, true);
        }
        if (rand() % 200 == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_055_GUARD_INVESTIGATING_NOISE", false, Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (handle.IsValid())
        handle.Release();
}