// Cwelcome_guy::Main() -- the welcoming villager: take Me under scripted control (priority 4, retried each frame), walk Me up to the hero (within 3 units), then for as long as the script runs wait each frame until the hero is close (squared 2D distance under 3) and turn Me to face the hero
// Leaf: ?Main@Cwelcome_guy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFAB40, 571 bytes (576 with int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle (vtable 0x0127094C) IsValid
//   0x00CD23B9 / Release 0x00CD2770 / MoveToThing 0x007E7300 / out-of-line dtor 0x007E74D0, IsActiveThreadTerminating 0x00F35B30,
//   operator delete 0x00BFE9BC; GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 475
//   EntitySetFacingAngleTowardsThing; CScriptThing slot 6 GetPos. Literal 3.0f at 0x0122DED4.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}; handle hierarchy as the landed CMeleeThunder::Main 0x00D58080 /
//   CDF_ShineLight::Main 0x00E6EA30 (implicit handle dtor over __forceinline CCountedPointer / base dtors).
// Shape: the CMeleeThunder handle model (implicit, plain-inline handle dtor). Both wait-loop bodies end in an explicit `continue`
//   (no "proper" loop, GOTCHAS chunk 274), so VC7.1 expands the dtor at every exit; in-body inline_depth(0) then outlines the
//   two hero-wait returns (retail's shared call block 0x00EFAD6A) while the start-loop return and the outer loop-test exit stay
//   inline and the post-start return calls 0x007E74D0 on its own (before the shrink-wrapped ebx push). The closeness test is an
//   inline squared 2D distance of the two position references (the hero's fetched first, Me's copied nowhere) against 3.0f,
//   negated (`test ah,5 ; jp`); a C2DVector-temporary form copies the hero's position to the frame first.
class C3DVector { public: float X, Y, Z; };
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    VPAD4(ma) VPAD1(mb)                                 // slots 1..5
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct Cwelcome_guy : CEntityScriptBase {
    void Main();
};

__forceinline float GetDistanceSquared2D(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}
void Cwelcome_guy::Main()
{
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    scripted.MoveToThing(Game->GetHero(), 3.0f, SCRIPT_ENTITY_MOVE_TYPE_WALK, false, true, false, true);
    while (!IsActiveThreadTerminating()) {
        while (!(GetDistanceSquared2D(Me.GetPos(), Game->GetHero().GetPos()) < 3.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            continue;
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
        Game->NewScriptFrame();
        continue;
    }
}