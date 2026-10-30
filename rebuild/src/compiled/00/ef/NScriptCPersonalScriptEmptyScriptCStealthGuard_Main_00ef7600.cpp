// NScript::CPersonalScript_EmptyScript::CStealthGuard::Main() -- the stealth-course guard: find the GameRestart and ReachedGoalRestart markers, guard the thread with "the guard and both markers are alive", run one script frame and, unless the thread is terminating, read the guard's position (the value is unused)
// Leaf: ?Main@CStealthGuard@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF7600, 500 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): the row is referenced once, from slot 1 (Main) of the RTTI-less entity vtable 0x012F6238 (0x012F623C),
//   which the landed NScript::CPersonalScript_EmptyScript::CStealthGuard::Alloc 0x00EF2820 stores (site 0x00EF2856); Ego_r.pdb
//   gives CStealthGuard exactly Alloc, Init, GetParentScript and ?Main@CStealthGuard@CPersonalScript_EmptyScript@NScript@@EAEXXZ
//   (private virtual); the vtable follows the "TestTheInventory" literal. Catalog `sub_00ef7600`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CScriptThing copy 0x004ABE90, ~CBaseClass
//   0x0099A2E0, operator delete 0x00BFE9BC, CPredicate_And<CIsThingAlive, CIsThingAlive>(const CIsThingAlive&, const
//   CIsThingAlive&) 0x00E24760, CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>(...) 0x00EF5BC0,
//   ~CIsThingAlive 0x00E241B0, CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30; CScriptThing
//   slot 6 GetPos; CGameScriptInterface slots 7 NewScriptFrame, 72 GetThingWithScriptName (ghidra_out/cgsi_slots_aligned.tsv).
// Data: "GameRestart" 0x012F680C, "ReachedGoalRestart" 0x012F67F8 (paired with retail by relocation, rel.py); vtables
//   CIsThingAlive 0x012C32F0, CPredicate_And<CIsThingAlive, CIsThingAlive> 0x012E1B70, CPredicate_And<CPredicate_And<...>,
//   CIsThingAlive> 0x012F6628, CPredicateBase 0x012C2FF4, CScriptThing 0x01238C8C.
// Layout: Ego_r.pdb CStealthGuard (28 bytes = the landed Alloc's new size): CEntityScriptBase {vptr, Game +4, Me +8},
//   ParentClass +0x14, PMasterData +0x18.
// Shape: the landed CCrate1::Main 0x00EF54A0 model. `(Alive(Me) && Alive(a)) && Alive(b)` builds both CPredicate_And levels
//   through the out-of-line ctors (operands right to left) and runs both inline dtors; `Me.GetPos();` is a discarded virtual
//   call (`mov edx,[edi] ; call [edx+18h]`); the two markers' inline CScriptThing dtors run at the one shared exit. First grade.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CPredicateBase {                                  // vtable 0x012C2FF4
public:
    virtual bool operator()() const = 0;                // slot 0
    virtual CPredicateBase* Clone() const = 0;          // slot 1
    virtual ~CPredicateBase() { }                       // slot 2
};
class CIsThingAlive : public CPredicateBase {           // 0x10 bytes, vtable 0x012C32F0
public:
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CIsThingAlive();                           // 0x00E241B0
    CScriptThing Thing;                                 // +0x04
};
template <class P1, class P2> class CPredicate_And : public CPredicateBase {   // vtable 0x012E1B70 for <CIsThingAlive, CIsThingAlive>
public:
    CPredicate_And(const P1& p1, const P2& p2);         // 0x00E24760
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CPredicate_And() { }
    P1 Predicate1;                                      // +0x04
    P2 Predicate2;                                      // +0x14
};
template <class P1, class P2> inline CPredicate_And<P1, P2> operator&&(const P1& p1, const P2& p2)
{
    return CPredicate_And<P1, P2>(p1, p2);
}
}
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CEmptyData;
class CPersonalScript_EmptyScript {
public:
    class CStealthGuard;
};
class CPersonalScript_EmptyScript::CStealthGuard : public CEntityScriptBase {
public:
    CPersonalScript_EmptyScript* ParentClass;           // +0x14
    CEmptyData*                  PMasterData;           // +0x18
private:
    virtual void Main();
};

void CPersonalScript_EmptyScript::CStealthGuard::Main()
{
    CScriptThing gameRestart = Game->GetThingWithScriptName("GameRestart");
    CScriptThing goalRestart = Game->GetThingWithScriptName("ReachedGoalRestart");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(gameRestart) && CIsThingAlive(goalRestart));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Me.GetPos();
}
}   // namespace NScript