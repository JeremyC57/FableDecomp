#include <cstdio>
#include <list>
class CCharString { public:long value;explicit CCharString(long v):value(v){};};
class CDefString { public:long value;operator CCharString() const{return CCharString(value);}};
struct Ref {mutable int count;mutable int deleted;Ref():count(1),deleted(0){};void Release()const{if(--count==0)++deleted;}};
template<class T>class CDefPointer {public:T* Object;CDefPointer():Object(0){}~CDefPointer(){if(Object)Object->Release();}T* operator->()const{return Object;}};
class CEnemyDef:public Ref { public:long Faction;};
static CEnemyDef enemydef;static int has_enemy;
class CThingBaseDef:public Ref { public:bool GetSubDef(CDefPointer<const CEnemyDef>* out)const{if(!has_enemy)return false;out->Object=&enemydef;return true;}};
static CThingBaseDef thingdef;static int base_calls,def_calls,sub_calls,name_calls,faction_calls;static unsigned long requested_index;static long requested_faction;
class CGameDefinitionManager { public:template<class T>bool GetDef(long index,CDefPointer<const T>* out)const{++def_calls;requested_index=(unsigned long)index;out->Object=&thingdef;return true;}CDefString GetDefNameFromGlobalIndex(long i)const{++name_calls;requested_faction=i;CDefString s;s.value=i;return s;}};
static CGameDefinitionManager manager;
class CFaction { public:long index;};static CFaction faction;static int found_faction;
class CFactionManager { public:CFaction* GetPFaction(const CCharString& name)const{++faction_calls;faction.index=name.value;return found_faction?&faction:0;}};
class CWorld { public:CFactionManager factions;const CFactionManager& GetFactionManager()const{return factions;}};static CWorld world;
class CThing { public:unsigned short DefGlobalIndex;unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}};
class CTCBase {public:CThing* ParentThing;explicit CTCBase(CThing& p):ParentThing(&p){++base_calls;}const CGameDefinitionManager& GetDefinitionManager()const{return manager;}const CWorld& PeekWorld()const{return world;}};
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;std::list<int> EnemyThings,AllyThings;const CFaction* PFaction;explicit CTCEnemy(CThing& thing);void SetFaction(const CCharString&);};
void CTCEnemy::SetFaction(const CCharString& faction)
{
    PFaction=PeekWorld().GetFactionManager().GetPFaction(faction);
}
CTCEnemy::CTCEnemy(CThing& thing)
    : CTCBase(thing),FriendsWithEverythingFlag(false),EnableFollowersEnemyProxy(true),PFaction(0)
{
    CDefPointer<const CThingBaseDef> pthing_def;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(),&pthing_def);
    CDefPointer<const CEnemyDef> penemy_def;
    if(pthing_def->GetSubDef(&penemy_def))
        SetFaction(GetDefinitionManager().GetDefNameFromGlobalIndex(penemy_def->Faction));
}

int main(){int ok=1,cases=0;for(int present=0;present<2;++present)for(int found=0;found<2;++found)for(int refs=1;refs<=3;++refs)for(int index=0;index<4;++index){has_enemy=present;found_faction=found;thingdef.count=refs;thingdef.deleted=0;enemydef.count=refs;enemydef.deleted=0;enemydef.Faction=-2+index*17;CThing thing;thing.DefGlobalIndex=(unsigned short)(index==3?65535:index*7+1);int old_base=base_calls,old_def=def_calls,old_name=name_calls,old_faction=faction_calls;{CTCEnemy tc(thing);ok &= tc.ParentThing==&thing&&!tc.FriendsWithEverythingFlag&&tc.EnableFollowersEnemyProxy;ok &= tc.EnemyThings.empty()&&tc.AllyThings.empty();ok &= base_calls==old_base+1&&def_calls==old_def+1&&requested_index==thing.DefGlobalIndex;ok &= name_calls==old_name+present&&faction_calls==old_faction+present;ok &= tc.PFaction==(present&&found?&faction:0);if(present)ok &= requested_faction==enemydef.Faction&&faction.index==enemydef.Faction;ok &= thingdef.count==refs-1&&thingdef.deleted==(refs==1);ok &= enemydef.count==refs-present&&enemydef.deleted==(present&&refs==1);}++cases;}ok &= cases==48;std::printf(ok?"0076c8a0_TEST PASS\n":"0076c8a0_TEST FAIL\n");return ok?0:1;}