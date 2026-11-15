#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __declspec(x)
#endif
#define FABLE_RETAIL_STLP_LIST
// Fixture-only access seeds valid nonnull empty ranges.
#define protected public
#include "stlport/fable_containers.h"
#undef protected
// Controlled external interfaces: counts use the host's long width; strings retain ASCII test captions.
struct CCharString { const char* Text; CCharString(const char* s,long=-1):Text(s){} };
struct CWideString {
 char Text[128];
 CWideString(){Text[0]=0;}
 CWideString(const char*s){assert(strlen(s)<sizeof Text);strcpy(Text,s);}
 const CWideString&operator+=(const CWideString&s){assert(strlen(Text)+strlen(s.Text)<sizeof Text);strcat(Text,s.Text);return *this;}
};
CWideString FABLE_FASTCALL operator+(const CWideString&a,const CWideString&b){CWideString x(a);x+=b;return x;}
CWideString FABLE_FASTCALL GFIntToWideString(long n){char s[48];sprintf(s,"%ld",n);return CWideString(s);}
enum ETCInterfaceType {TCI_SHOP=83,FixtureInventory=100};
struct Item {long Type;bool Stock,Affordable,Space,Special,Have;};
static Item Items[16];static long ShopQueries=0,InventoryQueries=0,AffordableQueries=0,SpaceQueries=0,SpecialQueries=0,HaveQueries=0,TypeQueries=0;
class CTCShop {public:std::vector<long> Wanted;bool IsValidStockItem(long i)const{return Items[i].Stock;}std::vector<long>GetWantedItemsList()const{return Wanted;}};
namespace NInventory {class CTCInventoryBase {public:bool IsSpaceInInventoryForItem(long i)const{++SpaceQueries;return Items[i].Space;}};}
static CTCShop Shop;static NInventory::CTCInventoryBase Inventory;
class CThing {public:template<class T>bool GetTC(T**p){++ShopQueries;*p=(T*)&Shop;return true;}template<class T>bool PeekTC(ETCInterfaceType,T**p)const{++InventoryQueries;*p=(T*)&Inventory;return true;}};
class CThingPlayerCreature:public CThing{};
static CThing Building;static CThingPlayerCreature Hero;
class CPlayer {public:const CThingPlayerCreature*PeekPControlledCreature()const{return &Hero;}};
class CPlayerManager {public:const CPlayer&GetMainPlayer()const{static CPlayer p;return p;}};
namespace NGameText {class CDataBank {public:const CWideString GetTextBySymbol(const CCharString&s)const{if(strcmp(s.Text,"TEXT_GUI_MENU_SELL")==0)return CWideString("Sell");if(strcmp(s.Text,"TEXT_GUI_MENU_BUY")==0)return CWideString("Buy");assert(strcmp(s.Text,"TEXT_GUI_MENU_WANTED")==0);return CWideString("Wanted");}};}
class CMainGameComponent {public:static CMainGameComponent*Get(){static CMainGameComponent g;return &g;}const NGameText::CDataBank&GetTextBank()const{static NGameText::CDataBank b;return b;}const CPlayerManager&PeekPlayerManager()const{static CPlayerManager m;return m;}};
class CTCInventoryStats {public:static CWideString FABLE_FASTCALL GetXString(){return CWideString(" x ");}};
class CTCInventoryItem {public:static ETCInterfaceType FABLE_FASTCALL GetInventoryType(long i){assert(i>=0&&i<16);return FixtureInventory;}};
namespace NUISystem {
class CComponent {public:virtual~CComponent(){}virtual CComponent*FindChild(CCharString)=0;};
class CText:public CComponent {public:CWideString Value;long Writes;CText():Writes(0){}CComponent*FindChild(CCharString){assert(false);return 0;}virtual void SetValue(const CWideString&s,bool a,bool b){assert(a&&b);++Writes;Value=s;}};
class Entry:public CComponent {public:CText Text;const char*Expected;Entry(const char*s):Expected(s){}CComponent*FindChild(CCharString s){assert(strcmp(s.Text,Expected)==0);return &Text;}};
class CList:public CComponent {public:Entry Sell,Buy,Wanted;CList():Sell("PC_TRADING_SELL_TEXT"),Buy("PC_TRADING_BUY_TEXT"),Wanted("PC_TRADING_WANTED_TEXT"){}CComponent*FindChild(CCharString s){if(strcmp(s.Text,"PC_MENU_ENTRY_TRADE_SELL")==0)return &Sell;if(strcmp(s.Text,"PC_MENU_ENTRY_TRADE_BUY")==0)return &Buy;assert(strcmp(s.Text,"PC_MENU_ENTRY_TRADE_WANTED")==0);return &Wanted;}};
static CList List;
class CManager {public:static CManager*GetInstance(){static CManager m;return &m;}CComponent*CreateComponent(CCharString s,bool b){assert(!b&&strcmp(s.Text,"PC_TOP_LEVEL_TRADE_LIST")==0);return &List;}};
}
template<class F,class T>T FABLE_FASTCALL GFDebugPDynamicCast(F p,T){return static_cast<T>(p);}
class CTCInventoryTrade {public:
 enum EItem_Menu_Type{ITEM_MENU_TYPE_CLOTHES=0,ITEM_MENU_TYPE_WEAPONS=1,ITEM_MENU_TYPE_ITEMS=2};
 std::list<long>NotInStock;
 long TotNumClothesToSell,TotNumWeaponsToSell,TotNumItemsToSell,TotNumClothesToBuy,TotNumWeaponsToBuy,TotNumItemsToBuy,TotNumClothesWanted,TotNumWeaponsWanted,TotNumItemsWanted;
 CThing*GetBuildingThing()const{return &Building;}
 EItem_Menu_Type GetTypeFromDefIndexPC(long i){++TypeQueries;return(EItem_Menu_Type)Items[i].Type;}
 bool IsItemAffordable(long i,long n){assert(n==1);++AffordableQueries;return Items[i].Affordable;}
 bool IsSpecialCase(long i){++SpecialQueries;return Items[i].Special;}
 bool AlreadyHaveSpecial(long i){++HaveQueries;return Items[i].Have;}
 NUISystem::CComponent*ConstructNormalShopListPC(const std::vector<std::pair<long,long> >&,const std::vector<std::pair<long,long> >&);
};
using namespace NUISystem;
using NGameText::CDataBank;

class CTCBase {};
#include "engine/CVectorMap.h"

// Exact original external helper bodies.
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


// Exact complete selected target body.
CComponent* CTCInventoryTrade::ConstructNormalShopListPC(const std::vector<std::pair<long, long> >& buyable,
                                                         const std::vector<std::pair<long, long> >& sellable)
{
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    CManager* pmanager = CManager::GetInstance();
    CComponent* plist_component = pmanager->CreateComponent("PC_TOP_LEVEL_TRADE_LIST", false);
    CList* plist = GFDebugPDynamicCast(plist_component, (CList*)0);
    CComponent* pparent = plist_component->FindChild("PC_MENU_ENTRY_TRADE_SELL");
    // what the pplayer can sell: the stock the shop deals in
    TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;
    if (!sellable.empty()) {
        CTCShop* shop = 0;
        GetBuildingThing()->GetTC(&shop);
        for (std::vector<std::pair<long, long> >::const_iterator it = sellable.begin(); it != sellable.end(); ++it) {
            if (shop->IsValidStockItem(it->first)) {
                switch (GetTypeFromDefIndexPC(it->first)) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToSell += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToSell += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToSell += it->second; break;
                }
            }
        }
    }
    CWideString XString = CTCInventoryStats::GetXString();
    CWideString new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + XString;
    new_title += GFIntToWideString(TotNumWeaponsToSell + TotNumItemsToSell + TotNumClothesToSell);
    static_cast<CText*>(pparent->FindChild("PC_TRADING_SELL_TEXT"))->SetValue(new_title, true, true);
    // what the pplayer can buy: affordable, room in the pplayer's ptc_inventory (or a special not yet owned)
    pparent = plist_component->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyable.empty() || !NotInStock.empty()) {
        CTCShop* pTCShop = 0;
        CThing* pbuilding_thing = GetBuildingThing();
        pbuilding_thing->GetTC(&pTCShop);
        for (std::vector<std::pair<long, long> >::const_iterator it = buyable.begin(); it != buyable.end(); ++it) {
            long item_def = it->first;
            long quantity = it->second;
            const CThingPlayerCreature* pplayer = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
            const NInventory::CTCInventoryBase* ptc_inventory = 0;
            ETCInterfaceType inventory_type = CTCInventoryItem::GetInventoryType(item_def);
            pplayer->PeekTC(inventory_type, &ptc_inventory);
            if (IsItemAffordable(item_def, 1) && (ptc_inventory->IsSpaceInInventoryForItem(item_def) || IsSpecialCase(item_def)) &&
                quantity > 0 && !AlreadyHaveSpecial(item_def)) {
                switch (GetTypeFromDefIndexPC(item_def)) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy += it->second; break;
                }
            }
        }
    }
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + XString;
    new_title += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
    static_cast<CText*>(pparent->FindChild("PC_TRADING_BUY_TEXT"))->SetValue(new_title, true, true);
    // Keep the wanted category totals tied to the inventory screen counters.
    long& clothes_wanted = TotNumClothesWanted;
    long& items_wanted = TotNumItemsWanted;
    // what the shop wants
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(&shop);
    std::vector<long> wanted_list = shop->GetWantedItemsList();
    pparent = plist_component->FindChild("PC_MENU_ENTRY_TRADE_WANTED");
    clothes_wanted = TotNumWeaponsWanted = items_wanted = 0;
    if (!wanted_list.empty()) {
        for (std::vector<long>::iterator it = wanted_list.begin(); it != wanted_list.end(); ++it) {
            switch (GetTypeFromDefIndexPC(*it)) {
            case ITEM_MENU_TYPE_CLOTHES: ++clothes_wanted; break;
            case ITEM_MENU_TYPE_WEAPONS: ++TotNumWeaponsWanted; break;
            case ITEM_MENU_TYPE_ITEMS:   ++items_wanted; break;
            }
        }
    }
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_WANTED") + CTCInventoryStats::GetXString();
    new_title += GFIntToWideString(TotNumWeaponsWanted + items_wanted + clothes_wanted);
    static_cast<CText*>(pparent->FindChild("PC_TRADING_WANTED_TEXT"))->SetValue(new_title, true, true);
    return plist_component;
}

static unsigned RandomState=19;
static unsigned Next(){RandomState=1664525u*RandomState+1013904223u;return RandomState;}
static void CheckCaption(const CText&t,const char*head,long total,long writes){char expected[128];sprintf(expected,"%s x %ld",head,total);assert(strcmp(t.Value.Text,expected)==0&&t.Writes==writes);}
template<class T>static void SeedVector(std::vector<T>&v,unsigned n){assert(!v._M_start);v._M_start=std::allocator<T>().allocate(n);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+n;}
static long CheckMaps(){
 typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Map;
 CTCBase value;long cases=0;
 for(int n=0;n<32;++n){Map m;SeedVector(static_cast<std::vector<Map::value_type>&>(m),32);for(int i=0;i<n;++i)m.push_back(Map::value_type((ETCInterfaceType)(i*3),&value));const Map&cm=m;
  for(int k=0;k<102;++k){ETCInterfaceType key=(ETCInterfaceType)k;int index=0;while(index<n&&index*3<k)++index;Map::iterator a=m.LowerBound(key);Map::const_iterator b=cm.LowerBound(key);assert(a==m.begin()+index&&b==cm.begin()+index);++cases;}
 }
 return cases;
}
int main(){
 SeedVector(Shop.Wanted,32);
 long map_cases=CheckMaps();assert(map_cases==3264);
 for(int c=0;c<4096;++c){
  CTCInventoryTrade trade;std::vector<std::pair<long,long> > buy,sell;SeedVector(buy,32);SeedVector(sell,32);Shop.Wanted.clear();
  if(c&1)trade.NotInStock.push_back(73);
  for(int k=0;k<16;++k){unsigned f=Next();Items[k].Type=(long)(f%4);Items[k].Stock=(f&8)!=0;Items[k].Affordable=(f&16)!=0;Items[k].Space=(f&32)!=0;Items[k].Special=(f&64)!=0;Items[k].Have=(f&128)!=0;}
  int nb=c%7==0?0:(int)(Next()%24),ns=c%11==0?0:(int)(Next()%24),nw=c%13==0?0:(int)(Next()%24);
  for(int k=0;k<nb;++k){long i=(long)(Next()%16),q=(long)(Next()%10)-2;buy.push_back(std::make_pair(i,q));}
  for(int k=0;k<ns;++k){long i=(long)(Next()%16),q=(long)(Next()%10)-2;sell.push_back(std::make_pair(i,q));}
  for(int k=0;k<nw;++k)Shop.Wanted.push_back((long)(Next()%16));
  long expected[9]={0,0,0,0,0,0,0,0,0};long types=nw,space=0,special=0,have=0;
  for(int k=0;k<ns;++k){const Item&t=Items[sell[k].first];if(t.Stock){++types;if(t.Type<3)expected[t.Type]+=sell[k].second;}}
  for(int k=0;k<nb;++k){const Item&t=Items[buy[k].first];if(t.Affordable){++space;if(!t.Space)++special;if((t.Space||t.Special)&&buy[k].second>0){++have;if(!t.Have){++types;if(t.Type<3)expected[3+t.Type]+=buy[k].second;}}}}
  for(int k=0;k<nw;++k){long type=Items[Shop.Wanted[k]].Type;if(type<3)++expected[6+type];}
  trade.TotNumClothesToSell=trade.TotNumWeaponsToSell=trade.TotNumItemsToSell=trade.TotNumClothesToBuy=trade.TotNumWeaponsToBuy=trade.TotNumItemsToBuy=trade.TotNumClothesWanted=trade.TotNumWeaponsWanted=trade.TotNumItemsWanted=-999;
  ShopQueries=InventoryQueries=AffordableQueries=SpaceQueries=SpecialQueries=HaveQueries=TypeQueries=0;
  CComponent*result=trade.ConstructNormalShopListPC(buy,sell);assert(result==&List);
  long actual[9]={trade.TotNumClothesToSell,trade.TotNumWeaponsToSell,trade.TotNumItemsToSell,trade.TotNumClothesToBuy,trade.TotNumWeaponsToBuy,trade.TotNumItemsToBuy,trade.TotNumClothesWanted,trade.TotNumWeaponsWanted,trade.TotNumItemsWanted};
  for(int k=0;k<9;++k)assert(actual[k]==expected[k]);
  CheckCaption(List.Sell.Text,"Sell",expected[0]+expected[1]+expected[2],c+1);CheckCaption(List.Buy.Text,"Buy",expected[3]+expected[4]+expected[5],c+1);CheckCaption(List.Wanted.Text,"Wanted",expected[6]+expected[7]+expected[8],c+1);
  assert(ShopQueries==(ns?1:0)+(nb||!trade.NotInStock.empty()?1:0)+1);assert(InventoryQueries==nb&&AffordableQueries==nb&&SpaceQueries==space&&SpecialQueries==special&&HaveQueries==have&&TypeQueries==types);
 }
 printf("005a6769_TEST PASS\n");return 0;
}