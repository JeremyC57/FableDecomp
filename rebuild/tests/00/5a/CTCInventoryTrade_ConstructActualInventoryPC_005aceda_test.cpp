
#include <vector>
#include <utility>
#include <string>
#include <stdio.h>
#include <stdlib.h>
static int scenario;
#define ck(v) do { if(!(v)){fprintf(stderr,"case %d line %d\n",scenario,__LINE__);exit(1);} }while(0)
// The release assertion has no observable effect.
#define FABLE_RETAIL_ASSERT_ARM(x) ((void)0)
typedef std::string CCharString;
struct CHairCardDef{}; struct CTattooDef{bool invalid;bool IsInvalidCustomTattoo()const{return invalid;}};
template<class T>struct CDefPointer{T* p;CDefPointer(T* a):p(a){}bool IsNull()const{return !p;}T* Get()const{return p;}T* operator->()const{return p;}};
static CHairCardDef hair;static CTattooDef good,bad;
struct CTCInventoryItem{
 static CDefPointer<const CHairCardDef> GetHairCardDef(long n){return (n&1)?&hair:0;}
 static CDefPointer<const CTattooDef> GetTattooCardDef(long n){return (n&2)?((n&4)?&bad:&good):0;}
};
struct CTCShop{int kind;bool IsBarberShop(){return kind==1;}bool IsTattooShop(){return kind==2;}bool IsTitleShop(){return kind==3;}};
struct Building {
    CTCShop shop;
    bool hasShop;
    int lookups;
    Building() : hasShop(true), lookups(0) {}
    bool GetTC(CTCShop** out)
    {
        ++lookups;
        *out = hasShop ? &shop : 0;
        return hasShop;
    }
};
struct Sound{std::vector<unsigned> groups;void SetGainFader(unsigned g,float x,float y){ck(x==0&&y==500);groups.push_back(g);}};static Sound* PSoundManager;
struct CTCInventoryTrade;
namespace NUISystem { enum EState { UI_STATE_LIST_ACTIVE = 5 };
struct CComponent {
    int steps;
    CComponent** root;
    CComponent* next;

    CComponent() : steps(0), root(0), next(0) {}

    void Initialise()
    {
        ck(steps == 0);
        steps = 1;
        if (root)
            *root = next;
    }

    void ChangeState(int state)
    {
        ck(steps == 1 && state == UI_STATE_LIST_ACTIVE);
        steps = 2;
        if (root)
            *root = next;
    }
};
struct CManager{int calls;CTCInventoryTrade* link;static CManager* GetInstance(){static CManager m;return &m;}void ClearActionParams(){++calls;}void SetInventoryTrade(CTCInventoryTrade* p);};
}
typedef std::vector<std::pair<long,long> > Items;
struct CTCInventoryTrade {
 Building building; NUISystem::CComponent result,*PTop_Level_List; std::vector<NUISystem::CComponent*> Components;
 bool displayEnabled, enabled,valid,IsShoppingValid,nullList;int enabledQueries,highlighted,owned,reset,closed,sellcalls,buycalls,allcalls,normal,special,seed;
 int shopKindOnDisplay;
 CTCInventoryTrade() : shopKindOnDisplay(-1) {}
 Items left,right;
 void ConstructActualInventoryPC();
 void ResetInventoryTradeItems(){++reset;}
 bool GetInventoryShoppingValidity(){return valid;}Building* GetBuildingThing(){return &building;}
 bool GetIsEnabled(){return ++enabledQueries==1 ? enabled : displayEnabled;}void InitialiseSellableItems(){++sellcalls;}
 void populate(Items& x,int start){for(int i=0;i<8;++i)x.push_back(std::make_pair((long)((seed+i)%8),(long)(start+i)));}
 void InitialiseBuyableItems(Items& x){++buycalls;populate(x,10);}
 void GetAllItems(Items& x){++allcalls;populate(x,20+allcalls);}
 int GetHighlightedItem(){return highlighted;}void SetNumberOfSelectedItemOwned(int n){owned=n;}
 NUISystem::CComponent* ConstructSpecialShopListPC(const Items& x,CCharString& name){ck(name=="PC_TOP_LEVEL_BARBER_LIST");++special;left=x;return nullList?0:&result;}
 NUISystem::CComponent* ConstructNormalShopListPC(const Items& x,const Items& y){++normal;left=x;right=y;return nullList?0:&result;}
 void AddCloseInventoryEvent(){++closed;}
};
void NUISystem::CManager::SetInventoryTrade(CTCInventoryTrade* trade)
{
    link = trade;
    if (trade->shopKindOnDisplay >= 0)
        trade->building.shop.kind = trade->shopKindOnDisplay;
}
void CTCInventoryTrade::ConstructActualInventoryPC()
{
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
    }
    ResetInventoryTradeItems();
    IsShoppingValid = GetInventoryShoppingValidity();
    std::vector<std::pair<long, long> > sellable;
    std::vector<std::pair<long, long> > buyable;
    std::vector<std::pair<long, long> > all_items;
    CTCShop* ptc_shop = 0;
    if (!GetBuildingThing()->GetTC(&ptc_shop)) { FABLE_RETAIL_ASSERT_ARM(false); }
    if (GetIsEnabled()) {
        if (!ptc_shop->IsBarberShop() && !ptc_shop->IsTattooShop() && !ptc_shop->IsTitleShop()) {
            InitialiseSellableItems();
            GetAllItems(sellable);
        }
        InitialiseBuyableItems(buyable);
        GetAllItems(all_items);
    }
    long thing_def_index = GetHighlightedItem();
    if (thing_def_index > 0)
        SetNumberOfSelectedItemOwned(thing_def_index);
    if (GetIsEnabled()) {
        NUISystem::CManager::GetInstance()->ClearActionParams();
        NUISystem::CManager::GetInstance()->SetInventoryTrade(this);
        if (ptc_shop->IsBarberShop()) {
            // a barber only deals in hair cards: drop every item without a hair-card def
            if (false) {} else for (std::vector<std::pair<long, long> >::iterator it = all_items.begin(); it != all_items.end(); ) {
                CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(it->first);
                if (phair_card_def.IsNull())
                    it = all_items.erase(it);
                else
                    ++it;
            }
            CCharString name("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(all_items, name);
        } else if (ptc_shop->IsTattooShop()) {
            // a tattooist only deals in valid tattoo cards
            if (false) {} else for (std::vector<std::pair<long, long> >::iterator it = all_items.begin(); it != all_items.end(); ) {
                CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(it->first);
                if (ptattoo_def.IsNull() || ptattoo_def->IsInvalidCustomTattoo())
                    it = all_items.erase(it);
                else
                    ++it;
            }
            CCharString name("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(all_items, name);
        } else if (ptc_shop->IsTitleShop()) {
            CCharString name("PC_TOP_LEVEL_BARBER_LIST");
            PTop_Level_List = ConstructSpecialShopListPC(all_items, name);
        } else {
            // a normal shop leaves the hair and tattoo cards to the special shops
            if (false) {} else for (std::vector<std::pair<long, long> >::iterator it = buyable.begin(); it != buyable.end(); ) {
                if (CTCInventoryItem::GetTattooCardDef(it->first).Get() || CTCInventoryItem::GetHairCardDef(it->first).Get())
                    it = buyable.erase(it);
                else
                    ++it;
            }
            if (false) {} else for (std::vector<std::pair<long, long> >::iterator it = sellable.begin(); it != sellable.end(); ) {
                if (CTCInventoryItem::GetTattooCardDef(it->first).Get() || CTCInventoryItem::GetHairCardDef(it->first).Get())
                    it = sellable.erase(it);
                else
                    ++it;
            }
            PTop_Level_List = ConstructNormalShopListPC(buyable, sellable);
        }
    } else {
        AddCloseInventoryEvent();
    }
    if (PTop_Level_List) {
        PTop_Level_List->Initialise();
        PTop_Level_List->ChangeState(NUISystem::UI_STATE_LIST_ACTIVE);
        Components.push_back(PTop_Level_List);
    }
}

int main(){bad.invalid=true;
for(int display=0; display<2; ++display)
for(int i=0;i<3072;++i){
 scenario=display*3072+i; CTCInventoryTrade t; t.enabledQueries=0; t.displayEnabled=(display!=0); t.reset=t.closed=t.sellcalls=t.buycalls=t.allcalls=t.normal=t.special=0; Sound sound;PSoundManager=(i&1)?&sound:0;
 t.enabled=(i&2)!=0;t.valid=(i&4)!=0;t.nullList=(i&8)!=0;t.PTop_Level_List=(i&16)?&t.result:0;
 bool previous=t.PTop_Level_List!=0;t.building.shop.kind=(i>>5)%4;int kind=t.building.shop.kind;
 t.highlighted=(i>>7)%3-1;t.owned=-2;t.seed=i/384;
 NUISystem::CManager* m=NUISystem::CManager::GetInstance();m->calls=0;m->link=0;
 t.ConstructActualInventoryPC();ck(t.reset==1&&t.IsShoppingValid==t.valid&&t.owned==(t.highlighted>0?t.highlighted:-2));
 ck(sound.groups.size()==(PSoundManager?3u:0u));if(PSoundManager)ck(sound.groups[0]==16&&sound.groups[1]==64&&sound.groups[2]==128);
 ck(t.enabledQueries==2&&t.closed==!t.displayEnabled&&t.sellcalls==(t.enabled&&!kind)&&t.buycalls==t.enabled);
 ck(t.allcalls==(t.enabled?(kind?1:2):0));ck(t.normal==(t.displayEnabled&&!kind)&&t.special==(t.displayEnabled&&kind));
 ck(m->calls==(int)t.displayEnabled&&m->link==(t.displayEnabled?&t:0));
 bool final=t.displayEnabled?!t.nullList:previous;ck(t.result.steps==(final?2:0)&&t.Components.size()==(final?1u:0u));if(final)ck(t.Components[0]==&t.result);
 Items l,r;if(t.enabled&&t.displayEnabled)for(int n=0;n<8;++n){int v=(t.seed+n)%8;bool keep=kind==0?(v%4==0):kind==1?(v%2==1):kind==2?(v==2||v==3):true;if(keep){
  l.push_back(std::make_pair((long)v,(long)((kind?21:10)+n)));if(!kind)r.push_back(std::make_pair((long)v,(long)(21+n)));
 }}ck(l==t.left&&r==t.right);
}
    // Each virtual call can change the field read by the following operation.
    scenario = 6144;
    CTCInventoryTrade trade;
    NUISystem::CComponent stateReceiver, pushedComponent;
    trade.PTop_Level_List = &trade.result;
    trade.result.root = &trade.PTop_Level_List;
    trade.result.next = &stateReceiver;
    stateReceiver.steps = 1;
    stateReceiver.root = &trade.PTop_Level_List;
    stateReceiver.next = &pushedComponent;
    trade.enabled = trade.displayEnabled = trade.valid = false;
    trade.highlighted = 0;
    trade.enabledQueries = trade.reset = trade.closed = 0;
    PSoundManager = 0;
    trade.ConstructActualInventoryPC();
    ck(trade.result.steps == 1 && stateReceiver.steps == 2 && pushedComponent.steps == 0);
    ck(trade.PTop_Level_List == &pushedComponent);
    ck(trade.Components.size() == 1 && trade.Components[0] == &pushedComponent);
    scenario = 6145;
    CTCInventoryTrade missing;
    missing.building.hasShop = false;
    missing.enabled = missing.displayEnabled = false;
    missing.valid = true;
    missing.highlighted = 0;
    missing.enabledQueries = missing.reset = missing.closed = 0;
    missing.PTop_Level_List = 0;
    missing.ConstructActualInventoryPC();
    ck(missing.building.lookups == 1 && missing.IsShoppingValid);
    ck(missing.reset == 1 && missing.enabledQueries == 2 && missing.closed == 1);
    ck(!missing.PTop_Level_List && missing.Components.empty());

    // The manager callback can change the shop type before the display query.
    scenario = 6146;
    CTCInventoryTrade changed;
    changed.enabled = changed.displayEnabled = changed.valid = true;
    changed.nullList = false;
    changed.PTop_Level_List = 0;
    changed.building.shop.kind = 0;
    changed.shopKindOnDisplay = 2;
    changed.seed = changed.highlighted = 0;
    changed.reset = changed.closed = changed.enabledQueries = 0;
    changed.sellcalls = changed.buycalls = changed.allcalls = 0;
    changed.normal = changed.special = 0;
    changed.ConstructActualInventoryPC();
    ck(changed.building.lookups == 1 && changed.sellcalls == 1 && changed.buycalls == 1);
    ck(changed.allcalls == 2 && changed.special == 1 && changed.normal == 0);
    ck(changed.left.size() == 2);
    ck(changed.left[0] == std::make_pair(2L, 24L));
    ck(changed.left[1] == std::make_pair(3L, 25L));
    ck(changed.PTop_Level_List == &changed.result && changed.result.steps == 2);
    puts("005aceda_TEST PASS");
    return 0;
}