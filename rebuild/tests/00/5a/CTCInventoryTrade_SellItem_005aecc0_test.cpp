#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include <vector>
#include "fable_compiler.h"
#include "rebuild_abi.h"
struct CCharString {std::string Text;CCharString(){}CCharString(const char*s,long=-1):Text(s){}};
struct CWideString {std::string Text;CWideString(){}CWideString(const char*s):Text(s){}const CWideString&operator+=(const CWideString&s){Text+=s.Text;return *this;}};
CWideString FABLE_FASTCALL operator+(const CWideString&a,const CWideString&b){CWideString r(a);r+=b;return r;}
CWideString FABLE_FASTCALL GFIntToWideString(long n){char b[48];sprintf(b,"%ld",n);return CWideString(b);}
template<class T>class CCountedPointer{public:T*Data;CCountedPointer(T*p):Data(p){}T*operator->()const{return Data;}};
namespace NUISystem {
enum EType{UI_TYPE_TEXT=6};enum EEvent{UI_EVENT_BACK=5};
class CDeletion{public:enum EDeletionMethod{UI_DELETE_THIS_INSTANCE=1};EDeletionMethod Method;};
class CComponent{public:
 std::map<std::string,CComponent*>Children;long Available,Unavailable,Deleted,Added;CComponent():Available(0),Unavailable(0),Deleted(0),Added(0){}virtual~CComponent(){}
 CComponent*FindChild(CCharString n){std::map<std::string,CComponent*>::iterator i=Children.find(n.Text);return i==Children.end()?0:i->second;}
 CComponent*FindFirstChildOfType(EType t){assert(t==UI_TYPE_TEXT);return FindChild("Text");}
 void MakeAvailableDynamically(){++Available;}void MakeUnavailableDynamically(){++Unavailable;}
 void SetDeletion(CDeletion d){assert(d.Method==CDeletion::UI_DELETE_THIS_INSTANCE);++Deleted;}
 void AddChildDynamically(CCountedPointer<CComponent>&p){assert(p.Data);++Added;}
};
class CText:public CComponent{public:CWideString Value;long Writes;CText():Writes(0){}void SetValue(const CWideString&s,bool a,bool b){assert(a&&b);Value=s;++Writes;}};
class CList:public CComponent{public:long Offsets;CList():Offsets(0){}void InitialiseOffsets(long n){assert(n==1);Offsets=n;}};
class CViewport:public CComponent{};
static std::vector<CComponent*>Created;static long ArrowCreates,ListCreates,Events;
class CManager{public:static CManager*GetInstance(){static CManager m;return &m;}CComponent*CreateComponent(CCharString n,bool f){assert(!f);CComponent*p;if(n.Text=="UI_SUB_LIST"){p=new CList;++ListCreates;}else{assert(n.Text=="UI_SELECTION_ARROW_RIGHT");p=new CComponent;++ArrowCreates;}Created.push_back(p);return p;}void SetEvent(EEvent e){assert(e==UI_EVENT_BACK);++Events;}};
}
using namespace NUISystem;
enum ETCInterfaceType{TCI_SHOP=83,TCI_HERO_STATS=4,FixtureInventory=100};
static bool Special[16],Space[16];static long ShopLookups,HeroLookups,InventoryLookups,PeekLookups,SpaceQueries,SpecialQueries;
class CTCShop{public:long Sales,LastDef,BuyPrice,SellPrice;CTCShop():Sales(0),LastDef(0),BuyPrice(7),SellPrice(9){}long GetCurrentBuyPrice(long d,long n)const{assert(d>0);return BuyPrice*n;}long GetCurrentSellPrice(long d,long n)const{assert(d>0&&n==1);return SellPrice;}void BuyItem(long d){++Sales;LastDef=d;}};
class CTCHeroStats{public:long Money,Added,Calls;CTCHeroStats():Money(0),Added(0),Calls(0){}long GetMoney()const{return Money;}void AddMoney(long n,bool f){assert(f);Money+=n;Added+=n;++Calls;}};
namespace NInventory{class CTCInventoryBase{public:long Removed,Def,Calls;CTCInventoryBase():Removed(0),Def(0),Calls(0){}bool IsSpaceInInventoryForItem(long d)const{++SpaceQueries;return Space[d];}void RemoveItemFromInventory(long d,long n){Removed+=n;Def=d;++Calls;}};}
static CTCShop Shop;static CTCHeroStats Stats;static NInventory::CTCInventoryBase Inventory;
class CThing{public:template<class T>bool GetTC(ETCInterfaceType t,T**p){if(t==TCI_SHOP){*p=(T*)&Shop;++ShopLookups;}else if(t==TCI_HERO_STATS){*p=(T*)&Stats;++HeroLookups;}else{assert(t==FixtureInventory);*p=(T*)&Inventory;++InventoryLookups;}return true;}template<class T>bool PeekTC(ETCInterfaceType t,const T**p)const{assert(t==FixtureInventory);*p=(const T*)&Inventory;++PeekLookups;return true;}};
class CThingPlayerCreature:public CThing{};static CThingPlayerCreature Hero;static CThing Building;
class CPlayer{public:CThingPlayerCreature*GetPControlledCreature()const{return &Hero;}};
class CPlayerManager{public:const CPlayer&GetMainPlayer()const{static CPlayer p;return p;}};
namespace NGameText{class CDataBank{public:const CWideString GetTextBySymbol(const CCharString&s)const{if(s.Text=="TEXT_GUI_MENU_CLOTHING")return CWideString("Clothing");if(s.Text=="TEXT_GUI_MENU_WEAPONS")return CWideString("Weapons");if(s.Text=="TEXT_GUI_MENU_ITEMS")return CWideString("Items");assert(s.Text=="TEXT_GUI_MENU_SELL");return CWideString("Sell");}};}
class CMainGameComponent{public:static CMainGameComponent*Get(){static CMainGameComponent g;return &g;}const NGameText::CDataBank&GetTextBank()const{static NGameText::CDataBank d;return d;}const CPlayerManager&PeekPlayerManager()const{static CPlayerManager m;return m;}};
class CTCInventoryStats{public:static CWideString GetXString(){return CWideString(" x ");}};
class CTCInventoryItem{public:static ETCInterfaceType GetInventoryType(long d){assert(d>0&&d<16);return FixtureInventory;}};
namespace{CCharString CLOTHES_SUBTITLE("Clothes_Subtitle"),WEAPONS_SUBTITLE("Weapons_Subtitle"),ITEMS_SUBTITLE("Items_Subtitle"),SELL_SUBTITLE("Sell_Subtitle");}
typedef std::map<CComponent*,std::pair<long,long> > CComponentPairMap;typedef std::map<CComponent*,CList*> CComponentListMap;
class CTCInventoryTrade{public:
 enum EItem_Menu_Type{MENU_TYPE_CLOTHING=0,MENU_TYPE_WEAPONS=1,MENU_TYPE_ITEMS=2};
 CComponent*Components[1];CComponentPairMap ItemsToBuy,ItemsToSell;CComponentListMap SellListsByCategory,BuyListsByCategory;std::map<CComponent*,std::pair<CWideString,long> >NumSellableThings;
 long TotNumClothesToSell,TotNumWeaponsToSell,TotNumItemsToSell;CComponent*SellMenu,*BuyMenu,*SellBranch,*BuyBranch,*ExpectedViewport;EItem_Menu_Type Type;
 long Subtitles,Names,BuyAdds,AddQuantity,AddDef,AddRefresh,BuyUpdates,SellUpdates,Descriptions,Possibilities,EntryRefreshes;CList*AddedList;
 CTCInventoryTrade():Subtitles(0),Names(0),BuyAdds(0),AddQuantity(0),AddDef(0),AddRefresh(-1),BuyUpdates(0),SellUpdates(0),Descriptions(0),Possibilities(0),EntryRefreshes(0),AddedList(0){}
 CComponent*GetMenuBranchFromDefIndex(long d,CComponent*m,EItem_Menu_Type&t){assert(d>0&&(m==SellMenu||m==BuyMenu));t=Type;return m==SellMenu?SellBranch:BuyBranch;}
 CThing*GetBuildingThing(){return &Building;}bool IsSpecialCase(long d){++SpecialQueries;return Special[d];}
 void RefreshSubtitles(CComponent*p,std::map<CComponent*,std::pair<CWideString,long> >&m){assert(p==SellMenu->FindChild("UI_TRADING_GOODS_LIST")&&&m==&NumSellableThings);++Subtitles;}
 void RefreshNameAndTitle(CComponent*p,long d,long n){assert(p&&d>0&&n>=0);++Names;}
 bool AddBuyItemToList(long d,long n,CList*l,CViewport*v,bool f){assert(l&&v==ExpectedViewport);++BuyAdds;AddQuantity=n;AddDef=d;AddRefresh=f?1:0;AddedList=l;return true;}
 void UpdateBuyActionsList(){++BuyUpdates;}void UpdateSellActionsList(){++SellUpdates;}void RefreshDescriptions(){++Descriptions;}void RefreshBuyPossibilityNumbers(){++Possibilities;}
 std::pair<unsigned long,unsigned long>RefreshEntriesAmountsAndAvailabilities(CComponent*p,CComponentListMap&m){assert((p==BuyMenu&&&m==&BuyListsByCategory)||(p==SellMenu&&&m==&SellListsByCategory));++EntryRefreshes;return std::make_pair(0UL,0UL);}
 void SellItem(CComponent*,long);
};

// Exact production target body follows.
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

static void Caption(const CText&t,const char*head,long n,long writes){char b[96];sprintf(b,"%s x %ld",head,n);assert(t.Value.Text==b&&t.Writes==writes);}
int main(){
 for(int c=0;c<4096;++c){
  CTCInventoryTrade t;CComponent root,sell,buy,sbranch,bbranch,item,arrow,goods,buyentries[4],actions;
  CText branchtext,clothing,weapons,items,clothes_subtitle,weapons_subtitle,items_subtitle,selltext,totaltext,gold;
  CList selllist,buylist;CViewport viewport;
  t.Components[0]=&root;t.SellMenu=&sell;t.BuyMenu=&buy;t.SellBranch=&sbranch;t.BuyBranch=&bbranch;t.ExpectedViewport=&viewport;t.Type=(CTCInventoryTrade::EItem_Menu_Type)(c%3);
  root.Children["UI_TRADING_MENU_SELL_ENTRY"]=&sell;root.Children["UI_TRADING_MENU_BUY_ENTRY"]=&buy;root.Children["UI_SCROLLING_VIEWPORT_TEXT_AREA"]=&viewport;
  const bool with_gold=(c&4)!=0,with_category=(c&8)!=0,with_arrow=(c&16)!=0;
  if(with_gold)root.Children["UI_GOLD_AMOUNT_TRADE_SCREEN"]=&gold;
  sell.Children["UI_TRADING_GOODS_LIST"]=&goods;sbranch.Children["Text"]=&branchtext;if(with_arrow)sbranch.Children["UI_SELECTION_ARROW_RIGHT"]=&arrow;
  if(with_category){sell.Children["UI_TRADING_CLOTHING_TEXT"]=&clothing;sell.Children["UI_TRADING_WEAPONS_TEXT"]=&weapons;sell.Children["UI_TRADING_ITEMS_TEXT"]=&items;}
  sell.Children[CLOTHES_SUBTITLE.Text]=&clothes_subtitle;sell.Children[WEAPONS_SUBTITLE.Text]=&weapons_subtitle;sell.Children[ITEMS_SUBTITLE.Text]=&items_subtitle;sell.Children["UI_TRADING_SELL_TEXT"]=&selltext;sell.Children[SELL_SUBTITLE.Text]=&totaltext;
  long quantity=1+(c%4),stock=quantity+((c&1)?2:0),other=(c&2)?3:0;bool exhausted=stock==quantity,branch_empty=exhausted&&other==0;
  t.ItemsToSell[&item]=std::make_pair(5L,stock);t.SellListsByCategory[&sbranch]=&selllist;t.NumSellableThings[&sbranch]=std::make_pair(CWideString("Branch"),stock+other);
  t.TotNumClothesToSell=41;t.TotNumWeaponsToSell=53;t.TotNumItemsToSell=67;long totals[3]={41,53,67};totals[c%3]-=quantity;
  long old_counts[4],defs[4],avail[4];bool has_actions[4];int mode=c%3;
  for(int k=0;k<16;++k){Special[k]=((c>>(k%9))&1)!=0;Space[k]=((c>>((k+3)%9))&1)!=0;}
  for(int k=0;k<4;++k){defs[k]=k==0&&mode!=2?5:6+k;old_counts[k]=k==0&&mode==1?0:((c>>(k+5))&1)?0:2+k;has_actions[k]=((c>>(k+1))&1)!=0;if(has_actions[k])buyentries[k].Children["UI_LIST_BUY_ACTIONS"]=&actions;t.ItemsToBuy[&buyentries[k]]=std::make_pair(defs[k],old_counts[k]);}
  bool existing_list=(c&32)!=0;if(existing_list)t.BuyListsByCategory[&bbranch]=&buylist;
  Shop=CTCShop();Stats=CTCHeroStats();Inventory=NInventory::CTCInventoryBase();long initial_money=5+(c%31);Stats.Money=initial_money;
  ShopLookups=HeroLookups=InventoryLookups=PeekLookups=SpaceQueries=SpecialQueries=0;ArrowCreates=ListCreates=Events=0;
  long expected_arrows=0,expected_peeks=0,expected_space=0,expected_special=0;
  for(int k=0;k<4;++k){avail[k]=0;if(old_counts[k]>0){++expected_peeks;if(!has_actions[k]&&9<=initial_money+quantity*7){++expected_special;if(!Special[defs[k]])++expected_space;if(Special[defs[k]]||Space[defs[k]]){avail[k]=1;++expected_arrows;}}}}
  if(mode!=2&&old_counts[0]==0){++avail[0];++expected_arrows;}
  t.SellItem(&item,quantity);
  assert(t.TotNumClothesToSell==totals[0]&&t.TotNumWeaponsToSell==totals[1]&&t.TotNumItemsToSell==totals[2]);
  assert(t.NumSellableThings[&sbranch].second==stock+other-quantity);
  if(exhausted)assert(t.ItemsToSell.find(&item)==t.ItemsToSell.end());else assert(t.ItemsToSell[&item].second==stock-quantity);
  assert(item.Deleted==(exhausted?1:0)&&sbranch.Unavailable==(branch_empty?1:0)&&arrow.Deleted==(branch_empty&&with_arrow?1:0));
  assert((t.SellListsByCategory.find(&sbranch)==t.SellListsByCategory.end())==branch_empty);
  if(stock+other-quantity>0)Caption(branchtext,"Branch",stock+other-quantity,1);else assert(branchtext.Value.Text=="Branch"&&branchtext.Writes==1);
  CText*captions[3]={&clothing,&weapons,&items};CText*subtitles[3]={&clothes_subtitle,&weapons_subtitle,&items_subtitle};const char*names[3]={"Clothing","Weapons","Items"};
  for(int k=0;k<3;++k){long writes=with_category&&k==c%3?1:0;assert(captions[k]->Writes==writes&&subtitles[k]->Writes==writes);if(writes){Caption(*captions[k],names[k],totals[k],1);Caption(*subtitles[k],names[k],totals[k],1);}}
  Caption(selltext,"Sell",totals[0]+totals[1]+totals[2],1);Caption(totaltext,"Sell",totals[0]+totals[1]+totals[2],1);
  assert(Shop.Sales==quantity&&Shop.LastDef==5&&Stats.Added==quantity*7&&Stats.Calls==1&&Stats.Money==initial_money+quantity*7);
  assert(Inventory.Removed==quantity&&Inventory.Def==5&&Inventory.Calls==1&&ShopLookups==1&&HeroLookups==1&&InventoryLookups==1&&PeekLookups==expected_peeks&&SpaceQueries==expected_space&&SpecialQueries==expected_special);
  for(int k=0;k<4;++k){assert(buyentries[k].Available==avail[k]&&buyentries[k].Added==avail[k]);assert(t.ItemsToBuy[&buyentries[k]].second==old_counts[k]+(k==0&&mode!=2?quantity:0));}
  assert(ArrowCreates==expected_arrows&&ListCreates==(mode==2&&!existing_list?1:0));
  assert(t.BuyAdds==(mode==2?1:0)&&t.Names==(!exhausted?1:0)+(mode!=2?1:0));
  if(mode==2){assert(t.AddDef==5&&t.AddQuantity==quantity&&t.AddRefresh==(existing_list?1:0));assert(t.BuyListsByCategory[&bbranch]==t.AddedList);if(!existing_list)assert(t.AddedList->Offsets==1&&bbranch.Added==1);else assert(t.AddedList==&buylist&&bbranch.Added==0);}
  assert(t.Subtitles==1&&t.BuyUpdates==1&&t.SellUpdates==1&&t.Descriptions==1&&t.Possibilities==1&&t.EntryRefreshes==2&&Events==1);
  if(with_gold){char b[48];sprintf(b,"%ld",Stats.Money);assert(gold.Value.Text==b&&gold.Writes==1);}else assert(gold.Writes==0);
  for(unsigned i=0;i<Created.size();++i)delete Created[i];Created.clear();
 }
 puts("005aecc0_TEST PASS");return 0;
}