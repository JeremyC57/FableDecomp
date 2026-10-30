// CPermanentBarrier::Main() -- the permanent magic barrier entity script: register an alive-filter over the barrier (Me), let the game update, and unless the script is already done put the HK_MAGIC_BARRIER effect on the barrier's position and keep updating until it is done
// Leaf: Main
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

class CScriptThing {                                   // 12 bytes, polymorphic
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

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

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CPermanentBarrier : public CEntityScriptBase {
public:
    void*                 __vftable;                   // +0x00
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void Main();
};

void CPermanentBarrier::Main()
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