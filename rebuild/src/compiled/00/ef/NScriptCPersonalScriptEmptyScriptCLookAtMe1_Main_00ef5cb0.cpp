// NScript::CPersonalScript_EmptyScript::CLookAtMe1::Main() -- under "Me and WalkToHere are alive": fetch the
//   TriggerCrate, then repeatedly wait until the hero uses Me and make the hero wear OBJECT_HERO_NO_HAT
// Leaf: ?Main@CLookAtMe1@CPersonalScript_EmptyScript@NScript
// Retail: Fable.exe @ 0x00EF5CB0, 817 bytes (+ int3 padding), plain `ret`.
// Callees: CPredicate_And<CIsThingAlive, CIsThingAlive> ctor 0x00E24760, ~CIsThingAlive 0x00E241B0, CScriptThing copy
//   0x004ABE90, out-of-line ~CScriptThing 0x004AA840, SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBaseClass dtor 0x0099A2E0, CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7 NewScriptFrame, 72
//   GetThingWithScriptName, 190 SetHeroAsWearing; CScriptThing slot 20 MsgIsUsedBy.
//   Strings "WalkToHere" 0x012F6654, "TriggerCrate" 0x012F5EB0, "SCRIPT_NAME_HERO" 0x0125D1C8, "OBJECT_HERO_NO_HAT" 0x01250A68.
// Layout: the landed 00d9f270 CMC_Minion model (CEntityScriptBase {vptr, Game +4, Me +8}); predicate classes from the
//   landed 00e86e60 FisticuffsChallenge (CIsThingAlive vtable 0x012C32F0, CPredicate_And 0x012E1B70 over base 0x012C2FF4).
// Shape: __forceinline ~CScriptThing; `for (;;) { if (Term()) return; ... }` outer loop (a `while` swaps the two inline
//   copies' registers); the out-of-line exits (0x004AA840) are the two returns after the wait, under
//   in-body inline_depth(0).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
__forceinline CScriptThing::~CScriptThing() {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CPredicateBase { public:                                 // vtable 0x012C2FF4
    virtual bool IsTrue() = 0;
    ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase { public:                 // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive();                                   // 0x00E241B0
    virtual bool IsTrue();
};
template <class A, class B> class CPredicate_And : public CPredicateBase { public:   // vtable 0x012E1B70 for <CIsThingAlive, CIsThingAlive>
    A First;                                            // +0x04
    B Second;                                           // +0x14
    CPredicate_And(const A& first, const B& second);    // 0x00E24760
    ~CPredicate_And() { }
    virtual bool IsTrue();
};
template <class A, class B> inline CPredicate_And<A, B> operator&&(const A& first, const B& second)
{
    return CPredicate_And<A, B>(first, second);
}
class CPersonalScript_EmptyScript { public:
struct CLookAtMe1 : CEntityScriptBase {
    void Main();
};
};

void CPersonalScript_EmptyScript::CLookAtMe1::Main()
{
    CScriptThing walkTo = Game->GetThingWithScriptName("WalkToHere");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(walkTo));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing crate = Game->GetThingWithScriptName("TriggerCrate");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->SetHeroAsWearing("OBJECT_HERO_NO_HAT");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}

}

