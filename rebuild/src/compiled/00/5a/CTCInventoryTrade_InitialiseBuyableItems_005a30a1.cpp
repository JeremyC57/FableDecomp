// CTCInventoryTrade::InitialiseBuyableItems(std::vector<std::pair<long,long> >& items) -- rebuild the buyable list from the shop's stock: the hero's special (tattoo / hair card / title) items are counted into SpecialItemsToDelete and marked in stock, out-of-stock plain items go to NotInStock, every buyable stocked item is added to the trade inventory and to `items`; ItemBuyCount is the total
// Leaf: ?InitialiseBuyableItems@CTCInventoryTrade@@QAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005A30A1, 917 bytes; size TU.
// Callers: CTCInventoryTrade::SellHighlightedItem 0x005A3436, BuyHighlightedItem 0x005A3581, ConstructActualSellInventoryPC 0x005A4CDD,
//   ConstructActualSpecialInventoryPC 0x005A8C66 (and unlanded 0x005A96EC / 0x005AC184 / 0x005AD00F).
// Name fix (agentNF91, q11199 / q11202 / q11204): Ego_r.pdb + FableWin.pdb
//   ?InitialiseBuyableItems@CTCInventoryTrade@@QAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@@Z (engine/CTCInventoryTrade.h
//   declares it at this address). The old row defined it on a row-local 20-class model (own leaf AAU?$vector stand-in; it bound 0x005BF482 as
//   ?GetAllItems@CTCInventoryItem@@.. and 0x0059C6B0 as a public QAEXJJ AddItemOfTypeToInventory). Now the same-TU body of the landed
//   0x005A4CDD over the shared headers (NF89 draft 2, Ego_r member names: SpecialItemsToDelete +0x1A4, ItemBuyCount +0x168, CategoryIdent
//   +0x170, GetBuildingThing), with the old row's else-if order for the two `stock[key] = 1` stores and the header template bodies retail's TU
//   saw: CDefinitionManager::GetDef<T>(long) (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY) and CDefClassBase::GetSubDef<T>
//   (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, with the NF91 overlay opt-in FABLE_RETAIL_CHEROTITLEDEF_TYPENAME: FableWin.pdb
//   ?StatGetTypeName@CHeroTitleDef@@SI?AVCCharString@@XZ + retail literal 0x0123586C). With them VC7.1 knows the two def handles stay local:
//   the thing def lives in ebx across the title test (retail 0x005A3247 region) and both stores keep their own operator[] call. Binds
//   0x005BF482 as the const PDB GetAllItems and 0x0059C6B0 as the private ?AddItemOfTypeToInventory@CTCInventoryTrade@@AAE_NJJ@Z.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE
#define FABLE_RETAIL_CTCSHOP_GETSTOCKLIST
#define FABLE_RETAIL_CTCSHOP_STOCKQUERIES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CHEROTITLEDEF_TYPENAME
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CTCINVENTORYTRADE_ADDITEMOFTYPE
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/CThingBaseDef.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;



#pragma auto_inline(off)
template <class T> long __fastcall CBasicString<T>::Compare(const T* a, const T* b)
{
    for (;;) {
        T ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        T cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
template <class T> bool CBasicString<T>::operator==(const T* text) const
{
    return Compare(PData, text) == 0;
}
#pragma auto_inline(on)

void CTCInventoryTrade::InitialiseBuyableItems(std::vector<std::pair<long, long> >& items)
{
    SpecialItemsToDelete.clear();
    long total = 0;
    CategoryIdent = (EInventoryCategoryIdentifier)0xb;
    CTCShop* pShop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &pShop);
    std::map<long, long> stock = pShop->GetStockList();
    const NInventory::CTCInventoryBase* pInventory = 0;
    CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_INVENTORY, &pInventory);
    std::vector<std::pair<long, long> > heroItems;
    pInventory->GetAllItems(heroItems);
    for (unsigned int i = 0; i < heroItems.size(); ++i) {
        long defIndex = heroItems[i].first;
        long key = defIndex;
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(defIndex, &def);
        CDefPointer<const CHeroTitleDef> heroTitle;
        bool special = !CTCInventoryItem::GetTattooCardDef(defIndex).IsNull() && pShop->IsTattooShop();
        if (!special)
            special = !CTCInventoryItem::GetHairCardDef(defIndex).IsNull() && pShop->IsBarberShop();
        if (special)
            stock[key] = 1;
        else if (def->GetSubDef(&heroTitle) && pShop->IsTitleShop())
            stock[key] = 1;
        else
            goto next;
        {
            std::map<long, long>::iterator it = SpecialItemsToDelete.find(key);
            if (it == SpecialItemsToDelete.end())
                SpecialItemsToDelete[key] = heroItems[i].second;
            else
                (*it).second += heroItems[i].second;
        }
    next:
        ;
    }
    NotInStock.clear();
    for (std::map<long, long>::iterator it = stock.begin(); it != stock.end(); ++it) {
        long count = (*it).second;
        long defIndex = (*it).first;
        long key = defIndex;
        if (count == 0) {
            bool plain = CTCInventoryItem::GetTattooCardDef(defIndex).IsNull() && CTCInventoryItem::GetHairCardDef(defIndex).IsNull();
            if (plain && pShop->GetMaxStock(defIndex))
                NotInStock.push_back(key);
        } else if (CTCInventoryItem::IsBuyable(defIndex)) {
            AddItemOfTypeToInventory(defIndex, count);
            total += count;
            items.push_back(std::pair<long, long>(defIndex, count));
        }
    }
    ItemBuyCount = total;
}

