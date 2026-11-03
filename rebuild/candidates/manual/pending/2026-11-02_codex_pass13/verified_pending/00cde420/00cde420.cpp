// NScript::CPersonalScript_GlobalThingsScript::Main() -- original script entry logic.
// Leaf: ?Main@CPersonalScript_GlobalThingsScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CENTITYSCRIPTBINDING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CPersonalScript_GlobalThingsScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"

void NScript::CPersonalScript_GlobalThingsScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("ClickableDoor", this, &CClickableDoor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("TeleportPoint", this, &CTeleportPoint::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("HERO_GUILD_TELEPORT_MARKER", this, &CHERO_GUILD_TELEPORT_MARKER::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("BowerstoneTeleportPointMarker", this, &CBowerstoneTeleportPointMarker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("OF_MainGates", this, &COF_MainGates::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("KG_Gate", this, &CKG_Gate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("DragonGate", this, &CDragonGate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("TestGateSwitch", this, &CTestGateSwitch::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("InvisibleBarrier", this, &CInvisibleBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("BSGateGuard", this, &CBSGateGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("TargetForKey", this, &CTargetForKey::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("EndGameBarrier", this, &CEndGameBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("LadyGreysBed", this, &CLadyGreysBed::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("ArcheryCompetitionSignpost", this, &CArcheryCompetitionSignpost::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("MK_HS2_SOUL", this, &CMK_HS2_SOUL::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("MK_HS3_SOUL", this, &CMK_HS3_SOUL::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_GlobalThingsScript>("MK_HSE_SOUL", this, &CMK_HSE_SOUL::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
}
