// 00f04000 NScript::CQ_AmbushTradersScript::CAT_Phase1Archer::Main
// Leaf: ?Main@CAT_Phase1Archer@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Flags: none
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"

inline bool CScriptThing::IsAlive() const {
    return !PImp.IsNull() && PImp->IsAlive();
}
inline const C3DVector& CScriptThing::GetPos() const {
    return PImp.IsNull() ? NullCoord : PImp->GetPos();
}

namespace NScript {
void CQ_AmbushTradersScript::CAT_Phase1Archer::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (false) {
    } else for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (ParentClass.QuestFailed || ParentClass.QuestFinished) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game.StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptThing Exit = Game.GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (Exit.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (sehMe.IsValid()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    sehMe.MoveToPosition(Exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, Exit, 2.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (true) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}
}