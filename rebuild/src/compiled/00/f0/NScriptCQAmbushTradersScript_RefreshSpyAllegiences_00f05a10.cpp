// Leaf: ?RefreshSpyAllegiences@CQ_AmbushTradersScript@NScript@@QAEXXZ
// Flags: /O2 /Oy /W3
// FableWin opens each of the four loops with the project's VC6 for-scope guard.
#define FABLE_RETAIL_AMBUSH_TRADERS_SPY_INLINES_PASS30
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYSPY
#include "engine/TRetVoid.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_AmbushTradersScript.h"

void NScript::CQ_AmbushTradersScript::RefreshSpyAllegiences()
{
    TRetVoid _Fail_Val;
    CScriptThing spy = Game.GetThingWithScriptName("AT_ConvoySpy");
    while (BanditsVector.size() != 0)
        BanditsVector.pop_back();
    while (ConvoyMembersVector.size() != 0)
        ConvoyMembersVector.pop_back();
    Game.GetAllThingsWithScriptName("AT_BanditAlly", BanditsVector);
    Game.GetAllThingsWithScriptName("AT_ConvoyMember", ConvoyMembersVector);
    Game.GetAllThingsWithScriptName("AT_Sentry", SentriesVector);
    if (spy.IsAlive()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (false) {} else for (long idx = 0; idx < BanditsVector.size(); idx++) {
            if (BanditsVector[idx].IsAlive()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.EntitySetThingAsAllyOfThing(spy, BanditsVector[idx]);
                Game.EntitySetThingAsAllyOfThing(BanditsVector[idx], spy);
            }
        }
        if (SpyContactMade) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (false) {} else for (long idx = 0; idx < ConvoyMembersVector.size(); idx++) {
                if (ConvoyMembersVector[idx].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.MiniMapRemoveMarker(ConvoyMembersVector[idx]);
                    Game.EntityUnsetThingAsAllyOfThing(spy, ConvoyMembersVector[idx]);
                    Game.EntityUnsetThingAsAllyOfThing(ConvoyMembersVector[idx], spy);
                    Game.EntitySetThingAsEnemyOfThing(spy, ConvoyMembersVector[idx]);
                    Game.EntitySetThingAsEnemyOfThing(ConvoyMembersVector[idx], spy);
                    Game.EntitySetThingAsEnemyOfThing(Game.GetHero(), ConvoyMembersVector[idx]);
                    Game.EntitySetThingAsEnemyOfThing(ConvoyMembersVector[idx], Game.GetHero());
                }
            }
            if (false) {} else for (long idx = 0; idx < SentriesVector.size(); idx++) {
                if (SentriesVector[idx].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.EntityUnsetThingAsAllyOfThing(spy, SentriesVector[idx]);
                    Game.EntityUnsetThingAsAllyOfThing(SentriesVector[idx], spy);
                    Game.EntitySetThingAsEnemyOfThing(spy, SentriesVector[idx]);
                    Game.EntitySetThingAsEnemyOfThing(SentriesVector[idx], spy);
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (false) {} else for (long idx = 0; idx < ConvoyMembersVector.size(); idx++) {
                if (ConvoyMembersVector[idx].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game.EntitySetThingAsAllyOfThing(spy, ConvoyMembersVector[idx]);
                    Game.EntitySetThingAsAllyOfThing(ConvoyMembersVector[idx], spy);
                }
            }
        }
    }
}