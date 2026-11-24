// 007820e0 CTCBoastingPosition::FrameUpdate
// Leaf: ?FrameUpdate@CTCBoastingPosition@@UAEXXZ
// Flags:
#define FABLE_RETAIL_BOAST_AREA_PASS27
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTCBOASTINGPOSITION_PODIUM
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEALLIFE_START_ORIGINAL
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBoastingPosition.h"
#include "engine/CTCBoastingPodium.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/ThingAlive.h"
#include "engine/GFSqr.h"

void CTCBoastingPosition::FrameUpdate()
{
    if (IS_THING_ALIVE(PBoastingPodium) && PositionActivated) {
        CThingCreatureBase* phero;
        GetThingSearchTools().GetPBestThing(CThingFilter_HasTC(TCI_HERO),CThingCompare_Nearest(GetParentThing().GetPos()),&phero);
        if (IS_THING_ALIVE(phero) && BoastOn) {
            const CTCHeroStats* ptc_hero_stats;
            bool hero_can_afford_boast=phero->PeekTC(&ptc_hero_stats) ? ptc_hero_stats->GetMoney()>=Cost : true;
            if ((phero->GetPos()-OrbPosition).GetSquaredMagnitude()<GFSqr(1.5f) &&
                (phero->GetPos()-PBoastingPodium->GetPos()).GetSquaredMagnitude2D()<
                (GetParentThing().GetPos()-PBoastingPodium->GetPos()).GetSquaredMagnitude2D()) {
                if (!HeroInsideRadius) {
                    CTCBoastingPodium* ptc_boasting_podium;
                    PBoastingPodium->GetTC(&ptc_boasting_podium);
                    ptc_boasting_podium->OnHeroEnterBoastArea(BoastIndex,hero_can_afford_boast,*phero);
                    HeroInsideRadius=true;
                }
                CTCTargetingBase* ptc_targeting;
                if (phero->GetTC(&ptc_targeting)) {
                    CThingGameObject& parent_game_object=*GFDebugDynamicCast(&GetParentThing(),(CThingGameObject*)0);
                    ptc_targeting->SetPTargetedThing(&parent_game_object);
                }
            } else if (HeroInsideRadius) {
                CTCBoastingPodium* ptc_boasting_podium;
                PBoastingPodium->GetTC(&ptc_boasting_podium);
                ptc_boasting_podium->OnHeroLeaveBoastArea(BoastIndex,hero_can_afford_boast,*phero);
                HeroInsideRadius=false;
            }
        }
    }
    GetParentThing().SetAsFrameUpdated();
    GetParentThing().SetAsRenderedThisFrame(4);
}