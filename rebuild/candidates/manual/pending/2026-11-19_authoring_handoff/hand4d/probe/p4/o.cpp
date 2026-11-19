// CTCShop::BuyItem(long def) -- the shop buys one unit of an item: add it to stock (from the default stock list when it is not stocked), raise its max stock by the shop def's per-buy change (not below 0), display it on a free sale place when it is cheap enough to display, give the hero skill experience for the profit (a twentieth, at least 1), and when the shop takes donations add the item's basic price to the gold donated and raise the hero's morality on a logarithmic scale between the shop's minimum and maximum donation
// Grade: (in progress)
// Leaf: ?BuyItem@CTCShop@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00774D20, 1149 bytes, thiscall `ret 4`. Name: Ego_r.pdb ?BuyItem@CTCShop@@QAEXJ@Z (0x00782AB0,
//   tc_shop.obj #41 after GetFreeSaleItemPlaces / StockDisplay = retail 0x00774B40 / 0x00774C60) and FableWin.pdb (0x0261E960); the only
//   caller is CTCInventoryTrade::SellHighlightedItem 0x005A3436 (landed, binds 0x00774D20 as BuyItem). The catalog label
//   _global::CTCShop_SellHighlightedItem is descriptive.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_CTCSHOP_BUYITEM
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSHOP_ADDSALEITEMPLACES
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_CREATEDBYOWNER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSEUSRT_SCALE_SQUARED
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SET_ORIGINAL_NAMES
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_CTCSTOCKITEM
#define FABLE_RETAIL_CTCSTOCKITEM_SETASFORSALE
#include "fable_string.h"
#include <math.h>
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CShopDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CShopItem.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CThingObject.h"
#include "engine/CTCCreatedEntity.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCStockItem.h"
#include "engine/ThingAlive.h"
#include "engine/GFPower.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CFilter_Bind2.h"

void CMatrix3x4::Set(const CRightHandedSet& rhset,const C3DVector& translate)
{
    Set(rhset.GetRight(), rhset.GetForward(), rhset.GetUp(), translate);
}

class CTCParticleAttacher : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_ATTACHER; }
    void DetachParticles();                                              // 0x007F8860
    void AttachParticles();                                              // 0x007F8890
};
void CTCShop::AddSaleItem(long goods, const GoodsPlace& place)
{
    long player = PeekPlayerManager().GetMainPlayerNumber();
    CThingObject* thing = CThingObject::Create(goods, place.pos, player, &place.rhset, 0, "");
    CRightHandedSet objectRH = thing->GetPhysicsTC()->GetRHSet();
    C3DVector objectPos = thing->GetPhysicsTC()->GetPosition();
    const CRightHandedSet& rhset = place.rhset;
    CRightHandedSet placeRH;
    placeRH = rhset;
    C3DVector placePos = place.pos;
    C3DVector dummyPos;
    CRightHandedSet dummyRH;
    NGraphicAppearance::CTCGraphicAppearance* appearance = thing->GetTCGraphicAppearance();
    if (appearance && (appearance->GetDummyObject("COUNTER_PLACEMENT", 0, 0, dummyPos, dummyRH, 1.0f)
                       || appearance->GetDummyObject("COUNTER_PLACEMENT ", 0, 0, dummyPos, dummyRH, 1.0f))) {
        CMatrix3x4 objectMatrix;
        objectMatrix.Set(objectRH, objectPos);
        CMatrix3x4 dummyMatrix;
        dummyMatrix.Set(dummyRH, dummyPos);
        CMatrix3x4 placeMatrix;
        placeMatrix.Set(placeRH, placePos);
        CMatrix3x4 dummyInverse;
        dummyInverse.SetInverseOfUniformScaleRotationTranslation(dummyMatrix);
        objectMatrix.PostMultiply(dummyInverse);
        objectMatrix.PostMultiply(placeMatrix);
        thing->GetPhysicsTC()->ResetPosition(C3DVector(objectMatrix.E41, objectMatrix.E42, objectMatrix.E43));
        thing->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(objectMatrix.E31, objectMatrix.E32, objectMatrix.E33),
                                                          C3DVector(objectMatrix.E21, objectMatrix.E22, objectMatrix.E23)));
    }
    thing->GetPhysicsTC()->EnablePhysics(false);
    thing->SetAsCreatedByOwnerThingCreation(true);
    thing->AddTC("CTCCreatedEntity", false, 0);
    CTCCreatedEntity* created;
    thing->GetTC(&created);
    created->SetPCreator(GetParentThing());
    thing->AddTC("CTCOwnedEntity", false, 0);
    CTCOwnedEntity* owned;
    thing->GetTC(&owned);
    owned->SetPOwner(&GetParentThing());
    if (thing->HasTC(TCI_ON_ACTION_USE))
        thing->RemoveTC(TCI_ON_ACTION_USE);
    CTCParticleAttacher* attacher;
    if (thing->GetTC(&attacher)) {
        attacher->DetachParticles();
        attacher->AttachParticles();
    }
    CTCStockItem* stock;
    if (thing->GetTC(&stock)) {
        stock->SetAsForSale(true);
        stock->OnInitialActivate();
    }
}

std::vector<CTCShop::GoodsPlace> CTCShop::GetFreeSaleItemPlaces(bool allow_blocked)
{
    GoodsPlaces.erase(GoodsPlaces.begin(), GoodsPlaces.end());
    AddSaleItemPlacesFromThing(&GetParentThing());
    std::vector<GoodsPlace> places;
    for (long n = 0; n < GoodsPlaces.size(); ++n) {
        if (!allow_blocked) {
            const float BLOCKED_RADIUS = 0.2f;
            CThing* p_thing;
            if (PeekWorld().PeekThingSearchTools().GetPFirstThing(CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In3DRange>(CThingFilter_IsNot(GetParentThing()), CThingFilter_In3DRange(GoodsPlaces[n].pos, BLOCKED_RADIUS)), &p_thing))
                continue;
        }
        places.push_back(GoodsPlaces[n]);
    }
    return places;
}

void CTCShop::BuyItem(long def)
{
    if (Stock.find(def) == Stock.end()) {
        const CShopItem* pshop_item = FindDefaultStockItem(def);
        if (!pshop_item)
            return;
        Stock[def] = *pshop_item;
    }
    CDefPointer<const CShopDef> shop_def;
    GetParentThing().PeekBaseDef().GetSubDef(&shop_def);
    Stock[def].Quantity += 1.0f;
    Stock[def].MaxStock += shop_def->MaxStockChangePerBuy;
    if (Stock[def].MaxStock < 0.0f)
        Stock[def].MaxStock = 0.0f;
    CDefPointer<const CThingBaseDef> pdef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &pdef);
    CDefPointer<const CShopDef> pshop_def;
    CDefPointer<const CDefClassBase> def_class_base = GetDefinitionManager().GetPDefFromGlobalIndex(def);
    CDefPointer<const CStockItemDef> stock_item_def;
    if (pdef->GetSubDef(&pshop_def) && def_class_base->GetSubDef(&stock_item_def)) {
        if (stock_item_def->DefaultPrice <= pshop_def->MaxPriceToDisplay && stock_item_def->CanBeDisplayedInShop) {
            std::vector<GoodsPlace> places = GetFreeSaleItemPlaces(false);
            if (places.size() > 0)
                AddSaleItem(def, places[0]);
        }
    }
    CThingPlayerCreature* phero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroExperience* tc_hero_experience;
    if (IS_THING_ALIVE(phero) && phero->GetTC(&tc_hero_experience)) {
        long bought_price = 0;
        const NInventory::CTCInventoryBase* ptc_inventory_base;
        if (phero->PeekTC(CTCInventoryItem::GetInventoryType(def), &ptc_inventory_base))
            bought_price = ptc_inventory_base->GetBoughtPrice(def);
        long profit = GetCurrentBuyPrice(def, 1) - bought_price;
        if (profit > 0) {
            long experience = (std::max)(profit / 20, 1L);
            tc_hero_experience->AddTrainableStatExperience(HERO_STAT_SKILL, experience);
        }
    }
    CTCHeroStats* ptc_stats;
    if (pshop_def->MaxGoldForDonation > 0 && IS_THING_ALIVE(phero) && phero->GetTC(&ptc_stats)) {
        float value = GetCurrentBasicPrice(def, 1);
        GoldDonated = (long)((float)GoldDonated + value);
        if (value >= pshop_def->MinGoldForDonation) {
            if (value > pshop_def->MaxGoldForDonation)
                value = (float)pshop_def->MaxGoldForDonation;
            float max_value = (float)(pshop_def->MaxGoldForDonation - pshop_def->MinGoldForDonation + 1);
            float max_log_value = GFPower(max_value, pshop_def->LogarithmicPower);
            float morality_adjustment = pshop_def->MaxMoralityForDonation / max_log_value;
            float log_value = GFPower(value - (pshop_def->MinGoldForDonation - 1), pshop_def->LogarithmicPower);
            float morality = morality_adjustment * log_value;
            float immorality = 1.0f - ptc_stats->GetMoralityPercentage();
            float new_morality = 1.0f - immorality * (1.0f - morality);
            long max_morality = ptc_stats->GetMoralityMax();
            ptc_stats->SetMorality((long)(ceil((double)(max_morality * 2) * new_morality) - max_morality));
        }
    }
}
