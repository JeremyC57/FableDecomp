// NScript::CPersonalScript_EmptyScript::CUndeadFellow::Main() -- the empty personal script's "UndeadFellow" entity: run under "Me, Marker4 and Marker5 are alive", spawn the parent script's ChangeAlpha thread on Me, then take Me under scripted control (priority 4) and walk it back and forth between Marker4 and Marker5 every script frame until the thread terminates
// Grade: DIFFER(1047v1003) -- PILOT (landed RELOCATION_MATCH with #pragma inline_depth + a non-const CScriptThing copy overload): pragma-free, every exit already matches retail (first return inline, the rest 0x004AA840); the residue is the spawned functor's by-value CScriptThing argument: retail copies Me inline and calls ~CScriptThing 0x004AA840 for it inside the inlined CSpawnedFunc1 ctor, VC7.1 calls the copy 0x004ABE90 and expands the dtor (an inline copy ctor instead expands every copy: 1100 bytes)
// Leaf: ?Main@CUndeadFellow@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Pilot result: 13 probes (inline / out-of-line copy ctor, plain-inline functor ctor specialisation, /Ob1, QFE-4035, loop forms, named functor pointer, argument temporaries, late copy-ctor definition); 128 residue lines remain, all in the functor block. Keep the landed source.
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
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // : CBaseClass, vtable 0x01238C8C
inline CScriptThing::~CScriptThing() { }               // out-of-line copy 0x004AA840
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
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"                       // vtable per instantiation; ctor inline
#include "engine/CScriptBase.h"
#include "engine/CEntityScriptBase.h"
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
        virtual CScriptBase* GetParentScript() const;   // slot 3
    };
};

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
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        object.MoveToThing(marker4, 0.0f, (EScriptEntityMoveType)0, true, true, false, true);
        object.MoveToThing(marker5, 0.0f, (EScriptEntityMoveType)0, true, true, false, true);
        Game->NewScriptFrame();
    }
}
}   // namespace NScript
