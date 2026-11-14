// CTCContainerRewardHero::GetRewardItems(std::vector<std::pair<long,CCharString> >& items, long& gold) -- collect what the container gives: its gold, then either its stored items (dynamic potion / resurrection dummies resolved, a failed resurrection pick worth 20 gold) or one random object family pick from the def (a gold object spawned at once, otherwise the object as an item)
// Leaf: ?GetRewardItems@CTCContainerRewardHero@@IAEXAAV?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@AAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074E860, 1005 bytes (`ret 8` twice, last instruction `jmp 0x0074EBE7`).
// Name: Ego_r.pdb 0x00760960 + FableWin.pdb 0x0259A5F0 ?GetRewardItems@CTCContainerRewardHero@@IAEX...@Z; reloc_bindings.tsv binds 0x0074E860;
//   callers RewardHeroWithItems 0x0074EE90 (landed) and 0x0074EC77.
// Callees: vector<pair<long,CCharString> >::erase 0x0074FB20 / push_back 0x0074FE50 (out of line), CVectorMap::LowerBound const 0x004365B0
//   (the expanded PeekTC<const CTCGold>, interface 0xCC), CTCGold::GetGold 0x007CBDC0, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, GetDynamicPotionDefIndex 0x0074E280,
//   GetDynamicResurrectionDefIndex 0x0074E460, GetSubDef<CContainerRewardHeroDef> 0x0074F6A0, CTCBase::PeekWorld 0x00686A80,
//   CWorld::GetWorldSeed 0x0049D810, GFRandom 0x004CDAE0 (left out of line), vector<CDefIndex>::at 0x0074CBC0, CTCBase::GetDefinitionManager
//   0x006869D0, GetDef<CObjectFamilyDef> 0x0074F760, CObjectFamilyDef::GetRandomObject 0x008ED590, GetDef<CThingBaseDef> 0x0042AF3C,
//   GetSubDef<CGoldDef> 0x006AE320, CThing::GetPos 0x004C73D0, CreateNewObject 0x0074E560, pair<long,CCharString>(const long&, const
//   CCharString&) 0x00695250 (out of line at the def-path site only). Strings: OBJECT_DYNAMIC_POTION_DUMMY 0x012694B8,
//   OBJECT_DYNAMIC_RESURRECTION_DUMMY 0x01269494, "" 0x0122D70E.
// Shape: the GetDef / GetSubDef template bodies are VISIBLE (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY /
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, both noinline, as the retail calls show): VC7.1 then knows the out handles do not escape,
//   keeps each def in esi / ebx, drops the handles' dead `Object = 0` stores and shares the thing-def / gold-def slot [esp+18h]
//   (declaration-only templates: 1119 bytes). ContainerItems.clear() (STLport's inline erase(begin(), end()) through the `this` of
//   clear, i.e. esi) for the final erase. CContainerRewardHeroDef / CObjectFamilyDef: local models (their engine headers are quarantined).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CGOLDDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_AT_OUTOFLINE
#define FABLE_RETAIL_CTCCONTAINERREWARDHERO_GETREWARDITEMS
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

class CContainerRewardHeroDef : public CSubDefClassBase {      // RTTI .?AVCContainerRewardHeroDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName();
    CArray<CDefIndex> ObjectFamilies;                           // +0x28 (Ego_r CArray<CDefIndex> ObjectFamilies)
};
class CObjectFamilyDef : public CDefClassBase {
public:
    long GetRandomObject() const;                               // 0x008ED590 ?GetRandomObject@CObjectFamilyDef@@QBEJXZ
};

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
