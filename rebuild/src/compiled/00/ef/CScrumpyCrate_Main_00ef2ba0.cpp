// CScrumpyCrate::Main() -- the scrumpy crate: register an alive-filter over Me, then every frame watch who carries it: while the hero carries it the crate is invisible, a screen message says so and the "Gate" is open, and when he puts it down the gate closes and the crate fades back in; when someone else carries it the crate is dimmed to 0.2, then restored to 0.6 when it is put down
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF2BA0, length 882 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over the CScriptThing copy ctor 0x004ABE90 and its out-of-line
// dtor 0x00E241B0, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 /
// 0x0099EAE0; ~CScriptThing 0x004AA840; CScriptThing slot 63 IsBeingCarriedBy; CGameScriptInterface slots 7
// NewScriptFrame, 72 GetThingWithScriptName, 112 AddScreenMessage, 425 OpenDoor, 426 CloseDoor, 510 EntitySetAlpha.
// Literals "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E, "Gate" 0x012F6324, and the four debug screen messages
// 0x012F632C / 0x012F62F4 / 0x012F62B8 / 0x012F628C.
// Layout: Ego_r.pdb CScrumpyCrate (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18 (neither used here).
// Shape: the carry waits are `while (!(!x))` (retail's `test al,al ; setne`), the first carry tests plain `if (x)`.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11();
    virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16(); virtual void t17();
    virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23();
    virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29();
    virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34(); virtual void t35();
    virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39(); virtual void t40(); virtual void t41();
    virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46(); virtual void t47();
    virtual void t48(); virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53();
    virtual void t54(); virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59();
    virtual void t60(); virtual void t61(); virtual void t62();
    virtual bool IsBeingCarriedBy(const CCharString& name) const;   // slot 63 (+0xfc) 0x004AAFE0
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
    virtual ~CIsThingAlive();                                  // 0x00E241B0
};
}
using NScript::CIsThingAlive;
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_0 };
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
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CScrumpyCrate : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CPersonalScript_EmptyScript*
    void* PMasterData;                                 // +0x18 CEmptyData*
    void Main();
};

void CScrumpyCrate::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        if (Me.IsBeingCarriedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetAlpha(Me, 0.0f, true);
            Game->AddScreenMessage("Hero's carrying the scrumpy crate!", TEXT_GROUP_SELECTION_0);
            CScriptThing gate = Game->GetThingWithScriptName("Gate");
            Game->OpenDoor(gate);
            while (!(!Me.IsBeingCarriedBy("SCRIPT_NAME_HERO"))) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->AddScreenMessage("Hero's no longer carrying the scrumpy crate!", TEXT_GROUP_SELECTION_0);
            Game->CloseDoor(gate);
            Game->EntitySetAlpha(Me, 1.0f, true);
        } else if (Me.IsBeingCarriedBy("")) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetAlpha(Me, 0.2f, true);
            Game->AddScreenMessage("Someone other than the hero is carrying the scrumpy crate!", TEXT_GROUP_SELECTION_0);
            while (!(!Me.IsBeingCarriedBy(""))) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetAlpha(Me, 0.6f, true);
            Game->AddScreenMessage("No one's carrying the scrumpy crate now!", TEXT_GROUP_SELECTION_0);
        }
        Game->NewScriptFrame();
    }
}