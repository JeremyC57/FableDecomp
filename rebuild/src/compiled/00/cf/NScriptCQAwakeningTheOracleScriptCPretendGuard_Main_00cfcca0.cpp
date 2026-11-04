// NScript::CQ_AwakeningTheOracleScript::CPretendGuard::Main() -- entity script: alive predicate on Me, remove Me once the opening necromancer cutscene has played, then script Me (priority 4, retried each frame) and run frames until the thread terminates
// Leaf: ?Main@CPretendGuard@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFCCA0, 274 bytes (+ int3 padding), plain `ret`; entity-script vtable 0x012C7CB4 slot 1
//   (Main; Ego_r.pdb / FableWin.pdb ?Main@CPretendGuard@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ, private virtual).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 (the predicate's member; the predicate's inline destructor re-stores
//   its vtable 0x012C32F0), CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject()
//   0x0099A380, the scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 (vtable 0x0127094C);
//   GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity, 108 RemoveThing.
// Layout: Ego_r.pdb CPretendGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_AwakeningTheOracleScript: PlayedOpeningNecroCS PDB +0x79 = retail +0x65), PMasterData +0x18.
// Shape: every wait ends with the script macro's `if (IsActiveThreadTerminating()) return;`; the one after the StartScriptingEntity
//   wait and the closing do/while share one test (VC7.1 cross-jumps them: retail enters the final loop by `jmp` to its test).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CPredicateBase.h"
#include "engine/CScriptThing.h"
namespace NScript {
class CIsThingAlive : public CPredicateBase { public:                 // vtable 0x012C32F0 (16 bytes, Thing +4)
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive() { }
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
};
}
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"     // dtor 0x007E74D0 out of line; IsValid 0x00CD23B9, Release 0x00CD2770
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_AwakeningTheOracleScript {        // the owner script; only the flag this entity reads is modelled
public:
    unsigned char _pad[0x65];
    bool PlayedOpeningNecroCS;             // +0x65 (PDB +0x79)
    class CPretendGuard;
};
class CQ_AwakeningTheOracleScript::CPretendGuard : public CEntityScriptBase {
public:
    CQ_AwakeningTheOracleScript* ParentClass;          // +0x14
    void*                        PMasterData;          // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_AwakeningTheOracleScript::CPretendGuard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->PlayedOpeningNecroCS) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
    }
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}