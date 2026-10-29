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
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing() {}                          // 0x004AA840 where it stays out of line
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
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
class CGameScriptInterfaceBase { public:                // slots: ghidra_out/cgsi_slots_aligned.tsv (CGameScriptInterface vtable 0x01260F0C)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
};
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};

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