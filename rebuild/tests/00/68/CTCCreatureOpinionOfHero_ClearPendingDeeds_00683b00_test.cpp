// Selected caller body copied verbatim, with observable services and actual standard lists.
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <map>
#include <vector>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
static int managerCalls,playerCalls,creatureCalls,tcCalls;
static std::vector<long> lookups;
class COpinionDeed {public:int DeedType;long DeedHandle;};
class CTCHeroOpinionDeedLog {
public:std::map<long,COpinionDeed> Deeds;
 const COpinionDeed* GetDeedFromHandle(long handle)const{
  CHECK(tcCalls==1);lookups.push_back(handle);
  std::map<long,COpinionDeed>::const_iterator i=Deeds.find(handle);
  return i==Deeds.end()?0:&i->second;
 }
};
class CThingPlayerCreature {
public:CTCHeroOpinionDeedLog* Log;
 bool GetTC(CTCHeroOpinionDeedLog** out){CHECK(creatureCalls==1);++tcCalls;*out=Log;return true;}
};
class CPlayer {public:CThingPlayerCreature* Controlled;CThingPlayerCreature* GetPControlledCreature()const{CHECK(playerCalls==1);++creatureCalls;return Controlled;}};
class CPlayerManager {public:CPlayer Player;CPlayer& GetMainPlayer(){CHECK(managerCalls==1);++playerCalls;return Player;}};
class CTCBase {public:CPlayerManager* Manager;protected:CPlayerManager& GetPlayerManager(){++managerCalls;return *Manager;}};
class CTCCreatureOpinionOfHero:public CTCBase {public:std::list<long> PendingDeeds;std::list<long> KnownDeeds[92];void ClearPendingDeeds();};
void CTCCreatureOpinionOfHero::ClearPendingDeeds()
{
 const COpinionDeed* pdeed=0;
 CThingPlayerCreature* player=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 if(player){
  CTCHeroOpinionDeedLog* ptc_deedlog;
  player->GetTC(&ptc_deedlog);
  while(!PendingDeeds.empty()){
   pdeed=ptc_deedlog->GetDeedFromHandle(*PendingDeeds.rbegin());
   if(pdeed)KnownDeeds[pdeed->DeedType].push_front(pdeed->DeedHandle);
   PendingDeeds.pop_back();
  }
 }else{
  PendingDeeds.clear();
 }
}

int main(){
 int cases=0;
 for(int hero=0;hero<2;++hero)for(int length=0;length<9;++length)for(int pattern=0;pattern<6;++pattern){
  CTCHeroOpinionDeedLog log;
  const int kinds[]={0,1,91,1};
  for(int k=0;k<4;++k){COpinionDeed deed;deed.DeedType=kinds[k];deed.DeedHandle=500+k;log.Deeds.insert(std::make_pair(100+k,deed));}
  CThingPlayerCreature creature;creature.Log=&log;
  CPlayerManager manager;manager.Player.Controlled=hero?&creature:0;
  CTCCreatureOpinionOfHero opinion;opinion.Manager=&manager;
  std::vector<long> pending;std::list<long> expected[92];
  for(int t=0;t<92;++t){opinion.KnownDeeds[t].push_back(9000+t);expected[t].push_back(9000+t);}
  for(int n=0;n<length;++n){long key=100+(pattern+n*3+n/2)%6;pending.push_back(key);opinion.PendingDeeds.push_back(key);}
  if(hero)for(int n=length;n>0;--n){long key=pending[n-1];if(key<104){const COpinionDeed& deed=log.Deeds.find(key)->second;expected[deed.DeedType].push_front(deed.DeedHandle);}}
  for(int repeat=0;repeat<2;++repeat){
   managerCalls=playerCalls=creatureCalls=tcCalls=0;lookups.clear();
   opinion.ClearPendingDeeds();++cases;
   CHECK(managerCalls==1 && playerCalls==1 && creatureCalls==1 && tcCalls==hero);
   CHECK(opinion.PendingDeeds.empty());
   CHECK(lookups.size()==(unsigned)(hero && !repeat?length:0));
   for(unsigned n=0;n<lookups.size();++n)CHECK(lookups[n]==pending[length-1-n]);
   for(int t=0;t<92;++t)CHECK(opinion.KnownDeeds[t]==expected[t]);
   CHECK(log.Deeds.size()==4 && creature.Log==&log && opinion.Manager==&manager);
  }
 }
 CHECK(cases==216);puts("00683b00_TEST PASS");return 0;
}