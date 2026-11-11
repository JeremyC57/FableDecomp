#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
namespace std{template<class T>const T& min(const T&a,const T&b){return b<a?b:a;}template<class T>const T&max(const T&a,const T&b){return a<b?b:a;}}
#endif
static void check(bool x){if(!x){std::puts("005f64dd_TEST FAIL");std::exit(1);}}
enum EHeroAbility{HERO_ABILITY_NULL=0,ABILITY1,ABILITY2,ABILITY3};
class CHeroAbilityDef{ public:EHeroAbility Ability;mutable int refs;};
template<class T>class CDefPointer{T*ptr;public:CDefPointer(T*p=0):ptr(p){if(ptr)++const_cast<T*>(ptr)->refs;}CDefPointer(const CDefPointer&other):ptr(other.ptr){if(ptr)++const_cast<T*>(ptr)->refs;}~CDefPointer(){if(ptr)--const_cast<T*>(ptr)->refs;}CDefPointer&operator=(const CDefPointer&other){if(ptr!=other.ptr){if(ptr)--const_cast<T*>(ptr)->refs;ptr=other.ptr;if(ptr)++const_cast<T*>(ptr)->refs;}return *this;}bool IsNull()const{return !ptr;}T*operator->()const{return ptr;}};
class CCharString{ public:const char*value;CCharString(const char*p):value(p){}};
static int sound_init_calls,bank_calls,lookup_calls,play_calls,get_sound_calls,child_calls,def_calls;static long expected_sample;
class CSoundInit{ public:unsigned long GainFaderBits;CSoundInit():GainFaderBits(0){++sound_init_calls;}};
class CASoundBank{ public:long GetSoundIndexFromSymbol(const CCharString&s)const{++lookup_calls;check(!std::strcmp(s.value,"SND_MENU_04"));return expected_sample;}};
static CASoundBank bank;
struct CASoundEmitter{unsigned long PlaySample(CASoundBank&b,long sample,bool loop,const CSoundInit&init,bool attached){++play_calls;check(&b==&bank&&sample==expected_sample&&loop&&init.GainFaderBits==32&&!attached);return 77;}};
struct CASoundManager{CASoundEmitter item;CASoundEmitter&Get2DEmitter(){++get_sound_calls;return item;}};
static CASoundManager*PSoundManager;
class CThing{ public:CASoundBank&GetSampleBank()const{++bank_calls;return bank;}};
class CThingCreatureBase:public CThing{ public:bool child;bool IsChild()const{++child_calls;return child;}};
struct Assign{EHeroAbility ability;long button;};
class CTCInventoryAbilities{public:CThingCreatureBase*parent;long width,CurrentAssignedAbilityLevel;std::vector<EHeroAbility>ReceivedAbilityOrder;CHeroAbilityDef defs[4];std::vector<Assign>calls;int queries;
 const CThing&GetParentThing()const{return *parent;}
 long GetNumAssignableSpecialMoveButtons()const{check(width>0);++const_cast<CTCInventoryAbilities*>(this)->queries;check(queries<2000);return width;}
 CDefPointer<const CHeroAbilityDef>GetAbilityPDef(EHeroAbility a)const{++def_calls;check(int(a)>=0&&int(a)<4);return CDefPointer<const CHeroAbilityDef>(a==HERO_ABILITY_NULL?0:&defs[int(a)]);}
 void SetAbilityToButton(EHeroAbility a,long button,bool flag){check(!flag&&button>=0&&button<width);for(int i=1;i<4;++i)check(defs[i].refs>=1);Assign x;x.ability=a;x.button=button;calls.push_back(x);}
 void GetPrioritisedAbilityDefs(std::vector<CDefPointer<const CHeroAbilityDef> >&out)const;
 void CycleAssignedAbilities(bool forward);
};
void CTCInventoryAbilities::GetPrioritisedAbilityDefs(std::vector<CDefPointer<const CHeroAbilityDef> >& abilities)const
{
 const CThingCreatureBase& creature=static_cast<const CThingCreatureBase&>(GetParentThing());
 if(!creature.IsChild()) {
  long max_size=-1;
  for(long i=0;i<ReceivedAbilityOrder.size();++i) {
   abilities.push_back(GetAbilityPDef(ReceivedAbilityOrder[i]));
   if(ReceivedAbilityOrder[i]!=HERO_ABILITY_NULL)max_size=i;
  }
  std::vector<CDefPointer<const CHeroAbilityDef> > temp_abilities;
  for(long i=0;i<=max_size;++i)temp_abilities.push_back(abilities[i]);
  for(long i=0;i<temp_abilities.size()%GetNumAssignableSpecialMoveButtons();++i)
   temp_abilities.push_back(CDefPointer<const CHeroAbilityDef>());
  abilities=temp_abilities;
 }
}
void CTCInventoryAbilities::CycleAssignedAbilities(bool forward)
{
 std::vector<CDefPointer<const CHeroAbilityDef> > abilities;
 GetPrioritisedAbilityDefs(abilities);
 for(long button_index=0;button_index<GetNumAssignableSpecialMoveButtons();++button_index)
  SetAbilityToButton(HERO_ABILITY_NULL,button_index,false);
 if(abilities.empty())return;
 std::vector<CDefPointer<const CHeroAbilityDef> >::const_iterator it=abilities.begin();
 long original_level=std::max(CurrentAssignedAbilityLevel,-GetNumAssignableSpecialMoveButtons());
 bool valid_set=false;
 do {
  if(forward)CurrentAssignedAbilityLevel+=GetNumAssignableSpecialMoveButtons();
  else CurrentAssignedAbilityLevel-=GetNumAssignableSpecialMoveButtons();
  if(CurrentAssignedAbilityLevel>=(long)abilities.size())CurrentAssignedAbilityLevel=std::min(original_level,0L);
  else if(CurrentAssignedAbilityLevel<0) {
   CurrentAssignedAbilityLevel=abilities.size()-GetNumAssignableSpecialMoveButtons();
   if(CurrentAssignedAbilityLevel<0)CurrentAssignedAbilityLevel=0;
  }
  CurrentAssignedAbilityLevel+=CurrentAssignedAbilityLevel%GetNumAssignableSpecialMoveButtons();
  it=abilities.begin();
  if(CurrentAssignedAbilityLevel>0)it+=CurrentAssignedAbilityLevel;
  long c1=0;
  if(c1<GetNumAssignableSpecialMoveButtons()) {
   std::vector<CDefPointer<const CHeroAbilityDef> >::const_iterator ability_it=it;
   do {
   if(ability_it!=abilities.end()) {
    CDefPointer<const CHeroAbilityDef> pdef=*ability_it;
    if(!pdef.IsNull()){valid_set=true;break;}
   }
   ++c1;++ability_it;
  }while(c1<GetNumAssignableSpecialMoveButtons());
  }
 }while(!valid_set&&CurrentAssignedAbilityLevel!=original_level);
 if(valid_set) {
  for(long button_index=0;it!=abilities.end();++it) {
   CDefPointer<const CHeroAbilityDef> pdef=*it;
   if(!pdef.IsNull())SetAbilityToButton(pdef->Ability,button_index,false);
   else SetAbilityToButton(HERO_ABILITY_NULL,button_index,false);
   if(++button_index==GetNumAssignableSpecialMoveButtons())break;
  }
  if(PSoundManager) {
   CSoundInit init;init.GainFaderBits=0x20;
   CASoundBank& bank=GetParentThing().GetSampleBank();
   long sample=bank.GetSoundIndexFromSymbol("SND_MENU_04");
   PSoundManager->Get2DEmitter().PlaySample(bank,sample,true,init,false);
  }
 }
}

int main(){unsigned cases=0;CASoundManager sound;for(int width=1;width<=4;++width)for(int length=0;length<=8;++length)for(unsigned mask=0;mask<256;++mask)for(int child=0;child<2;++child)for(int enabled=0;enabled<2;++enabled)for(int forward=0;forward<2;++forward){
 CThingCreatureBase parent;parent.child=child!=0;CTCInventoryAbilities q;q.parent=&parent;q.width=width;q.queries=0;for(int j=0;j<4;++j){q.defs[j].Ability=(EHeroAbility)j;q.defs[j].refs=1;}
 for(int i=0;i<length;++i)q.ReceivedAbilityOrder.push_back((mask&(1U<<i))?(EHeroAbility)(i%3+1):HERO_ABILITY_NULL);
 int last=-1;for(int i=0;i<length;++i)if(q.ReceivedAbilityOrder[i])last=i;
 int count=child?0:((last+width)/width)*width;
 // Try every aligned cursor, including the empty sentinel row.
 for(int start=-width;start<(count?count:width);start+=width){
  q.CurrentAssignedAbilityLevel=start;q.calls.clear();q.queries=0;sound_init_calls=bank_calls=lookup_calls=play_calls=get_sound_calls=child_calls=def_calls=0;PSoundManager=enabled?&sound:0;expected_sample=(cases&1)?-1:81;
  int expected_level=start;bool found=false;std::vector<Assign>expected;
  for(int i=0;i<width;++i){Assign a;a.ability=HERO_ABILITY_NULL;a.button=i;expected.push_back(a);}
  if(count){int original=start,steps=0;do{
   expected_level+=forward?width:-width;
   if(expected_level>=count)expected_level=original<0?original:0;
   else if(expected_level<0)expected_level=count-width;
   int first=expected_level>0?expected_level:0;
   for(int i=0;i<width;++i)if(first+i<=last&&q.ReceivedAbilityOrder[first+i])found=true;
   check(++steps<=count/width+2);
  }while(!found&&expected_level!=original);
   if(found){int first=expected_level>0?expected_level:0;for(int i=0;i<width&&first+i<count;++i){Assign a;a.ability=first+i<=last?q.ReceivedAbilityOrder[first+i]:HERO_ABILITY_NULL;a.button=i;expected.push_back(a);}}
  }
  q.CycleAssignedAbilities(forward!=0);
  check(q.CurrentAssignedAbilityLevel==expected_level&&q.calls.size()==expected.size());for(unsigned i=0;i<expected.size();++i)check(q.calls[i].ability==expected[i].ability&&q.calls[i].button==expected[i].button);
  check(child_calls==1&&def_calls==(child?0:length));check(sound_init_calls==int(found&&enabled)&&bank_calls==sound_init_calls&&lookup_calls==sound_init_calls&&get_sound_calls==sound_init_calls&&play_calls==sound_init_calls);
  for(int i=0;i<4;++i)check(q.defs[i].refs==1&&q.defs[i].Ability==i);check(q.ReceivedAbilityOrder.size()==unsigned(length));for(int i=0;i<length;++i)check(q.ReceivedAbilityOrder[i]==((mask&(1U<<i))?(i%3+1):0));++cases;
 }
}check(cases==188072);std::puts("005f64dd_TEST PASS");return 0;}