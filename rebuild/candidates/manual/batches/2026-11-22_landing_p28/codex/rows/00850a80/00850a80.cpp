// CContextSensitiveItemDef::IsValidOverride -- check whether the item is applicable in the current context.
// Leaf: ?IsValidOverride@CContextSensitiveItemDef@@QBE_NABVCThing@@JPBVCTCInventory@@@Z
// Flags: none
// Inline-budget: both ToC2DCoordI calls expand in retail. Plain inline emits two calls in this row.
// Retail: 0x00850A80, 2,576 bytes including two switch tables.
// Retail-asm-helper: GFFloatToLongCeil
// Retail-asm: 0x008511C1 `fistp dword ptr [ebp+0Ch]` inside GFFloatToLongCeil.
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS
#define FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
#define FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_CLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_ISRESPONDINGTOFOLLOWANDWAIT
#define FABLE_RETAIL_CTHINGAICREATURE_WIFE_QUERIES
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_STAMINA
#define FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA
#define FABLE_RETAIL_CTCDOOR
#define FABLE_RETAIL_CTCDOOR_VANDALISM
#define FABLE_RETAIL_CTCSTOCKITEM_TARGETING
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FINDAGUARD
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_CONTEXTSENSITIVEITEM
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCCARRIEDACTIONUSEFISHINGROD_PASS22
#define FABLE_RETAIL_CTCTROPHY_ISMINIGAMEINPROGRESS
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/EOpinionDeedType.h"
#include "engine/ECreatureType.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFSqr.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingMarker.h"
#include "engine/CContextSensitiveItemDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCDoor.h"
#include "engine/CTCStockItem.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCGuard.h"
#include "engine/CTCWife.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCInventory.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGiftDef.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCDiggingSpot.h"
#include "engine/CTCCarriedActionUseFishingRod.h"
#include "engine/CTCCarriedActionUseSpade.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCTrophy.h"
#include "engine/ThingAlive.h"

#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma auto_inline(off)
#endif
// The native CThingBaseDef owner 0042AF3C was compiled for size. Both sub-definition owners were compiled for speed.
template<> inline
bool CDefinitionManager::GetDef<CThingBaseDef>(long index,CDefPointer<const CThingBaseDef>* out) const
{
 if(index>0){
  CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
  if(def.Get()){
   out->SetDynamicCast(def);
   return true;
  }
 }
 return false;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#pragma optimize("",on)
#endif
bool CContextSensitiveItemDef::IsValidOverride(const CThing& thing, long inventory_item_def_index, const CTCInventory* ptc_inventory) const
{
    const CThingPlayerCreature* phero = GFDebugPDynamicCast(&thing, (const CThingPlayerCreature*)0);
    if (IS_THING_ALIVE(phero))
    switch (Type) {
    case CONTEXT_SENSITIVE_LAMP:
        return true;
    case CONTEXT_SENSITIVE_HEALTH:
        if (phero->GetMaxHealth() - phero->GetHealth() >= ExtraData1 || ExtraData2 == 0 || ExtraData2 >= phero->GetHealth())
            return true;
        break;
    case CONTEXT_SENSITIVE_MANA: {
        const CTCHeroStats* ptc_hero_stats;
        if (phero->PeekTC(&ptc_hero_stats) && ptc_hero_stats->GetMaxStamina() - ptc_hero_stats->GetStamina() >= ExtraData1)
            return true;
        break;
    }
    case CONTEXT_SENSITIVE_EXPRESSION: {
        const CTCTargetingBase* ptc_targeting = 0;
        phero->PeekTC(&ptc_targeting);
        if (!ptc_targeting) break;
        CThing* ptarget = ptc_targeting->GetPTargetedThing();
        if (!ptarget) break;
        if (phero->HasTC(TCI_BERSERK))
            return false;
        switch (ExtraData1) {
        case 1: {
            CThingCreatureBase* pcreature = GFDebugPDynamicCast(ptarget, (CThingCreatureBase*)0);
            if ((ptarget->GetTypeFlags() & 8) && pcreature->IsPickPocketable())
                return true;
            return false;
        }
        case 2: {
            const CTCDoor* ptc_door;
            if (ptarget->PeekTC(&ptc_door) && ptc_door->IsLockPickable())
                return true;
            return false;
        }
        case 3: {
            const CTCStockItem* ptc_stock_item;
            if (ptarget->PeekTC(&ptc_stock_item) && ptc_stock_item->IsStealable())
                return true;
            return false;
        }
        case 4: {
            const CThingAICreature* pcreature = GFDebugPDynamicCast(ptarget, (const CThingAICreature*)0);
            if (!(ptarget->GetTypeFlags() & 0x10) || !pcreature->IsRespondingToFollowAndWait())
                return false;
            const CTCFollowed* ptc_followed = 0;
            phero->PeekTC(&ptc_followed);
            if (ptc_followed != 0 && ptc_followed->IsCreatureValidFollower(*pcreature, 0) && !ptc_followed->IsFollower(ptarget))
                return true;
            return false;
        }
        case 5: {
            const CThingAICreature* pcreature = GFDebugPDynamicCast(ptarget, (const CThingAICreature*)0);
            if (!(ptarget->GetTypeFlags() & 0x10) || !pcreature->IsRespondingToFollowAndWait())
                return false;
            const CTCFollowed* ptc_followed = 0;
            phero->PeekTC(&ptc_followed);
            if (ptc_followed != 0 && ptc_followed->IsFollower(ptarget))
                return true;
            return false;
        }
        case 6: {
            if (!(ptarget->GetTypeFlags() & 0x10))
                return false;
            const CTCCreatureOpinionOfHero* ptc_opinion;
            if (ptarget->PeekTC(&ptc_opinion) && ptc_opinion->IsInFindAGuardAndYellForHelp())
                return true;
            CTCGuard* ptc_guard;
            if (ptarget->GetTC(&ptc_guard) && ptc_guard->PlayerIsCriminalAndCanApologise())
                return true;
            return false;
        }
        }
        return false;
    }
    case CONTEXT_SENSITIVE_OPINION_EXPRESSION: {
        const CTCTargetingBase* ptc_targeting = 0;
        phero->PeekTC(&ptc_targeting);
        if (!ptc_targeting) break;
        CThing* ptarget = ptc_targeting->GetPTargetedThing();
        if (ptarget != 0) {
            if (!(ptarget->GetTypeFlags() & 0x10) || !ptarget->HasTC(TCI_CREATURE_OPINION_OF_HERO) || ptarget->HasTC(TCI_GUARD))
                return false;
            CThingAICreature* ptarget_creature = GFDebugPDynamicCast(ptarget, (CThingAICreature*)0);
            if (!TargetCreatureLikelyToReactToDeed(phero, ptarget_creature, (EOpinionDeedType)ExtraData2))
                return false;
            if (ptc_inventory != 0 && inventory_item_def_index > 0) {
                if (ptc_inventory->GetCSOpinionExpressionCount((EContextSensitiveOpinionExpression)ExtraData1) > 1) {
                    if (ptc_inventory->GetLastOpinionExpression((EContextSensitiveOpinionExpression)ExtraData1) == inventory_item_def_index)
                        return false;
                    if (ptc_inventory->GetCSOpinionExpressionCount((EContextSensitiveOpinionExpression)ExtraData1) > 2 &&
                        ptc_inventory->GetLastButOneOpinionExpression((EContextSensitiveOpinionExpression)ExtraData1) == inventory_item_def_index)
                        return false;
                }
            }
            if (ExtraData1 == 4 || ExtraData1 == 5) {
                return true;
            } else if (ptarget_creature->GetCreatureType() != HUMAN_CHILD && (ExtraData1 == 2 || ExtraData1 == 3)) {
                return true;
            } else if (ptarget_creature->IsCourtable() && (ExtraData1 == 0 || ExtraData1 == 1)) {
                const CTCWife* ptc_wife;
                if (!(ExtraData1 == 1 && ptarget_creature->PeekTC(&ptc_wife) && !ptc_wife->IsCourtableAndNotBlockingIt()))
                    return true;
            }
        } else if (ExtraData2 > 0 && ExtraData2 != 0 && ExtraData2 < 0x5c) {
            const CTCHeroOpinionDeedLog* ptc_deedlog = 0;
            phero->PeekTC(&ptc_deedlog);
            if (ptc_deedlog && !ptc_deedlog->PeekPDeedEffectsDef((EOpinionDeedType)ExtraData2)->ReactionRestrictToTarget)
                return true;
        }
        break;
    }
    case CONTEXT_SENSITIVE_GIFT: {
        const CTCTargetingBase* ptc_targeting = 0;
        if (phero->PeekTC(&ptc_targeting)) {
            CThing* ptarget = ptc_targeting->GetPTargetedThing();
            if (ptarget && (ptarget->GetTypeFlags() & 0x10) && ptarget->HasTC(TCI_CREATURE_OPINION_OF_HERO) && !ptarget->HasTC(TCI_GUARD)) {
                CThingAICreature* ptarget_creature = GFDebugPDynamicCast(ptarget, (CThingAICreature*)0);
                if (!TargetCreatureLikelyToReactToDeed(phero,ptarget_creature,(EOpinionDeedType)ExtraData2))
                    return false;
                if (ptarget_creature->GetCreatureType()!=HUMAN_CHILD && (ExtraData1==2 || ExtraData1==0)) {
                    return true;
                } else if (ptarget_creature->IsCourtable() && ExtraData1==1) {
                    const CTCWife* ptc_wife;
                    if (ptarget_creature->PeekTC(&ptc_wife) && ptc_wife->IsCourtableAndNotBlockingIt()) {
        CDefPointer<const CThingBaseDef> pbase_def;
        if (CGameDefinitionManager::Get().GetDef(inventory_item_def_index, &pbase_def)) {
            CDefPointer<const CGiftDef> pgift_def;
            if (!(pbase_def->GetSubDef(&pgift_def) && pgift_def->IsWeddingRing) ||
                (ptc_wife->IsInLoveWithHero() && !ptc_wife->HasReceivedWeddingRing()))
                return true;
        }
                    }
                }
            }
        }
        break;
    }
    case CONTEXT_SENSITIVE_MARKER: {
        C2DBoxI search_area(phero->GetPos().ToC2DCoordI(), ExtraData2);
        switch (ExtraData1) {
        case 1:
            if (CTCCarriedActionUseFishingRod::IsThingAbleToFish(*phero)) return true;
            return false;
        case 2: {
            C2DBoxI area(phero->GetPos().ToC2DCoordI(), GFFloatToLongCeil(3.0f));
            std::vector<CThingMarker*> digging_spots;
            CMainGameComponent::Get()->PeekWorld().PeekThingSearchTools().GetAllThingsInRoughArea<CThingMarker>(
                area, CThingFilter_HasTC(TCI_DIGGING_SPOT), digging_spots);
            for (std::vector<CThingMarker*>::iterator it = digging_spots.begin(); it != digging_spots.end(); ++it) {
                CThingMarker* pmarker = *it;
                const CTCDiggingSpot* ptc_digging_spot;
                if (pmarker->PeekTC(&ptc_digging_spot) && ptc_digging_spot->IsEnabled() && !ptc_digging_spot->IsHidden()) {
                    if (GFGet2DDistanceSquared(C2DVector(phero->GetPos()), C2DVector(pmarker->GetPos())) < GFSqr(3.0f) &&
                        CTCCarriedActionUseSpade::IsThingAbleToDig(*phero))
                        return true;
                }
            }
            return false;
        }
        }
        return false;
    }
    case CONTEXT_SENSITIVE_TROPHY: {
        if (inventory_item_def_index <= 0)
            return false;
        CDefPointer<const CThingBaseDef> pinventory_item_base_def;
        CGameDefinitionManager::Get().GetDef(inventory_item_def_index, &pinventory_item_base_def);
        CDefPointer<const CTrophyDef> ptrophy_def;
        if (pinventory_item_base_def->GetSubDef(&ptrophy_def)) {
            const CTCCarrying* ptc_carrying;
            if (phero->PeekTC(&ptc_carrying)) {
                const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
                const CTCTrophy* ptc_trophy;
                if (IS_THING_ALIVE(pcarried_thing) && pcarried_thing->GetDefGlobalIndex() == inventory_item_def_index &&
                    pcarried_thing->PeekTC(TCI_TROPHY, &ptc_trophy))
                    return ptc_trophy->IsMiniGameInProgress();
            }
        }
        break;
    }
    }
    return false;
}
