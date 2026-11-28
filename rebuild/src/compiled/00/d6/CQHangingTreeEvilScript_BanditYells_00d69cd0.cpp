// Leaf: ?BanditYells@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#include "engine/TRetVoid.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_HangingTreeEvilScript.h"

void NScript::CQ_HangingTreeEvilScript::BanditYells()
{
    TRetVoid _Fail_Val;
    if (false) {} else for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        while (!BanditYelledRecently) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.Pause(5.0f);
        BanditYelledRecently = false;
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}
