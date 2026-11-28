// 00ef5840
// Leaf: ?Main@CBanditWalker@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Flags: none
// Inline-budget: The nested predicate cleanup has a 33-byte ordinary destructor in retail. The selected scope restores that body; two base cleanup calls still differ in Main.
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_C3DVECTOR
#include "engine/TRetVoid.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CPersonalScript_EmptyScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"

#ifdef _MSC_VER
#pragma optimize("gt",on)
#endif

#ifdef _MSC_VER
#pragma inline_depth()
#endif
NScript::CIsThingAlive::~CIsThingAlive() {}

#ifdef _MSC_VER
#pragma inline_depth()
#endif

#ifdef _MSC_VER
#pragma optimize("",on)
#endif
inline CScriptThing::~CScriptThing() {}
namespace NScript {
inline CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}

#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> inline CPredicate_And<CIsThingAlive,CIsThingAlive>::~CPredicate_And() {
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
}
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> inline CPredicate_And< CPredicate_And<CIsThingAlive,CIsThingAlive>, CIsThingAlive>::~CPredicate_And() {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void CPersonalScript_EmptyScript::CBanditWalker::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing marker1 = Game.GetThingWithScriptName("BanditRoute1");
    CScriptThing marker2 = Game.GetThingWithScriptName("BanditRoute2");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(marker1) && CIsThingAlive(marker2));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    seh_me.MoveToThing(marker1, 0.0f, ENTITY_MOVE_RUN, true, true, false, true);
    while (!IsActiveThreadTerminating()) {
        seh_me.FollowPreCalculatedRoute(marker1, ENTITY_MOVE_RUN, true, true);
        seh_me.FollowPreCalculatedRoute(marker2, ENTITY_MOVE_RUN, true, true);
        Game.NewScriptFrame();
    }
}
}   // namespace NScript
