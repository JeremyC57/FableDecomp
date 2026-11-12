// NScript::CQ_GuildTrainingScript::CKillBird::Main() -- entity script: is-conscious predicate on Me, run a frame, then retry each frame to start scripting Me (priority 4) through a fresh handle, and idle until the thread terminates
// Leaf: ?Main@CKillBird@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D43190, 226 bytes (+ int3 padding), plain `ret`; entity vtable 0x012CE32C slot 1.
// Identity: registration "KillBird" 0x00D3C493 in CQ_GuildTrainingScript::Main 0x00D3BC60, Alloc 0x00D43040, vtable slot 1;
//   Ego_r.pdb / FableWin.pdb ?Main@CKillBird@CQ_GuildTrainingScript@NScript@@EAEXXZ (was mislabelled CPretendGuard::Main, which is 0x00CFCCA0).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 (the predicate's member; the predicate's inline destructor re-stores its
//   vtable 0x012C2FE8 = NScript::CIsThingConscious), CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating
//   0x00F35B30, CBaseObject() 0x0099A380, the scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0;
//   Game slots 7 NewScriptFrame, 8 StartScriptingEntity.
// Shape: spelled as the landed CPretendGuard::Main 0x00CFCCA0: the terminate check after the StartScriptingEntity wait and the
//   closing do/while share one test (retail enters the final loop by `jmp` to its test, 0x00D4324C).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_KILLBIRD
#include "engine/CCountedPointer.h"
#include "engine/CPredicateBase.h"
#include "engine/CScriptThing.h"
#include "engine/CIsThingConscious.h"
// Retail expands these at this row's site: defined inline after the header.
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"     // dtor 0x007E74D0 out of line; IsValid 0x00CD23B9, Release 0x00CD2770
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_GuildTrainingScript.h"

void NScript::CQ_GuildTrainingScript::CKillBird::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
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