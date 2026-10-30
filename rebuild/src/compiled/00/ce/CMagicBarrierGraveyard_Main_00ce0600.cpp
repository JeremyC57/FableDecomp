// CMagicBarrierGraveyard::Main() (0x00CE0600) -- graveyard magic barrier: under an alive filter on Me, create the two red
//   force-field idle effects at Me (oriented by Me's XY angle); then every frame, once both effects are dead, re-create them
// Leaf: ?Main@CMagicBarrierGraveyard@
// Retail: Fable.exe @ 0x00CE0600, 795 bytes (+ int3 padding), plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing 0x004AA840),
//   SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0 ("" 0x0122D70E,
//   "NEW_RED_FORCEFIELD_IDLE_01" 0x012C354C, "NEW_RED_FORCEFIELD_IDLE_02" 0x012C3530), CCountedPointer::operator= 0x00704580,
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptThing slots 6 GetPos, 10 GetAngleXY, 76 IsDead; GameCalls
//   slots 7 NewScriptFrame, 100 CreateEffect.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, two effect CScriptThings at
//   +0x1c / +0x28. CScriptThing / CCountedPointer / CGameScriptInterface as the landed CMK_HS2_SOUL::Main 0x00CE0920.
// Shape: __forceinline ~CScriptThing; the first two creations assign the WHOLE thing (`Effect1 = ...`, implicit inline
//   operator=; `.PImp =` there lets VC cache &Effect1.PImp in ebx instead of the vptr in ebp); the loop ones assign `.PImp` (inline counted operator=
//   forwarding); the two loop re-creations put inline_depth(0) just before the `;` so their temporaries' destructors call
//   0x004AA840, while the first two expand inline.
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
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580
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
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
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
    void*        ParentClass;                             // +0x14
    void*        PMasterData;                             // +0x18
    CScriptThing Effect1;                                 // +0x1c
    CScriptThing Effect2;                                 // +0x28
    void Main();
};

void CMagicBarrierGraveyard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Effect1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    Effect2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    while (!IsActiveThreadTerminating()) {
        if (Effect1.IsDead() && Effect2.IsDead()) {
            if (IsActiveThreadTerminating())
                return;
            Effect1.PImp = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", Me.GetAngleXY(), false, false).PImp
#pragma inline_depth(0)
            ;
#pragma inline_depth()
            Effect2.PImp = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", Me.GetAngleXY(), false, false).PImp
#pragma inline_depth(0)
            ;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}