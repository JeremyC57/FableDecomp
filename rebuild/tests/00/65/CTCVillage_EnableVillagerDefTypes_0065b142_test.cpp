// Selected target and alive-template bodies verbatim; external game services are test boundaries.
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_FASTCALL
static int cases_run=0;
static void check(bool b){if(!b){fprintf(stderr,"case %d failed\n",cases_run);abort();}}
class CCharString {public:const char* text;explicit CCharString(const char* s):text(s){} };
class CDefIndex {long Val;public:CDefIndex(long v):Val(v){}operator long()const{return Val;} };
class C3DVector { public:float X,Y,Z;};
enum ETCInterfaceType {TCI_NAVIGATION=7,TCI_DUMMY_VILLAGER=214};
class CTCBase {public:virtual ~CTCBase(){} };
class CTCDummyVillager;class CTCCreatureNavigation;
class CThing {
public:
 bool alive,has_dummy,has_nav;CTCBase* dummy;CTCBase* nav;
 C3DVector pos;int killed,position_reads;
 CThing():alive(true),has_dummy(false),has_nav(false),dummy(0),nav(0),killed(0),position_reads(0) {pos.X=pos.Y=pos.Z=0;}
 bool IsDead()const{return !alive;}
 bool HasTC(ETCInterfaceType type)const{return type==TCI_DUMMY_VILLAGER?has_dummy:has_nav;}
 CTCBase& GetTC(ETCInterfaceType type){CTCBase* p=type==TCI_DUMMY_VILLAGER?dummy:nav;check(p!=0);return *p;}
 const C3DVector& GetPos(){++position_reads;return pos;}
 void Kill(bool silent){check(!silent);++killed;alive=false;}
};
class CThingAICreature:public CThing {};
template<class T>class CIntelligentPointer {T* p;public:explicit CIntelligentPointer(T* v):p(v){}T* Get()const{return p;} };
class CTCVillage {
public:CThing* ParentThing;std::list<CIntelligentPointer<CThing> > VillageDummies;int parent_reads;
 CTCVillage():ParentThing(0),parent_reads(0){}
 CThing& GetParentThing(){++parent_reads;return *ParentThing;}
 void EnableVillagerDefTypes(bool,const CCharString&);
};
class CTCDummyVillager:public CTCBase {
public:unsigned long type;bool respawnable;CThingAICreature* creature;CThingAICreature created;int enabled_calls,create_calls;C3DVector created_at;
 CTCDummyVillager():type(0),respawnable(false),creature(0),enabled_calls(0),create_calls(0){}
 unsigned long GetDefType(){return type;}
 CThingAICreature* GetCreature()const{return creature;}
 void SetRespawnable(bool value){++enabled_calls;respawnable=value;}
 CThingAICreature* CreateVillager(CTCVillage& village,const C3DVector& pos,bool stop){
  check(village.ParentThing!=0 && !stop);++create_calls;created_at=pos;created.alive=true;creature=&created;return creature;
 }
};
class CTCCreatureNavigation:public CTCBase {
public:mutable int calls;CTCCreatureNavigation():calls(0){}
 bool GetNearbyWanderPosition(const C3DVector& center,float radius,C3DVector& out,bool on_map)const {
  check(radius==50 && on_map);++calls;out=center;out.X+=radius;out.Y-=radius;out.Z+=0.5f;return true;
 }
};
class CGameDefinitionManager {
public:static int calls;static CGameDefinitionManager& Get(){static CGameDefinitionManager manager;return manager;}
 long GetDefGlobalIndexFromName(const CCharString& s)const{++calls;return strcmp(s.text,"first")==0?17:29;}
};int CGameDefinitionManager::calls=0;
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 if(thing!=0) { if(!thing->IsDead())return true; }
 return false;
}
void CTCVillage::EnableVillagerDefTypes(bool enable,const CCharString& def_name) {
 CDefIndex def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName(def_name);
 for(std::list<CIntelligentPointer<CThing> >::iterator i=VillageDummies.begin();i!=VillageDummies.end();++i) {
  CThing* dummy=i->Get();
  if(IS_THING_ALIVE(dummy) && dummy->HasTC(TCI_DUMMY_VILLAGER)) {
   CTCDummyVillager* dummy_villager=static_cast<CTCDummyVillager*>(&dummy->GetTC(TCI_DUMMY_VILLAGER));
   if(dummy_villager->GetDefType()==def_index) {
    dummy_villager->SetRespawnable(enable);
    if(!enable) {
     if(IS_THING_ALIVE(dummy_villager->GetCreature()))dummy_villager->GetCreature()->Kill(false);
    } else if(!IS_THING_ALIVE(dummy_villager->GetCreature())) {
     C3DVector creation_pos;
     if(dummy->HasTC(TCI_NAVIGATION)) {
      CTCCreatureNavigation* tc_creature_navigation=static_cast<CTCCreatureNavigation*>(&dummy->GetTC(TCI_NAVIGATION));
      tc_creature_navigation->GetNearbyWanderPosition(GetParentThing().GetPos(),50.0f,creation_pos,true);
     } else creation_pos=ParentThing->GetPos();
     dummy_villager->CreateVillager(*this,creation_pos,false);
    }
   }
  }
 }
}

static void run(int count,int config,bool enable,bool second) {
 ++cases_run;CGameDefinitionManager::calls=0;
 CTCVillage village;CThing owner;owner.pos.X=1;owner.pos.Y=2;owner.pos.Z=3;village.ParentThing=&owner;
 CThing dummies[4];CThingAICreature creatures[4];CTCDummyVillager components[4];CTCCreatureNavigation navigation[4];int settings[4];
 for(int i=0;i<count;++i) {
  const int bits=(config+i*13)&63;settings[i]=bits;
  CThing& d=dummies[i];d.alive=(bits&1)!=0;d.has_dummy=(bits&2)!=0;d.has_nav=(bits&8)!=0;d.dummy=&components[i];d.nav=&navigation[i];
  d.pos.X=100+i;d.pos.Y=200+i;d.pos.Z=300+i;
  components[i].type=(bits&4)?17:29;components[i].respawnable=!enable;
  creatures[i].alive=(bits&32)!=0;components[i].creature=(bits&16)?&creatures[i]:0;
  village.VillageDummies.push_back(CIntelligentPointer<CThing>(&d));
 }
 village.EnableVillagerDefTypes(enable,CCharString(second?"second":"first"));
 check(CGameDefinitionManager::calls==1);int expected_positions=0,expected_parent=0;
 for(int i=0;i<count;++i) {
  const int b=settings[i];const bool match=(b&1)&&(b&2)&&components[i].type==(second?29u:17u);
  const bool live=(b&16)&&(b&32);const bool create=match&&enable&&!live;const bool kill=match&&!enable&&live;
  check(components[i].enabled_calls==(match?1:0));check(components[i].respawnable==(match?enable:!enable));
  check(components[i].create_calls==(create?1:0));check(creatures[i].killed==(kill?1:0));check(creatures[i].alive==(((b&32)!=0)&&!kill));
  check(navigation[i].calls==(create&&(b&8)?1:0));check(dummies[i].position_reads==0);
  if(create){++expected_positions;if(b&8)++expected_parent;check(components[i].creature==&components[i].created);
   const C3DVector& p=components[i].created_at;check(p.X==(b&8?51.0f:1.0f)&&p.Y==(b&8?-48.0f:2.0f)&&p.Z==(b&8?3.5f:3.0f));
  }else check(components[i].creature==((b&16)?&creatures[i]:0));
 }
 check(owner.position_reads==expected_positions);check(village.parent_reads==expected_parent);
}
int main(){for(int n=0;n<=4;++n)for(int c=0;c<64;++c)for(int en=0;en<2;++en)for(int def=0;def<2;++def)run(n,c,en!=0,def!=0);
 check(cases_run==1280);puts("0065b142_TEST PASS");return 0;}