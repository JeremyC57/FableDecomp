// CTCInventoryTrade::SellItem -- sell hero inventory items and refresh the console trade screen.
// Leaf: ?SellItem@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z
// Retail: 0x005AECC0,2446bytes,thiscall.
#define FABLE_RETAIL_CDELETION_BODIES
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#define FABLE_RETAIL_STLP_LIST_CTORS_PASS22
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#define FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CTCINVENTORYTRADE_SELLITEM
#define FABLE_RETAIL_CTCSHOP_BUYITEM
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
#define FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTCSHOP_INTERFACETYPE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#define FABLE_RETAIL_CTCSHOP_WANTED
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;

#include "engine/CThingPlayerCreature.h"
CWideString FABLE_FASTCALL GFIntToWideString(long);
namespace { extern CCharString SELL_SUBTITLE,CLOTHES_SUBTITLE,WEAPONS_SUBTITLE,ITEMS_SUBTITLE; }
typedef std::map<NUISystem::CComponent*,std::pair<long,long> > CComponentPairMap;
typedef std::map<NUISystem::CComponent*,NUISystem::CList*> CComponentListMap;
// The external original map copies are speed optimized with an omitted frame.
template<> CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
template<> CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::const_iterator CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key) const
{
    const_iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}

template std::_Rb_tree_node_base* std::_Rb_global<bool>::_M_increment(std::_Rb_tree_node_base*);
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> std::pair<CWideString,long>::pair(const std::pair<CWideString,long>& x):first(x.first),second(x.second){}
template<> std::_List_node<NUISystem::CComponent*>* std::list<NUISystem::CComponent*>::_M_create_node(NUISystem::CComponent* const& x){
std::_List_node<NUISystem::CComponent*>* p=this->_M_node.allocate(1);new((void*)&p->_M_data) NUISystem::CComponent*(x);return p;}
// These shared external copies precede this console screen's size/frame options.
template<> std::_Rb_tree_node_base* std::_Rb_global<bool>::_M_increment(std::_Rb_tree_node_base* node) {
    if(node->_M_right!=0) {node=node->_M_right;while(node->_M_left!=0)node=node->_M_left;}
    else {std::_Rb_tree_node_base* parent=node->_M_parent;while(node==parent->_M_right){node=parent;parent=parent->_M_parent;}if(node->_M_right!=parent)node=parent;}
    return node;
}
typedef std::_Rb_tree<NUISystem::CComponent*,std::pair<NUISystem::CComponent* const,std::pair<long,long> >,std::_Select1st<std::pair<NUISystem::CComponent* const,std::pair<long,long> > >,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent* const,std::pair<long,long> > > > ItemsTree;
template<> std::_Rb_tree_node_base* ItemsTree::_M_lower_bound(NUISystem::CComponent* const& key)const {
_Link_type y=this->_M_header._M_data,x=_M_root();while(x!=0){if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}else x=_S_right(x);}return y;
}
template<> std::_Rb_tree_node_base* ItemsTree::_M_upper_bound(NUISystem::CComponent* const& key)const {
_Link_type y=this->_M_header._M_data,x=_M_root();while(x!=0){if(_M_key_compare(key,_S_key(x))) {y=x;x=_S_left(x);}else x=_S_right(x);}return y;
}
typedef std::_Rb_tree<NUISystem::CComponent*,std::pair<NUISystem::CComponent* const,std::pair<CWideString,long> >,std::_Select1st<std::pair<NUISystem::CComponent* const,std::pair<CWideString,long> > >,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent* const,std::pair<CWideString,long> > > > TitlesTree;
template<> std::_Rb_tree_node_base* TitlesTree::_M_lower_bound(NUISystem::CComponent* const& key)const {
_Link_type y=this->_M_header._M_data,x=_M_root();while(x!=0){if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}else x=_S_right(x);}return y;
}
template<> std::_Rb_tree_node_base* TitlesTree::_M_upper_bound(NUISystem::CComponent* const& key)const {
_Link_type y=this->_M_header._M_data,x=_M_root();while(x!=0){if(_M_key_compare(key,_S_key(x))) {y=x;x=_S_left(x);}else x=_S_right(x);}return y;
}
typedef std::_Rb_tree<NUISystem::CComponent*,std::pair<NUISystem::CComponent* const,NUISystem::CList* >,std::_Select1st<std::pair<NUISystem::CComponent* const,NUISystem::CList* > >,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent* const,NUISystem::CList* > > > ListsTree;
template<> std::_Rb_tree_node_base* ListsTree::_M_lower_bound(NUISystem::CComponent* const& key)const {
_Link_type y=this->_M_header._M_data,x=_M_root();while(x!=0){if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}else x=_S_right(x);}return y;
}
template<> std::_Rb_tree_node_base* ListsTree::_M_upper_bound(NUISystem::CComponent* const& key)const {
_Link_type y=this->_M_header._M_data,x=_M_root();while(x!=0){if(_M_key_compare(key,_S_key(x))) {y=x;x=_S_left(x);}else x=_S_right(x);}return y;
}
template<> template<> std::_Rb_tree_node<std::pair<NUISystem::CComponent* const,NUISystem::CList* > >* ListsTree::_M_find<NUISystem::CComponent*>(NUISystem::CComponent* const& key)const {
_Link_type y=this->_M_header._M_data,x=_M_root();while(x!=0){if(!_M_key_compare(_S_key(x),key)){y=x;x=_S_left(x);}else x=_S_right(x);}
if(y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;return y;
}
typedef std::map<NUISystem::CComponent*,std::pair<long,long> > ItemsMap;
template<> ItemsMap::iterator ItemsMap::insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
typedef std::map<NUISystem::CComponent*,std::pair<CWideString,long> > TitlesMap;
template<> TitlesMap::iterator TitlesMap::insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
typedef std::map<NUISystem::CComponent*,NUISystem::CList* > ListsMap;
template<> ListsMap::iterator ListsMap::insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
template<> std::pair<NUISystem::CComponent* const,std::pair<CWideString,long> >::pair(NUISystem::CComponent* const& a,const std::pair<CWideString,long>& b):first(a),second(b){}
template<> std::list<NUISystem::CComponent*>::iterator std::list<NUISystem::CComponent*>::insert(iterator position,NUISystem::CComponent* const& x) {
    _List_node<NUISystem::CComponent*>* tmp=_M_create_node(x);
    _List_node_base* n=position._M_node;
    _List_node_base* p=n->_M_prev;
    tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
    return iterator(tmp);
}
template<> std::pair<ItemsTree::iterator,ItemsTree::iterator> ItemsTree::equal_range(NUISystem::CComponent* const& key){return std::pair<iterator,iterator>(lower_bound(key),upper_bound(key));}
template<> ptrdiff_t std::__distance<ItemsTree::iterator>(const ItemsTree::iterator& first,const ItemsTree::iterator& last,const std::input_iterator_tag&) {ptrdiff_t n=0;ItemsTree::iterator it(first);while(it!=last){++it;++n;}return n;}
template<> std::pair<ListsTree::iterator,ListsTree::iterator> ListsTree::equal_range(NUISystem::CComponent* const& key){return std::pair<iterator,iterator>(lower_bound(key),upper_bound(key));}
template<> ptrdiff_t std::__distance<ListsTree::iterator>(const ListsTree::iterator& first,const ListsTree::iterator& last,const std::input_iterator_tag&) {ptrdiff_t n=0;ListsTree::iterator it(first);while(it!=last){++it;++n;}return n;}
#ifdef _MSC_VER
#pragma optimize("y",off)
#endif
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
CComponent* CTCInventoryTrade::GetMenuBranchFromDefIndex(long defIndex, CComponent* menu, EItem_Menu_Type& type)
{
    CComponent* branch;
    long melee, ranged, headwear, torso, gloves, legwear, footwear, suits;
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_FOOD");
    melee    = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_MELEE_WEAPONS");
    ranged   = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_RANGED_WEAPONS");
    headwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_HEADWEAR");
    torso    = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR");
    gloves   = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_GLOVES");
    legwear  = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_LEGWEAR");
    footwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_FOOTWEAR");
    suits    = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_CLOTHES_SUITS");
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_GIFTS");
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_TROPHIES");
    CGameDefinitionManager::Get().GetDefGlobalIndexFromName("INVENTORY_CATEGORY_POTIONS");
    long category = CTCInventoryItem::GetInventoryCategory(defIndex);
    if (category == melee)         { branch = menu->FindChild("UI_TRADING_MENU_MELEE_ENTRY");    type = MENU_TYPE_WEAPONS; }
    else if (category == ranged)   { branch = menu->FindChild("UI_TRADING_MENU_RANGED_ENTRY");   type = MENU_TYPE_WEAPONS; }
    else if (category == headwear) { branch = menu->FindChild("UI_TRADING_MENU_HEAD_ENTRY");     type = MENU_TYPE_CLOTHING; }
    else if (category == torso)    { branch = menu->FindChild("UI_TRADING_MENU_TORSO_ENTRY");    type = MENU_TYPE_CLOTHING; }
    else if (category == gloves)   { branch = menu->FindChild("UI_TRADING_MENU_HANDS_ENTRY");    type = MENU_TYPE_CLOTHING; }
    else if (category == legwear)  { branch = menu->FindChild("UI_TRADING_MENU_LEGS_ENTRY");     type = MENU_TYPE_CLOTHING; }
    else if (category == footwear) { branch = menu->FindChild("UI_TRADING_MENU_FEET_ENTRY");     type = MENU_TYPE_CLOTHING; }
    else if (category == suits)    { branch = menu->FindChild("UI_TRADING_MENU_SUITS_ENTRY");    type = MENU_TYPE_CLOTHING; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)0) { branch = menu->FindChild("UI_TRADING_MENU_TROPHIES_ENTRY"); type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)1) { branch = menu->FindChild("UI_TRADING_MENU_POTIONS_ENTRY");  type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)2) { branch = menu->FindChild("UI_TRADING_MENU_GIFTS_ENTRY");    type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)3) { branch = menu->FindChild("UI_TRADING_MENU_PRODUCE_ENTRY");  type = MENU_TYPE_ITEMS; }
    else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == (EUIInventoryCategory)4) { branch = menu->FindChild("UI_TRADING_MENU_OTHERS_ENTRY");   type = MENU_TYPE_ITEMS; }
    else                           { branch = menu->FindChild("UI_TRADING_MENU_OTHERS_ENTRY");   type = MENU_TYPE_ITEMS; }
    return branch;
}
void CTCInventoryTrade::SellItem(NUISystem::CComponent* item, long count)
{
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    std::pair<long, long>& sell_entry = ItemsToSell[item];
    long def_index = sell_entry.first;
    CWideString x_string = CTCInventoryStats::GetXString();
    sell_entry.second -= count;
    NUISystem::CComponent* sell_menu = Components[0]->FindChild(CCharString("UI_TRADING_MENU_SELL_ENTRY"));
    EItem_Menu_Type menu_type;
    NUISystem::CComponent* branch = GetMenuBranchFromDefIndex(def_index, sell_menu, menu_type);
    NumSellableThings[branch].second -= count;
    NUISystem::CComponent* branch_text = branch->FindFirstChildOfType(NUISystem::UI_TYPE_TEXT);
    CWideString subtitle(NumSellableThings[branch].first);
    if (NumSellableThings[branch].second > 0) {
        subtitle += CTCInventoryStats::GetXString();
        subtitle += GFIntToWideString(NumSellableThings[branch].second);
    }
    static_cast<NUISystem::CText*>(branch_text)->SetValue(subtitle, true, true);
    RefreshSubtitles(sell_menu->FindChild(CCharString("UI_TRADING_GOODS_LIST")), NumSellableThings);

    CWideString text;
    CCharString amount_name;
    NUISystem::CComponent* category_text;
    long category_total;
    switch (menu_type) {
    case MENU_TYPE_CLOTHING:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_CLOTHING_TEXT"));
        category_total = TotNumClothesToSell -= count;
        text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + x_string;
        amount_name = CLOTHES_SUBTITLE;
        break;
    case MENU_TYPE_WEAPONS:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_WEAPONS_TEXT"));
        category_total = TotNumWeaponsToSell -= count;
        text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + x_string;
        amount_name = WEAPONS_SUBTITLE;
        break;
    case MENU_TYPE_ITEMS:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_ITEMS_TEXT"));
        category_total = TotNumItemsToSell -= count;
        text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + x_string;
        amount_name = ITEMS_SUBTITLE;
        break;
    default:
        goto totals;
    }
    if (category_text != 0) {
        text += GFIntToWideString(category_total);
        static_cast<NUISystem::CText*>(category_text)->SetValue(text, true, true);
        static_cast<NUISystem::CText*>(sell_menu->FindChild(amount_name))->SetValue(text, true, true);
    }
totals:
    text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + x_string;
    text += GFIntToWideString(TotNumItemsToSell + TotNumWeaponsToSell + TotNumClothesToSell);
    static_cast<NUISystem::CText*>(sell_menu->FindChild(CCharString("UI_TRADING_SELL_TEXT")))->SetValue(text, true, true);
    static_cast<NUISystem::CText*>(sell_menu->FindChild(SELL_SUBTITLE))->SetValue(text, true, true);

    NUISystem::CViewport* text_area = (NUISystem::CViewport*)Components[0]->FindChild(CCharString("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    if (sell_entry.second == 0) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
        item->SetDeletion(deletion);
        ItemsToSell.erase(item);
        if (NumSellableThings[branch].second <= 0) {
            branch->MakeUnavailableDynamically();
            NUISystem::CComponent* arrow = branch->FindChild(CCharString("UI_SELECTION_ARROW_RIGHT"));
            if (arrow != 0) {
                NUISystem::CDeletion arrow_deletion;
                arrow_deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(arrow_deletion);
            }
            SellListsByCategory.erase(branch);
        }
    } else
        RefreshNameAndTitle(item, def_index, sell_entry.second);

    {
    CTCShop* shop = 0;
    CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* hero_stats = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    hero->GetTC(TCI_HERO_STATS, &hero_stats);
    long price = shop->GetCurrentBuyPrice(def_index, count);
    for (long i = 0; i < count; ++i)
        shop->BuyItem(def_index);
    hero_stats->AddMoney(price, true);
    NInventory::CTCInventoryBase* inventory = 0;
    hero->GetTC((ETCInterfaceType)CTCInventoryItem::GetInventoryType(def_index), &inventory);
    inventory->RemoveItemFromInventory(def_index, count);

    for (CComponentPairMap::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        if ((*it).second.second > 0) {
            const NInventory::CTCInventoryBase* inventory = 0;
            hero->PeekTC((ETCInterfaceType)CTCInventoryItem::GetInventoryType((*it).second.first), &inventory);
            if ((*it).first->FindChild(CCharString("UI_LIST_BUY_ACTIONS")) == 0
                && shop->GetCurrentSellPrice((*it).second.first, 1) <= hero_stats->GetMoney()
                && (IsSpecialCase((*it).second.first) || inventory->IsSpaceInInventoryForItem((*it).second.first))) {
                (*it).first->MakeAvailableDynamically();
                CCountedPointer<NUISystem::CComponent> arrow(
                    NUISystem::CManager::GetInstance()->CreateComponent("UI_SELECTION_ARROW_RIGHT", false));
                (*it).first->AddChildDynamically(arrow);
            }
        }
    }

    EItem_Menu_Type buy_type;
    NUISystem::CComponent* buy_menu = Components[0]->FindChild(CCharString("UI_TRADING_MENU_BUY_ENTRY"));
    NUISystem::CComponent* buy_branch = GetMenuBranchFromDefIndex(def_index, buy_menu, buy_type);
    CComponentPairMap::iterator buy_it;
    for (buy_it = ItemsToBuy.begin(); buy_it != ItemsToBuy.end(); ++buy_it) {
        if (def_index == (*buy_it).second.first) {
            long old_count = (*buy_it).second.second;
            (*buy_it).second.second += count;
            if (old_count == 0) {
                (*buy_it).first->MakeAvailableDynamically();
                CCountedPointer<NUISystem::CComponent> arrow(
                    NUISystem::CManager::GetInstance()->CreateComponent("UI_SELECTION_ARROW_RIGHT", false));
                (*buy_it).first->AddChildDynamically(arrow);
            }
            RefreshNameAndTitle((*buy_it).first, (*buy_it).second.first, (*buy_it).second.second);
            goto refresh;
        }
    }
    {
        CComponentListMap::iterator list_it = BuyListsByCategory.find(buy_branch);
        if (list_it != BuyListsByCategory.end())
            AddBuyItemToList(def_index, count, (*list_it).second, text_area, true);
        else {
            CCountedPointer<NUISystem::CComponent> sub_list(
                NUISystem::CManager::GetInstance()->CreateComponent("UI_SUB_LIST", false));
            NUISystem::CComponent* list = sub_list.Data;
            static_cast<NUISystem::CList*>(list)->InitialiseOffsets(1);
            AddBuyItemToList(def_index, count, (NUISystem::CList*)list, text_area, false);
            buy_branch->AddChildDynamically(sub_list);
            BuyListsByCategory[buy_branch] = (NUISystem::CList*)list;
        }
    }
refresh:
    UpdateBuyActionsList();
    UpdateSellActionsList();
    RefreshDescriptions();
    RefreshBuyPossibilityNumbers();
    NUISystem::CComponent* gold = Components[0]->FindChild(CCharString("UI_GOLD_AMOUNT_TRADE_SCREEN"));
    if (gold != 0)
        static_cast<NUISystem::CText*>(gold)->SetValue(GFIntToWideString(hero_stats->GetMoney()), true, true);
    RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild(CCharString("UI_TRADING_MENU_BUY_ENTRY")), BuyListsByCategory);
    RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild(CCharString("UI_TRADING_MENU_SELL_ENTRY")), SellListsByCategory);
    NUISystem::CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_BACK);
    }
}