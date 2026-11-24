// CTCInventoryTrade::ConstructSpecialShopListPC(const std::vector<std::pair<long,long> >& items, CCharString& name) -- creates the special-shop list, totals the price of the special items the hero can still buy and writes it into the buy entry's text
// Leaf: ?ConstructSpecialShopListPC@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A8A5D, 521 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CGameDefinitionManager::Get 0x0044C6B0 /
//   GetGlobalIndexFromClassIndex 0x009AD390 / GetDef<CUIMiscThingsDef> 0x0042B312 (out of line in retail; same-TU body as the console twin
//   0x005A29F4), GetMainPlayer 0x00449960, PeekPControlledCreature 0x00487DD0, CTCInventoryItem::GetInventoryType 0x005D99D0,
//   bitset::test 0x00410DE0 + LowerBound 0x004365B0 (inline PeekTC), IsItemAffordable 0x0059DA10, IsSpaceInInventoryForItem 0x005BF2ED,
//   IsSpecialCase 0x0059C983, AlreadyHaveSpecial 0x0059FC17, CComponent::FindChild (slot 3), CText::GetValue (slot 142) /
//   SetValue(const CWideString&,bool,bool) (slot 144), CWideString copy 0x0099B720 / operator= 0x0099B7D0 / += 0x0099B8D0 / ~ 0x0099B510,
//   CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00.
// Name fix (agentNF89, q11163): moved onto engine/CTCInventoryTrade.h (retail branch, the console twin 0x005A29F4's recipe): the callee
//   0x0059DA10 is ?IsItemAffordable@CTCInventoryTrade@@QAE_NJJ@Z (Ego_r.pdb + FableWin.pdb; the header's ACTIONSLISTS declaration), the
//   item test 0x005BF2ED is ?IsSpaceInInventoryForItem@CTCInventoryBase@NInventory@@QBE_NJ@Z, 0x005CC772 the static
//   ?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ (both PDBs), and the text slots are NUISystem::CText's GetValue / SetValue.
// Shape: the front-end manager goes through a named local so GetInstance is called before CreateComponent's pushes.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
using namespace NUISystem;

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00

// Same-TU body of the def lookup retail's TU holds (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312), as the landed
// console twin 0x005A29F4 and CTCInventoryWeapons::ConstructScreen 0x005C8459.
#pragma auto_inline(off)
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

CComponent* CTCInventoryTrade::ConstructSpecialShopListPC(const std::vector<std::pair<long, long> >& items, CCharString& name)
{
    CManager* frontEnd = CManager::GetInstance();
    CComponent* list = frontEnd->CreateComponent(name, false);
    CDefPointer<const CUIMiscThingsDef> misc;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &misc);
    CComponent* entry = list->FindChild("PC_SPECIALSHOP_MENU_BUY_ENTRY");
    TotSpecialItemsToBuy = 0;
    if (!items.empty() || !NotInStock.empty()) {
        for (std::vector<std::pair<long, long> >::const_iterator it = items.begin(); it != items.end(); ++it) {
            long defIndex = it->first;
            long count = it->second;
            const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
            const NInventory::CTCInventoryBase* inventory = 0;
            hero->PeekTC(CTCInventoryItem::GetInventoryType(defIndex), &inventory);
            if (IsItemAffordable(defIndex, 1) && (inventory->IsSpaceInInventoryForItem(defIndex) || IsSpecialCase(defIndex)) && count > 0
                && !AlreadyHaveSpecial(defIndex))
                TotSpecialItemsToBuy += it->second;
        }
        CText* text = static_cast<CText*>(entry->FindChild("PC_TRADING_BUY_TEXT"));
        SpecialItemString = text->GetValue();
        CWideString price(SpecialItemString);
        price += CTCInventoryStats::GetXString();
        price += GFIntToWideString(TotSpecialItemsToBuy);
        text->SetValue(price, true, true);
    }
    return list;
}
