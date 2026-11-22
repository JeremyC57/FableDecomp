// 0074ec50 CTCContainerRewardHero::CreateItemPickupContainer
// Leaf: ?CreateItemPickupContainer@CTCContainerRewardHero@@IAEXXZ
// Flags:
#define FABLE_RETAIL_REWARD_PICKUP_PASS27
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCCONTAINER_ITEMS
#define FABLE_RETAIL_CTCCONTAINERREWARDHERO_GETREWARDITEMS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGOLDDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_STLP_VECTOR_AT_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
// Inline-budget: the 249-byte growth owner calls the range copy and fill as distinct functions.
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGoldDef.h"
#include "engine/CTCGold.h"
#include "engine/CArray.h"
#include "engine/CDefIndex.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCContainerRewardHero.h"

#include "engine/CContainerRewardHeroDef.h"
#include "engine/CObjectFamilyDef.h"
#include "engine/CThingObject.h"

#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
// Fidelity: size-optimised STLport owners at 004DD782, 004DA68A and 004E34B2.
// The element destructor is implicit. Explicit instantiation selects its native size-mode copy.
template<> inline void FABLE_FASTCALL std::__destroy_aux(
    std::pair<long,CCharString>* first, std::pair<long,CCharString>* last, const std::__false_type&)
{
    for (; first != last; ++first)
        first->~pair();
}

template struct std::pair<long,CCharString>;

template<> inline std::vector<std::pair<long,CCharString> >::~vector()
{
    std::__destroy_aux(this->_M_start, this->_M_finish, std::__false_type());
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
template<> inline bool CDefClassBase::GetSubDef<CContainerRewardHeroDef>(CDefPointer<const CContainerRewardHeroDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CContainerRewardHeroDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CContainerRewardHeroDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = result;
        return true;
    }
    return false;
}

template<> inline bool CDefClassBase::GetSubDef<CGoldDef>(CDefPointer<const CGoldDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CGoldDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CGoldDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = result;
        return true;
    }
    return false;
}


#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
template<> inline bool CDefinitionManager::GetDef<CThingBaseDef>(long index,CDefPointer<const CThingBaseDef>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if (def.Get()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

void CTCContainerRewardHero::GetRewardItems(std::vector<std::pair<long,CCharString> >& items, long& gold)
{
    gold = 0;
    items.clear();
    const CTCGold* gold_tc;
    if (GetParentThing().PeekTC(&gold_tc))
        gold = gold_tc->GetGold();
    long potion_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_DYNAMIC_POTION_DUMMY");
    long resurrection_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_DYNAMIC_RESURRECTION_DUMMY");
    if (!ContainerItems.empty()) {
        for (unsigned long i = 0; i < ContainerItems.size(); ++i) {
            if (ContainerItems[i].first == potion_index) {
                long index = GetDynamicPotionDefIndex();
                if (index > 0)
                    items.push_back(std::pair<long,CCharString>(index, ContainerItems[i].second));
            } else if (ContainerItems[i].first == resurrection_index) {
                long index = GetDynamicResurrectionDefIndex();
                if (index > 0)
                    items.push_back(std::pair<long,CCharString>(index, ContainerItems[i].second));
                else
                    gold += 20;
            } else
                items.push_back(ContainerItems[i]);
        }
        ContainerItems.clear();
        return;
    }
    CDefPointer<const CContainerRewardHeroDef> def;
    if (GetParentThing().PeekBaseDef().GetSubDef(&def) && !def->ObjectFamilies.empty()) {
        long family = GFRandom(def->ObjectFamilies.size(), PeekWorld().GetWorldSeed());
        CDefPointer<const CObjectFamilyDef> family_def;
        if (GetDefinitionManager().GetDef(def->ObjectFamilies.at(family), &family_def)) {
            long index = family_def->GetRandomObject();
            if (index > 0) {
                CDefPointer<const CThingBaseDef> thing_def;
                if (GetDefinitionManager().GetDef(index, &thing_def)) {
                    CDefPointer<const CGoldDef> gold_def;
                    if (thing_def->GetSubDef(&gold_def)) {
                        CreateNewObject(index, GetParentThing().GetPos(), C3DVector(0.0f, 0.25f, 0.25f), "");
                        return;
                    }
                }
                if (index == potion_index)
                    index = GetDynamicPotionDefIndex();
                else if (index == resurrection_index) {
                    index = GetDynamicResurrectionDefIndex();
                    if (index == 0)
                        gold += 20;
                }
                if (index > 0)
                    items.push_back(std::pair<long,CCharString>(index, ""));
            }
        }
    }
}

void CTCContainerRewardHero::CreateItemPickupContainer()
{
    long gold=0;
    std::vector<std::pair<long,CCharString> > items;
    GetRewardItems(items,gold);
    if (!items.empty()) {
        long def_index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_CHEST_REWARD_ON_DEATH");
        if (def_index>0) {
            CThingObject* pobject=CreateNewObject(def_index,GetParentThing().GetPos(),C3DVector(0.0f,0.25f,0.25f),"");
            if (pobject) {
                CTCContainerRewardHero* ptc_container;
                pobject->GetTC(&ptc_container);
                if (ptc_container) {
                    bool targeting_set=false;
                    for (long i=0;i<items.size();++i)
                        ptc_container->InsertItem(items[i].first,items[i].second);
                }
            }
        }
    }
    if (gold>0) {
        long def_index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_GOLDBAG_SMALL_WITH_COINS");
        if (def_index>0) {
            CThingObject* pobject=CreateNewObject(def_index,GetParentThing().GetPos(),C3DVector(0.0f,0.25f,0.25f),"");
            if (pobject) {
                CTCGold* ptc_gold;
                pobject->GetTC(&ptc_gold);
                if (ptc_gold) ptc_gold->SetGold(gold);
            }
        }
    }
}