// NScript::CQ_EndGameFocalSitesScript::CPermanentBarrier::Main() -- the permanent magic barrier entity script: register an alive-filter over the barrier (Me), let the game update, and unless the script is already done put the HK_MAGIC_BARRIER effect on the barrier's position and keep updating until it is done
// Leaf: Main
// Identity (naming wave 11): ?Main@CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012CC48C slot 1 is stored by the ctor 0x00D2D7B0 of the factory 0x00D2D730, registered as "PermanentBarrier" (site 0x00D2AF3B)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2D830, 202 bytes (+ int3 padding), `ret` (vtable slot 1 = Main per the entity-script
//   contract [dtor, Main, Init, ...]).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (the filter's member; filter vtable 0x012C32F0
//   stored first), the filter registration 0x00F35B10 and IsDone 0x00F35B30 (both on `this`), CScriptThing::~CScriptThing
//   0x004AA840 (the filter's inline destructor re-stores its vtable first; also the effect handle at the end),
//   CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, Me's virtual GetPos (slot 6,
//   +0x18), the game interface's Update (slot 7, +0x1C) and CreateEffect(const CCharString&, const C3DVector&,
//   const CCharString&, float, bool, bool) (slot 100, +0x190, returns CScriptThing).
// Shape: the landed CHTE_Prisoner::Main 0x00D69F50 with the effect call between the first update and the wait loop; the
//   effect handle is a function-scope local after an early `return` (inside an `if` block it would share its frame slot
//   with the filter; retail keeps them apart in a 0x24-byte frame). The
//   two string literals are implicit `const char*` -> CCharString conversions, so both temporaries are built before any
//   argument is pushed, the rightmost ("" bone name) first; the three zero pushes (false, false, 0.0f angle) and the ""
//   address then precede Me.GetPos() -- VC evaluates arguments right to left -- and the returned effect handle is a
//   named local that lives across the wait loop (destroyed after it). The two string temporaries die right after the
//   call. Layout as the CEntityScriptBase family: Game +4, CScriptThing Me +8 (12 bytes).
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_EndGameFocalSitesScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CPermanentBarrier;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_EndGameFocalSitesScript;
namespace NScript {
class CQ_EndGameFocalSitesScript::CPermanentBarrier : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_EndGameFocalSitesScript::CPermanentBarrier::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing effect = Game->CreateEffect("HK_MAGIC_BARRIER", Me.GetPos(), "", 0.0f, false, false);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}