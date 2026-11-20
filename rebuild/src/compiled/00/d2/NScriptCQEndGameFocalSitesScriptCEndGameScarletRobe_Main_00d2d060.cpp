// NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe::Main() -- take scripted control of Me (priority 4), then wait each frame until the parent script's JackLeaves flag is set, release the handle and run one more frame
// Leaf: ?Main@CEndGameScarletRobe@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2D060, 308 bytes, plain `ret`; entity vtable 0x012CC428 slot 1 (installed by the Alloc 0x00D2CF80).
// Identity: registration "EndGameScarletRobe" 0x00D2AE6B in CQ_EndGameFocalSitesScript::Main 0x00D2AA40; Ego_r.pdb / FableWin.pdb
//   ?Main@CEndGameScarletRobe@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ (private virtual); Anniversary RTTI agrees.
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 (the scripted-thing handle's implicit ctor / dtor, expanded),
//   IsValid 0x00CD23B9 / Release 0x00CD2770, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; Game slots 7 NewScriptFrame,
//   8 StartScriptingEntity. The flag at ParentClass +0x81 is CQ_EndGameFocalSitesScript::JackLeaves (Ego_r.pdb layout, retail -0x14).
// Ported (NF205, 2026-11-20) onto the shared headers (was a local ScriptResource / GameCalls / NativeParent / CEntityScriptBase model).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_SCARLETROBE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_EndGameFocalSitesScript.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

void NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.JackLeaves) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_me.IsValid())
        seh_me.Release();
    Game->NewScriptFrame();
    IsActiveThreadTerminating();
}
