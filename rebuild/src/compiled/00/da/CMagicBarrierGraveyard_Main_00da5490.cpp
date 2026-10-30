// CMagicBarrierGraveyard::Main() (0x00DA5490) -- the second graveyard magic barrier: under an alive filter on Me, create the
//   two red force-field effects at Me (oriented by Me's XY angle); then every frame, once neither effect is alive, replace them
//   with the two force-field idle effects
// Leaf: ?Main@CMagicBarrierGraveyard@
// Retail: Fable.exe @ 0x00DA5490, 779 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing 0x004AA840),
//   SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0 ("" 0x0122D70E,
//   "NEW_RED_FORCEFIELD_01" 0x012C7D6C, "NEW_RED_FORCEFIELD_02" 0x012C7D54, "NEW_RED_FORCEFIELD_IDLE_01" 0x012C354C,
//   "NEW_RED_FORCEFIELD_IDLE_02" 0x012C3530), CScriptThing::operator= 0x008AB980, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC; CScriptThing slots 6 GetPos, 10 GetAngleXY, 75 IsAlive; GameCalls slots 7 NewScriptFrame, 100 CreateEffect.
// Layout: entity-script base {vptr, Game +4, Me +8}; CScriptThing / CGameScriptInterface as the landed CMK_HS2_SOUL::Main
//   0x00CE0920.
// Shape: __forceinline ~CScriptThing; the effects are locals bound as `CScriptThing& e = Game->CreateEffect(...)` (IsAlive
//   stays virtual); the in-loop return and the two re-creation temporaries sit under inline_depth(0) (0x004AA840), the
//   function end expands both destructors inline; `for (;;) { if (Term()) return; ... }` (a `while` sends the first
//   early return to the wrong epilogue copy).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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
class C3DVector { public: float X, Y, Z; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class __declspec(novtable) CPredicateBase
{
public:
    virtual void f00();
};
}
#pragma inline_depth(0)
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                    // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#pragma inline_depth()
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CMagicBarrierGraveyard : CEntityScriptBase {
    void* ParentClass;                                    // +0x14
    void* PMasterData;                                    // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CMagicBarrierGraveyard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing& effect1 = Game->CreateEffect("NEW_RED_FORCEFIELD_01", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    CScriptThing& effect2 = Game->CreateEffect("NEW_RED_FORCEFIELD_02", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!effect1.IsAlive() && !effect2.IsAlive()) {
            if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
            effect1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false)
#pragma inline_depth(0)
            ;
#pragma inline_depth()
            effect2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false)
#pragma inline_depth(0)
            ;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}