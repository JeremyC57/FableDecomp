// 00927f30 CAIStateGroup_SetupWares::Update() -- puts away or displays shop wares according to opening hours.
// Retail:0x00927F30,1331 bytes,thiscall,complete body.
// Flags: none
// Leaf: ?Update@CAIStateGroup_SetupWares@@UAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASBASEDEF
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_STOCKSTATE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_IS_THING_DEAD_DECL_ONLY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CAIStateGroup_SetupWares.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCCreatedEntity.h"
#include "engine/CStockItemDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CCreatureAction_ShopkeeperPutAway.h"
#include "engine/CCreatureAction_ShopkeeperGetOut.h"
// Original IS_THING_DEAD<CIntelligentPointer<CThing> > is external at005CC1B0..005CC1D4 (36 bytes).
#include "engine/ThingAlive.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif

// Native external GetSubDef<CStockItemDef>,0059350C..005935AE,162 bytes: size optimization and frame pointer.
// This real specialization retains the shared template's complete original behavior.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
template<> bool CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>* def)const
{
    const CSubDefInfo* psub_def_info=GetPSubDefInfo(CStockItemDef::StatGetTypeName());
    if(psub_def_info){
        CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
        def->SetDynamicCast(pbase);
        return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

void CAIStateGroup_SetupWares::Update()
{
    CTCShop* tc_shop;
    CTCShopKeeper* tc_shopkeeper;
    if (!GetCreature().GetTC(&tc_shopkeeper) || !tc_shopkeeper->GetShopTC()) {
        Terminate();
        return;
    }
    tc_shop = tc_shopkeeper->GetShopTC();
    tc_shop->SetStockState(CTCShop::STOCKED_DONTKNOW);
    if (!tc_shop->IsOpeningHours()) {
        std::vector<CTCShop::GoodsPlace> places = tc_shop->GetFreeSaleItemPlaces(true);
        CTCThingOwner* tc_thing_owner;
        if (tc_shop->GetParentThing().GetTC(&tc_thing_owner)) {
            const std::list<CIntelligentPointer<CThing> >& objects = tc_thing_owner->GetObjectList();
            for (std::list<CIntelligentPointer<CThing> >::const_iterator object_it=objects.begin();object_it!=objects.end();++object_it) {
                CTCCreatedEntity* created;
                CDefPointer<const CStockItemDef> stock_item;
                if (IS_THING_ALIVE(*object_it) && (*object_it)->GetTC(&created) && (*object_it)->HasBaseDef() && (*object_it)->PeekBaseDef().GetSubDef(&stock_item)) {
                    float best_dist=0.0f;
                    EAIActionReturn ret;
                    C3DVector best=(*object_it)->GetPos();
                    bool nav_route_calculated;
                    for (std::vector<CTCShop::GoodsPlace>::iterator i=places.begin();i!=places.end();++i) {
                        float dist=(i->pos-(*object_it)->GetPos()).GetMagnitude();
                        if (i==places.begin() || dist<best_dist) {
                            best_dist=dist;
                            best=i->pos;
                        }
                    }
                    ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,best,1.4f,nav_route_calculated,AI_MOVEMENT_WALKING,true,false,true,0,true,0.0f);
                    if (ret!=AI_ACTION_FINISHED || IS_THING_DEAD(*object_it)) { Terminate();return; }
                    ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,best,0x11,0.0027777f);
                    if (ret!=AI_ACTION_FINISHED || IS_THING_DEAD(*object_it)) { Terminate();return; }
                    AIAction_PerformActionAndWait(CCreatureAction_ShopkeeperPutAway(GetCreature(),object_it->Get()),false);
                }
            }
        }
        tc_shop->SetStockState(CTCShop::STOCKED_NO);
    } else {
        std::vector<CTCShop::GoodsPlace> places=tc_shop->GetFreeSaleItemPlaces(false);
        std::vector<long> items=tc_shop->GetSaleItemsToPlace();
        for (int n=0;n<std::min(places.size(),items.size());++n) {
            EMovementType movement=AI_MOVEMENT_WALKING;
            EAIActionReturn ret;
            bool nav_route_calculated;
            if (n==0) movement=AI_MOVEMENT_RUNNING;
            ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,places[n].pos,1.4f,nav_route_calculated,movement,true,false,true,0,true,0.0f);
            if(ret!=AI_ACTION_FINISHED){Terminate();return;}
            ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,places[n].pos,0x11,0.0027777f);
            if(ret!=AI_ACTION_FINISHED){Terminate();return;}
            AIAction_PerformActionAndWait(CCreatureAction_ShopkeeperGetOut(GetCreature(),tc_shop,places[n].pos,places[n].rhset,items[n]),false);
        }
        tc_shop->SetStockState(CTCShop::STOCKED_YES);
    }
    Terminate();
}
