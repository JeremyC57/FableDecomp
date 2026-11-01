// NScript::CPersonalScript_EmptyScript::CVillager::Main() -- the empty personal script's "Villager": under "Me, Crate and CrateHomeTarget are alive" take Me under scripted control, then whenever the crate is near home and carried by nobody, pick it up, and if Me carries it walk it to CrateHomeTarget, drop it and unsheathe
// Leaf: ?Main@CVillager@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF8D30, 1013 bytes, thiscall, plain `ret` (ends 0x00EF9124).
// Identity: slot 1 of the entity-script vtable 0x012F6518, stored by the landed CVillager::Alloc 0x00EF48A0 (slot 3 = the landed
//   CVillager::Init 0x00EF49A0, which fills Crate); Ego_r.pdb ?Main@CVillager@CPersonalScript_EmptyScript@NScript@@EAEXXZ
//   (private virtual, the sibling spelling of the landed CUndeadFellow::Main 0x00EF87D0).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CScriptThing copy 0x004ABE90 / ~ 0x004AA840, ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, CPredicate_And ctors 0x00E24760 / 0x00EF5BC0, ~CIsThingAlive 0x00E241B0, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380, CScriptGameResourceObjectScriptedThingBase IsValid
//   0x00CD23B9 / Release 0x00CD2770 / MoveToThing 0x007E7300 / MoveToAndPickUpGenericBox 0x007E7420 / DropGenericBox 0x007E7430 /
//   UnsheatheWeapons 0x007E7440 / IsPerformingScriptTask 0x007E7450 / implicit dtor 0x007E74D0, IsDistanceBetweenThingsOver
//   0x00CBE3EA; CGameScriptInterfaceBase slots 7, 8 StartScriptingEntity, 72; CScriptThing slots 1 GetName, 63 IsBeingCarriedBy.
//   Literal "CrateHomeTarget" 0x012F68C0, "" 0x0122D70E.
// Shape: the landed CUndeadFellow::Main model over the shared engine/CIsThingAlive.h / CPredicateBase.h / CEntityScriptBase.h
//   headers, without its inline_depth regions: with
//   engine/CScriptThing.h and a plain inline dtor VC7.1 itself expands ~CScriptThing at the first return only and calls
//   0x004AA840 at every later exit. The crate test is an early `continue` (retail materialises `!Over || Carried("")` into bl
//   and jumps to the shared NewScriptFrame); the positive `&&` form inverts the byte and reshuffles registers (85 lines).
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void operator delete(void* block);                      // 0x00BFE9BC
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // : CBaseClass, vtable 0x01238C8C; copy ctor 0x004ABE90 out of line
inline CScriptThing::~CScriptThing() { }               // plain inline: VC7.1 expands it at the first return only (0x004AA840 elsewhere)
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
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
// Predicates: the retail NScript models from the shared headers (CPredicateBase vtable 0x012C2FF4; CIsThingAlive 0x10 bytes,
//   vtable 0x012C32F0, out-of-line ~ 0x00E241B0); retail expands CIsThingAlive's constructor inline (vptr, then the 0x004ABE90 copy).
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
template <class A, class B> class CPredicate_And : public CPredicateBase {   // vtables 0x012E1B70 / 0x012F6628
public:
    A First;                                            // +0x04
    B Second;
    CPredicate_And(const A& first, const B& second);   // 0x00E24760 / 0x00EF5BC0
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CPredicate_And() { }
};
template <class A, class B> inline CPredicate_And<A, B> operator&&(const A& first, const B& second)
{
    return CPredicate_And<A, B>(first, second);
}
}   // namespace NScript
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
#include "engine/CEntityScriptBase.h"                   // Game +0x04, Me +0x08; SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30
namespace NScript {
class CPersonalScript_EmptyScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void* PMasterData;                                  // +0x44
    class CVillager : public CEntityScriptBase {        // 0x28 bytes, vtable 0x012F6518
    public:
        CPersonalScript_EmptyScript* ParentClass;       // +0x14
        void*                        PMasterData;       // +0x18
        CScriptThing                 Crate;             // +0x1c (set by the landed Init 0x00EF49A0)
    private:
        virtual void Main();                            // slot 1
        virtual CScriptBase* GetParentScript() const;   // slot 3
    };
};

void CPersonalScript_EmptyScript::CVillager::Main()
{
    CScriptThing home = Game->GetThingWithScriptName("CrateHomeTarget");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(Crate) && CIsThingAlive(home));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase object;
    if (object.IsValid())
        object.Release();
    while (!Game->StartScriptingEntity(Me, object, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        if (!IsDistanceBetweenThingsOver(Crate, home, 10.0f) || Crate.IsBeingCarriedBy("")) {
            Game->NewScriptFrame();
            continue;
        }
        if (IsActiveThreadTerminating())
            return;
        object.MoveToAndPickUpGenericBox(Crate, (EScriptEntityMoveType)1, false);
        while (object.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (Crate.IsBeingCarriedBy(Me.GetName())) {
            if (IsActiveThreadTerminating())
                return;
            object.MoveToThing(home, 0.0f, (EScriptEntityMoveType)1, true, true, false, true);
            object.DropGenericBox();
            while (object.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            object.UnsheatheWeapons();
            while (object.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}
}   // namespace NScript