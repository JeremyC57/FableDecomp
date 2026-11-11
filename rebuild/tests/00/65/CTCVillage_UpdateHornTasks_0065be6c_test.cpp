#include <stdio.h>
#include <list>
#include <vector>
#include <functional>
#include <algorithm>
static bool failed;static int line_failed;
#define CHECK(x) do{if(!(x)){failed=true;line_failed=__LINE__;return;}}while(0)
class CThing{ public:int id,refs;};
template<class T>class CIntelligentPointer{ public:T*p;CIntelligentPointer(T*q=0):p(q){if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;}T*Get()const{return p;}T&operator*()const{return *p;}};
static int created,destroyed,predicate_live;static std::vector<int>removed;
class CVillageTaskHorn{ public:CIntelligentPointer<CThing>PHorn;CVillageTaskHorn(CThing&t):PHorn(&t){++created;}~CVillageTaskHorn(){++destroyed;}CThing*GetHorn()const{return PHorn.Get();}};
template<class T>class CCountedPointer{ public:T*Data;int*Info;CCountedPointer(T*p=0):Data(p),Info(p?new int(1):0){}CCountedPointer(const CCountedPointer&o):Data(o.Data),Info(o.Info){if(Info)++*Info;}~CCountedPointer(){if(Info&&!--*Info){delete Data;delete Info;}}T*operator->()const{return Data;}};
class CIsHornTaskWithThisHorn:public std::unary_function<CCountedPointer<CVillageTaskHorn>,bool>{ public:CIntelligentPointer<CThing>PHorn;CIsHornTaskWithThisHorn(CIntelligentPointer<CThing>p):PHorn(p){++predicate_live;}CIsHornTaskWithThisHorn(const CIsHornTaskWithThisHorn&o):PHorn(o.PHorn){++predicate_live;}~CIsHornTaskWithThisHorn(){--predicate_live;}bool operator()(CCountedPointer<CVillageTaskHorn>p)const{return p->GetHorn()==PHorn.Get();}};
class CVillageDef{ public:mutable int refs;long CurfewStartTime,CurfewFinishTime;};
template<class T>class CDefPointer{ public:T*Object;CDefPointer(T*p=0):Object(p){if(Object)++Object->refs;}CDefPointer(const CDefPointer&o):Object(o.Object){if(Object)++Object->refs;}~CDefPointer(){if(Object)--Object->refs;}T*operator->()const{return Object;}};
static bool dependency_ok;static int time_calls;class CGameTimeManager{ public:bool curfew;const CVillageDef*def;bool IsTimeBetween(long start,long end)const{++time_calls;dependency_ok=dependency_ok&&start==def->CurfewStartTime&&end==def->CurfewFinishTime&&def->refs==3;return curfew;}};
class CWorld{ public:CGameTimeManager*manager;const CGameTimeManager&PeekGameTimeManager()const{return *manager;}};
class CTCVillage{ public:std::list<CIntelligentPointer<CThing> >VillageHorns;std::list<CCountedPointer<CVillageTaskHorn> >HornTasks;bool HornTasksAdded;CDefPointer<const CVillageDef>PDef;CWorld*world;CDefPointer<const CVillageDef>GetPDef()const{return PDef;}const CWorld&PeekWorld()const{return *world;}void RemoveThingFromAllTasks(const CThing&t){removed.push_back(t.id);for(std::list<CCountedPointer<CVillageTaskHorn> >::iterator i=HornTasks.begin();i!=HornTasks.end();){if(i->Data->GetHorn()==&t)i=HornTasks.erase(i);else ++i;}}void UpdateHornTasks();};
void CTCVillage::UpdateHornTasks()
{
 if(PeekWorld().PeekGameTimeManager().IsTimeBetween(GetPDef()->CurfewStartTime,GetPDef()->CurfewFinishTime)) {
  if(!HornTasksAdded) {
   for(std::list<CIntelligentPointer<CThing> >::iterator horn_it=VillageHorns.begin();horn_it!=VillageHorns.end();++horn_it) {
    CIntelligentPointer<CThing>&p_horn=*horn_it;
    std::list<CCountedPointer<CVillageTaskHorn> >::const_iterator task_it=std::find_if(HornTasks.begin(),HornTasks.end(),CIsHornTaskWithThisHorn(p_horn));
    if(task_it==HornTasks.end())HornTasks.push_back(CCountedPointer<CVillageTaskHorn>(new CVillageTaskHorn(*p_horn)));
   }
   HornTasksAdded=true;
  }
 } else if(HornTasksAdded) {
  for(std::list<CIntelligentPointer<CThing> >::iterator horn_it=VillageHorns.begin();horn_it!=VillageHorns.end();++horn_it)
   RemoveThingFromAllTasks(*horn_it->Get());
  HornTasksAdded=false;
 }
}

static void one(int curfew,int added,int count,int profile,int mask)
{
 CThing things[4];for(int i=0;i<4;++i){things[i].id=i;things[i].refs=0;}created=destroyed=predicate_live=time_calls=0;dependency_ok=true;removed.clear();
 std::vector<int>horns,expected,expected_removed;for(int i=0;i<4;++i)if(mask&(1<<i))expected.push_back(i);int code=profile;for(int i=0;i<count;++i){horns.push_back(code%3);code/=3;}
 if(curfew&&!added){for(unsigned h=0;h<horns.size();++h){bool found=false;for(unsigned t=0;t<expected.size();++t)if(expected[t]==horns[h])found=true;if(!found)expected.push_back(horns[h]);}}
 if(!curfew&&added){for(unsigned h=0;h<horns.size();++h){expected_removed.push_back(horns[h]);for(unsigned t=0;t<expected.size();){if(expected[t]==horns[h])expected.erase(expected.begin()+t);else ++t;}}}
 CVillageDef def;def.refs=0;def.CurfewStartTime=2200;def.CurfewFinishTime=600;CGameTimeManager manager;manager.curfew=curfew!=0;manager.def=&def;CWorld world;world.manager=&manager;
 {
 CTCVillage village;village.HornTasksAdded=added!=0;village.world=&world;village.PDef.Object=&def;++def.refs;for(unsigned h=0;h<horns.size();++h)village.VillageHorns.push_back(CIntelligentPointer<CThing>(&things[horns[h]]));for(int i=0;i<4;++i)if(mask&(1<<i))village.HornTasks.push_back(CCountedPointer<CVillageTaskHorn>(new CVillageTaskHorn(things[i])));
 for(int repeat=0;repeat<2;++repeat){village.UpdateHornTasks();CHECK(dependency_ok&&time_calls==repeat+1&&def.refs==1&&predicate_live==0);CHECK(village.HornTasksAdded==bool(curfew));CHECK(village.HornTasks.size()==expected.size());unsigned n=0;for(std::list<CCountedPointer<CVillageTaskHorn> >::const_iterator t=village.HornTasks.begin();t!=village.HornTasks.end();++t,++n){CHECK(t->Data->GetHorn()->id==expected[n]);CHECK(t->Info&&*t->Info==1);}CHECK(removed==expected_removed);CHECK(village.VillageHorns.size()==horns.size());for(int i=0;i<4;++i){int refs=0;for(unsigned h=0;h<horns.size();++h)if(horns[h]==i)++refs;for(unsigned t=0;t<expected.size();++t)if(expected[t]==i)++refs;CHECK(things[i].refs==refs);}}
 }
 CHECK(def.refs==0&&created==destroyed&&predicate_live==0);for(int i=0;i<4;++i)CHECK(things[i].refs==0);
}
int main(){unsigned long cases=0;for(int c=0;c<2;++c)for(int a=0;a<2;++a)for(int n=0;n<5;++n)for(int p=0;p<81;++p)for(int m=0;m<16;++m){one(c,a,n,p,m);if(failed){printf("0065be6c_TEST FAIL line%d case%lu\n",line_failed,cases);return 1;}++cases;}if(cases!=25920)return 2;puts("0065be6c_TEST PASS");return 0;}