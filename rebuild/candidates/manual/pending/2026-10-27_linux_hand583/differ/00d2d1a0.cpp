// NScript::CQ_EndGameFocalSitesScript::CBattleMinion::Main() -- the end-game battle minion: keep the script alive while Me lives, spawn the parent's FadeCreatureUp thread on Me, put Me in FACTION_MONSTERS facing the hero, wait while the hero stays within 18 units, then fade Me out and kill it
// Grade: DIFFER(556v620) -- the first early return (after SetPredicate / NewScriptFrame) jumps to the shared out-of-line ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 tail; retail gives that exit its own inline destructor (PImp release, CScriptGameResourceObjectBase vtable store, ~CBaseObject 0x0099A430) and epilogue. Everything else matches (frame 0x34, predicate, spawn, loop).
// Leaf: ?Main@CBattleMinion@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2D1A0, 620 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the RTTI-less entity-script vtable 0x012CC304 that the landed
//   NScript::CQ_EndGameFocalSitesScript::CBattleMinion::Alloc 0x00D2C340 stores [dtor 0x00D2C410, Main 0x00D2D1A0, Init 0x00D2C3F0,
//   GetParentScript 0x00D2C3E0, OnPersist 0x00CDEBC0 (base), OnPredicateFail 0x00D2C400, OnInterrupted 0x00CDEBE0 (base)]; the row
//   is referenced only from that slot (0x012CC308). Ego_r.pdb ?Main@CBattleMinion@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ.
//   The spawned thread 0x00D2D410 is ?FadeCreatureUp@CQ_EndGameFocalSitesScript@NScript@@QAEXVCScriptThing@@@Z (its name literal
//   "FadeCreatureUp" is the one built for the spawn). Catalog label Quest009_SpawnMonster_WithFade (invented).
// Callees: rand 0x00BFEB16 (result unused), CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430, ~CScriptGameResourceObjectScripted-
//   ThingBase 0x007E74D0, CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10 /
//   IsActiveThreadTerminating 0x00F35B30, operator new 0x00BFEA1A, operator delete 0x00BFE9BC, CCharString(const char*, long)
//   0x0099EBF0 / ~ 0x0099EAE0, operator+ 0x0099F570, CSpawnedFuncBase ctor 0x00CDD450, CScriptBase::AddSpawnedFunction 0x00CB7E50,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716; Game (+4) slots 7 NewScriptFrame, 70 GetHero,
//   475 EntitySetFacingAngleTowardsThing, 601 EntitySetInFaction; vtables CIsThingAlive 0x012C32F0,
//   CSpawnedFunc1<CQ_EndGameFocalSitesScript, CScriptThing> 0x012CC444, CScriptGameResourceObjectScriptedThingBase 0x0127094C,
//   CScriptGameResourceObjectBase 0x0126008C.
// Literals: "FadeCreatureUp" 0x012CC460, "ParentClass." 0x012C9934, "" 0x0122D70E, "FACTION_MONSTERS" 0x012CB1CC; 18.0f and
//   0.2f immediates.
// Layout: retail NScript headers (CEntityScriptBase {vptr, Game +4, Me +8}); CBattleMinion ParentClass +0x14 (Ego_r.pdb).
//   CBaseObject / CScriptGameResourceObjectBase / CScriptGameResourceObjectScriptedThingBase are local polymorphic models (the
//   chunk 582 model; no retail shared header yet) and CSpawnedFunc1<T, P> the local template of the landed spawning rows.
// Shape: the unused scripted-thing resource's inline destructor at the first exit and the out-of-line 0x007E74D0 at the end
//   come from a plain in-class virtual destructor body; the spawn is the landed CAT_BanditAlly::Init 0x00F05F30 form.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
void operator delete(void* block);                      // 0x00BFE9BC (MSVCR71)
extern "C" int __cdecl rand();                          // 0x00BFEB16 (MSVCR71)
class CCPPointerInfo {
public:
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
// CScriptThing stays local in this row (as in the landed CAT_BanditAlly::Init 0x00F05F30): retail expands the copy inline at
// the by-value CSpawnedFunc1 argument but calls 0x004ABE90 for the CIsThingAlive / CSpawnedFunc1 member copies; the shared
// engine/CScriptThing.h model either inlines every copy (inline body) or none (declared only). The include guard of the
// shared header is pre-defined so the shared predicate / entity headers use this model.
#define FABLE_ENGINE_CSCRIPTTHING_H
#define FABLE_RETAIL_NSCRIPT
#include "engine/fable_nscript_forward.h"
class CScriptThing : public CBaseClass {                // vtable 0x01238C8C, 12 bytes
public:
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other);            // 0x004ABE90 (the out-of-line copy)
    __forceinline CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // 0x004AA840
    CCountedPointer<CScriptThing> PImp;                 // +0x04
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float time, bool a, bool b);                  // 0x00CBE716
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

class CBaseObjectPointer;
class CBaseObject : public CBaseClass {                 // vtable 0x0129A7D8 (RTTI .?AVCBaseObject@@)
protected:
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;             // +0x04
};
class CScriptGameResourceObjectBase : public CBaseObject {   // vtable 0x0126008C
public:
    CScriptGameResourceObjectBase() { }
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    virtual ~CScriptGameResourceObjectScriptedThingBase() { }          // out of line 0x007E74D0
};

template <class TClass, class TPar1> class CSpawnedFunc1 : public CSpawnedFuncBase {
public:
    void (TClass::*Func)(TPar1);                        // +0x34
    TClass* PClass;                                     // +0x38
    TPar1   Par1;                                       // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, TClass* pclass, void (TClass::*func)(TPar1), TPar1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameFocalSitesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void FadeCreatureUp(CScriptThing creature);         // 0x00D2D410
    class CBattleMinion;
};
class CQ_EndGameFocalSitesScript::CBattleMinion : public CEntityScriptBase {
public:
    CQ_EndGameFocalSitesScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;            // +0x18
private:
    virtual void Main();
};

void CQ_EndGameFocalSitesScript::CBattleMinion::Main()
{
    rand();
    CScriptGameResourceObjectScriptedThingBase minion;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc1<CQ_EndGameFocalSitesScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("FadeCreatureUp"), 0, ParentClass,
            &CQ_EndGameFocalSitesScript::FadeCreatureUp, CScriptThing(Me, CScriptThing::InlineCopy())),
        CCharString(""));
    Game->EntitySetInFaction(Me, "FACTION_MONSTERS");
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
    while (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 18.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, 0.2f, true, true);
}
}   // namespace NScript
