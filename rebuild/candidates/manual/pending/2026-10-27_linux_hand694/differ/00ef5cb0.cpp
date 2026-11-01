// NScript::CPersonalScript_EmptyScript::CLookAtMe1::Main() -- the empty personal script's "LookAtMe1": under "Me and WalkToHere are alive" fetch the TriggerCrate, then repeatedly wait until the hero uses Me and make the hero wear OBJECT_HERO_NO_HAT
// Grade: DIFFER(706v817) -- PILOT (landed RELOCATION_MATCH with #pragma inline_depth): pragma-free, VC7.1 expands the plain inline ~CScriptThing at the first return and at the inner wait-loop return only; retail also expands it at the outer-loop exit (0x00EF5EDF, crate + WalkToHere) and calls 0x004AA840 only at the two returns after the wait (0x00EF5FC8)
// Leaf: ?Main@CLookAtMe1@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Pilot result: 13 probes (loop forms, predicate dtor shapes, /Ob1, QFE-4035, scope blocks, direct CPredicate_And, __forceinline dtor = 899 bytes everywhere); no pragma-free shape reproduces retail's three inline exits. Keep the landed source.
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
inline CScriptThing::~CScriptThing() { }               // plain inline: VC7.1 decides per exit (0x004AA840 elsewhere)
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptAIPriority { };
enum EScriptEntityMoveType { };
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
template <class A, class B> class CPredicate_And : public CPredicateBase {   // vtable 0x012E1B70
public:
    A First;                                            // +0x04
    B Second;                                           // +0x14
    CPredicate_And(const A& first, const B& second);   // 0x00E24760
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CPredicate_And() { }
};
template <class A, class B> inline CPredicate_And<A, B> operator&&(const A& first, const B& second)
{
    return CPredicate_And<A, B>(first, second);
}
}   // namespace NScript
#include "engine/CScriptBase.h"
#include "engine/CEntityScriptBase.h"                   // Game +0x04, Me +0x08; SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30
namespace NScript {
class CPersonalScript_EmptyScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void* PMasterData;                                  // +0x44
    class CLookAtMe1 : public CEntityScriptBase {
    public:
        CPersonalScript_EmptyScript* ParentClass;       // +0x14
        void*                        PMasterData;       // +0x18
    private:
        virtual void Main();                            // slot 1
        virtual CScriptBase* GetParentScript() const;   // slot 3
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
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetHeroAsWearing("OBJECT_HERO_NO_HAT");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}
}   // namespace NScript
