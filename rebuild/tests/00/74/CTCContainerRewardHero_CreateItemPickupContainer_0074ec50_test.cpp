#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <utility>
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "fable_assert.h"
namespace RewardFixture {
typedef std::string CCharString;
void check(bool b){if(!b)exit(1);}
struct CTCContainerRewardHero;struct CTCGold;
struct Case {
 long amount,chestID,goldID;int n,itemCreates,goldCreates,lookups,itemReads,goldReads,goldSet;bool itemMade,goldMade,containerPresent,goldPresent;
 std::vector<std::pair<long,CCharString> > inserted;
 Case():amount(0),chestID(7),goldID(8),n(0),itemCreates(0),goldCreates(0),lookups(0),itemReads(0),goldReads(0),goldSet(0),itemMade(true),goldMade(true),containerPresent(true),goldPresent(true){}
}*st;
struct Parent{C3DVector GetPos() const{return C3DVector(4,16,-2);}};
struct Definitions{long GetDefGlobalIndexFromName(const CCharString& name)const{
 ++st->lookups;if(name=="OBJECT_CHEST_REWARD_ON_DEATH")return st->chestID;
 check(name=="OBJECT_GOLDBAG_SMALL_WITH_COINS");return st->goldID;
}};
struct CTCGold{void SetGold(long x){check(x==st->amount);++st->goldSet;}};
struct CThingObject{
 CTCContainerRewardHero* c;CTCGold g;
 bool GetTC(CTCContainerRewardHero** out){++st->itemReads;*out=st->containerPresent?c:0;return *out!=0;}
 bool GetTC(CTCGold** out){++st->goldReads;*out=st->goldPresent?&g:0;return *out!=0;}
};
struct CTCContainerRewardHero{
 Definitions definitions;Parent parent;CThingObject obj;CTCContainerRewardHero(){obj.c=this;}
 void GetRewardItems(std::vector<std::pair<long,CCharString> >& list,long& gold){
  for(int i=0;i<st->n;++i)list.push_back(std::make_pair(long(31+i),CCharString(i==0?"first":"second")));
  gold=st->amount;
 }
 Definitions& GetDefinitionManager(){return definitions;}
 Parent& GetParentThing(){return parent;}
 CThingObject* CreateNewObject(long id,const C3DVector& p,const C3DVector& v,const CCharString& name){
  check(p.X==4&&p.Y==16&&p.Z==-2&&v.X==0&&v.Y==0.25f&&v.Z==0.25f&&name.empty());
  if(id==7){++st->itemCreates;return st->itemMade?&obj:0;}
  check(id==8);++st->goldCreates;return st->goldMade?&obj:0;
 }
 void InsertItem(long key,const CCharString& name){st->inserted.push_back(std::make_pair(key,name));}
 void CreateItemPickupContainer();
};
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

void cases(){
 for(int ni=0;ni<3;++ni)for(int amount=-1;amount<=1;++amount)for(int ci=0;ci<3;++ci)for(int gi=0;gi<3;++gi)
 for(int flags=0;flags<16;++flags){
  Case s;st=&s;s.n=ni;s.amount=amount*19;s.chestID=ci==2?7:ci-1;s.goldID=gi==2?8:gi-1;
  s.itemMade=(flags&1)!=0;s.goldMade=(flags&2)!=0;s.containerPresent=(flags&4)!=0;s.goldPresent=(flags&8)!=0;
  CTCContainerRewardHero reward;reward.CreateItemPickupContainer();
  int wantedChest=ni>0&&ci==2,wantedGold=amount>0&&gi==2;
  check(s.lookups==(ni>0)+(amount>0));check(s.itemCreates==wantedChest&&s.goldCreates==wantedGold);
  check(s.itemReads==wantedChest*int(s.itemMade)&&s.goldReads==wantedGold*int(s.goldMade));
  check(s.goldSet==s.goldReads*int(s.goldPresent));
  check(s.inserted.size()==(unsigned)(ni*s.itemReads*int(s.containerPresent)));
  for(unsigned int i=0;i<s.inserted.size();++i)check(s.inserted[i].first==long(31+i)&&s.inserted[i].second==(i==0?"first":"second"));
 }
}
}
int main(){RewardFixture::cases();puts("0074ec50_TEST PASS");return 0;}