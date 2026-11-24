// 00d90290 NScript::CQ_HobbeCaveScript::CBanditCommentMarker::Main
// Leaf: ?Main@CBanditCommentMarker@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Flags:
// Inline-budget: the depth-two predicate and the inline marker Main use the native generated scalar path.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_HOBBE_BANDIT_COMMENT_PASS27
#define FABLE_RETAIL_CCOUNTER
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CISTHINGALIVE_BANDIT_OWNERS_PASS27
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_HobbeCaveScript.h"
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
#include "engine/CIsThingAlive.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

#include "engine/CCounter.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"

extern CGameScriptInterfaceBase* pDSTGame;
inline CCounter::CCounter() : RelevantWorldFrame(pDSTGame->GetWorldFrame()) {}


inline CScriptThing::~CScriptThing() {}

namespace NScript {
CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CQ_HobbeCaveScript::CBanditCommentMarker::Alloc(
        CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBanditCommentMarker(parent, data, me));
}
inline CQ_HobbeCaveScript::CBanditCommentMarker::CBanditCommentMarker(
        CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
CScriptBase* CQ_HobbeCaveScript::CBanditCommentMarker::GetParentScript() const
{
    return ParentClass;
}
void CQ_HobbeCaveScript::CBanditCommentMarker::Init() {}
}

namespace NScript {
inline void CQ_HobbeCaveScript::CBanditCommentMarker::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing Bandit = Game.GetThingWithScriptName("BanditSacrifice");
    SetPredicate(CIsThingAlive(Bandit));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) {
        _Fail_Val.Return();
        return;
    }
    CCounter HungAroundMarkerLongEnough;
    bool BanditNearEnough = false;
    long BanditCloseDistance = pGlobalScriptDef->HC_BanditCloseToMarkerDistance;
    for (; true; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        BanditNearEnough = false;
        if (ParentClass->GotTreasure > 1 && !Game.IsRegionLoaded("HobbeCaveNymphChamber")) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            return;
        }
        if (ParentClass->ReleaseBrat) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            return;
        }
        if (IsDistanceBetweenThingsUnder(Me, Bandit, (float)BanditCloseDistance)) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            BanditNearEnough = true;
            if (!ParentClass->HobbesCloseby &&
                HungAroundMarkerLongEnough > (float)pGlobalScriptDef->HC_BanditTimeBeforeComment &&
                ParentClass->MakeBanditComment(Me.GetDataString(), Bandit)) {
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
                Game.RemoveThing(Me, false, true);
                return;
            } else if (HungAroundMarkerLongEnough > (float)pGlobalScriptDef->HC_BanditTimeBeforeComment &&
                       ParentClass->HobbesCloseby) {
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
                HungAroundMarkerLongEnough = (float)(pGlobalScriptDef->HC_BanditTimeBeforeComment - 1);
            }
        }
        if (!BanditNearEnough) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            HungAroundMarkerLongEnough = 0.0f;
        }
    }
    if (IsActiveThreadTerminating()) {
        _Fail_Val.Return();
        return;
    }
}
}