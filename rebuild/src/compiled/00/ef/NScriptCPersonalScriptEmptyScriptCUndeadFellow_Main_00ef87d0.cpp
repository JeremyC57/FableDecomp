// NScript::CPersonalScript_EmptyScript::CUndeadFellow::Main() -- the empty personal script's "UndeadFellow" entity: run under "Me, Marker4 and Marker5 are alive", spawn the parent script's ChangeAlpha thread on Me, then take Me under scripted control (priority 4) and walk it back and forth between Marker4 and Marker5 every script frame until the thread terminates
// Leaf: ?Main@CUndeadFellow@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF87D0, 1003 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): slot 1 of the entity-script vtable 0x012F64FC, the vtable the landed
//   NScript::CPersonalScript_EmptyScript::CUndeadFellow::Alloc 0x00EF47C0 stores into the 0x1C-byte object it news (the
//   "UndeadFellow" binding of CPersonalScript_EmptyScript::Main); Ego_r.pdb names it
//   ?Main@CUndeadFellow@CPersonalScript_EmptyScript@NScript@@EAEXXZ (private virtual). The thread it spawns is the landed
//   ?ChangeAlpha@CPersonalScript_EmptyScript@NScript@@QAEXVCScriptThing@@@Z 0x00EF8BC0.
// Label: catalog "sub_00ef87d0" (gap scan).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator+(const CCharString&, const CCharString&) 0x0099F570 (__fastcall),
//   CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CBaseClass dtor 0x0099A2E0, operator delete 0x00BFE9BC, operator new
//   0x00BFEA1A (0x48 = sizeof CSpawnedFunc1<CPersonalScript_EmptyScript, CScriptThing>), CSpawnedFuncBase ctor 0x00CDD450,
//   CScriptBase::AddSpawnedFunction 0x00CB7E50 (on ParentClass), CPredicate_And<CIsThingAlive, CIsThingAlive> ctor 0x00E24760,
//   CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive> ctor 0x00EF5BC0, ~CIsThingAlive 0x00E241B0,
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380,
//   CScriptGameResourceObjectScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / MoveToThing 0x007E7300 /
//   implicit dtor 0x007E74D0; CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity,
//   72 GetThingWithScriptName (ghidra_out/cgsi_slots_aligned.tsv).
// Data: vtables CScriptThing 0x01238C8C, CIsThingAlive 0x012C32F0, CPredicateBase 0x012C2FF4, CPredicate_And<IsAlive, IsAlive>
//   0x012E1B70, CPredicate_And<CPredicate_And<..>, IsAlive> 0x012F6628, CSpawnedFunc1<..> 0x012F6888,
//   CScriptGameResourceObjectScriptedThingBase 0x0127094C. Literals "Marker4" 0x012F68B8, "Marker5" 0x012F68B0,
//   "ChangeAlpha" 0x012F68A4, "ParentClass." 0x012C9934, "" 0x0122D70E.
// Layout: entity script (Game +0x04, Me +0x08, ParentClass +0x14, PMasterData +0x18); CSpawnedFunc1 0x48 bytes (Func +0x34,
//   PClass +0x38, Par1 +0x3C); predicates as the landed CLookAtMe1::Main 0x00EF5CB0 (CIsThingAlive 0x10 bytes, CPredicate_And
//   {vptr, First, Second}).
// Shape: the landed family models (L139 UndeadRising Init 0x00E0DAA0, CCorneredGuard::Main 0x00DA7830, CLookAtMe1::Main
//   0x00EF5CB0). Retail calls the CScriptThing copy 0x004ABE90 for the predicate members and the functor's Par1, but
//   expands the copy of Me into the by-value functor argument: modelled as the out-of-line const copy plus an inline
//   non-const copy (Me is a non-const lvalue; the const temporaries and the `const P1` parameter take the const one).
//   The parameter's dtor inside the inlined functor ctor is the out-of-line 0x004AA840 while the two markers' dtors at
//   the first return are inline: VC7.1 instantiates class-template member bodies at the end of the TU (so a pragma around
//   the template is ignored), hence an explicit specialization of the CSpawnedFunc1 ctor defined under
//   `#pragma inline_depth(0)` (same symbol, same body as the primary). The returns after the scripting wait are wrapped
//   in inline_depth(0) so their dtors stay out of line (LookAtMe1). Predicate dtors are user-declared virtual (they reset
//   the vtable), the CPredicate_And ctors out of line (retail calls them).
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void operator delete(void* block);                      // 0x00BFE9BC
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThingImp;
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline CScriptThing(CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }   // inline copy of a non-const thing
    virtual ~CScriptThing() { }                         // out-of-line copy 0x004AA840
};
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptAIPriority { };
enum EScriptEntityMoveType { };
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CPredicateBase {                                  // vtable 0x012C2FF4
public:
    virtual ~CPredicateBase() { }
    virtual bool IsTrue() const = 0;
};
class CIsThingAlive : public CPredicateBase {           // vtable 0x012C32F0, 16 bytes
public:
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive();                           // 0x00E241B0
    virtual bool IsTrue() const;
};
template <class A, class B> class CPredicate_And : public CPredicateBase {   // vtables 0x012E1B70 / 0x012F6628
public:
    A First;                                            // +0x04
    B Second;
    CPredicate_And(const A& first, const B& second);   // 0x00E24760 / 0x00EF5BC0
    virtual ~CPredicate_And() { }
    virtual bool IsTrue() const { return First.IsTrue() && Second.IsTrue(); }
};
template <class A, class B> inline CPredicate_And<A, B> operator&&(const A& first, const B& second)
{
    return CPredicate_And<A, B>(first, second);
}
}   // namespace NScript
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
class CEntityScriptBase {                               // retail 0x14 bytes
public:
    virtual ~CEntityScriptBase();
    virtual void Main();                                // slot 1
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
namespace NScript {
class CPersonalScript_EmptyScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void* PMasterData;                                  // +0x44
    void ChangeAlpha(CScriptThing thing);               // 0x00EF8BC0
    class CUndeadFellow : public CEntityScriptBase {    // 0x1C bytes, vtable 0x012F64FC
    public:
        CPersonalScript_EmptyScript* ParentClass;       // +0x14
        void*                        PMasterData;       // +0x18
    private:
        virtual void Main();                            // slot 1
    };
};
}   // namespace NScript
#pragma inline_depth(0)
template <> __forceinline CSpawnedFunc1<NScript::CPersonalScript_EmptyScript, CScriptThing>::CSpawnedFunc1(
    const CCharString& name, unsigned long id, NScript::CPersonalScript_EmptyScript* pclass,
    void (NScript::CPersonalScript_EmptyScript::*func)(CScriptThing), const CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1)
{
}
#pragma inline_depth()
namespace NScript {

void CPersonalScript_EmptyScript::CUndeadFellow::Main()
{
    CScriptThing marker4 = Game->GetThingWithScriptName("Marker4");
    CScriptThing marker5 = Game->GetThingWithScriptName("Marker5");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(marker4) && CIsThingAlive(marker5));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CPersonalScript_EmptyScript, CScriptThing>(
                                        CCharString("ParentClass.") + CCharString("ChangeAlpha"), 0, ParentClass,
                                        &CPersonalScript_EmptyScript::ChangeAlpha, Me),
                                    CCharString(""));
    CScriptGameResourceObjectScriptedThingBase object;
    if (object.IsValid())
        object.Release();
    while (!Game->StartScriptingEntity(Me, object, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    }
    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
    for (;;) {
        if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        object.MoveToThing(marker4, 0.0f, (EScriptEntityMoveType)0, true, true, false, true);
        object.MoveToThing(marker5, 0.0f, (EScriptEntityMoveType)0, true, true, false, true);
        Game->NewScriptFrame();
    }
}
}   // namespace NScript