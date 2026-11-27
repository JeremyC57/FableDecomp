#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Check(c) do{if(!(c)){printf("failed %d case %u\n",__LINE__,caseId);exit(1);}}while(0)
static unsigned caseId;
struct C3DVector {};
struct CCharString {const char* p;CCharString(const char* x=""):p(x){}};
struct CDefIndex {long v;CDefIndex(long n=0):v(n){}operator long()const{return v;}};
template<class T> struct CArray {T buf[8];unsigned count;CArray():count(0){}void push_back(const T& v){Check(count<8);buf[count++]=v;}T& operator[](unsigned i){Check(i<count);return buf[i];}const T& operator[](unsigned i)const{Check(i<count);return buf[i];}long size()const{return (long)count;} bool empty()const{return !count;}};
template<class T> struct CDefPointer {T* p;CDefPointer(T* v=0):p(v){}T* Get()const{return p;}T* operator->()const{return p;}};
struct CCreatureGenerationFamilyDef {long DifficultyLevel,GeneratorObject,id;CCharString GetInstantiationName()const{return CCharString(id==1?"1":id==2?"2":id==3?"3":"4");}};
struct CTCHeroExperience {long level;long GetRoughExperienceLevel()const{return level;}};
struct CTCCreatureGenerator {
 CArray<CDefIndex> families;unsigned clones,ready,active;
 const CArray<CDefIndex>& GetCreatureGeneratorFamilies()const{return families;}
 void SetCreatureGeneratorFamilies(const CArray<CDefIndex>& v){families=v;}
 void SetAsReadyToInitialise(bool b){Check(b);++ready;}
 void CloneFromCreator(const CTCCreatureGenerator&){++clones;}
 void SetAsActive(bool b){Check(!b);++active;}
};
struct CThing {
 bool dead;CTCCreatureGenerator* gen;CCharString name,section;C3DVector pos;unsigned kills;
 bool GetTC(CTCCreatureGenerator** v){*v=gen;return gen!=0;}const C3DVector& GetPos(){return pos;}
 const CCharString& GetScriptName(){return name;}const CCharString& GetSerialisationSectionName(){return section;}
 void SetSerialisationSectionName(const CCharString& s){section=s;}void Kill(bool b){Check(!b);++kills;dead=true;}
};
struct CThingPlayerCreature:CThing {CTCHeroExperience* exp;bool PeekTC(const CTCHeroExperience** t)const{*t=exp;return exp!=0;}};
template<class T> bool IS_THING_ALIVE(T* t){return t&&!t->dead;}
struct CGameDefinitionManager {
 CCreatureGenerationFamilyDef family[5];bool valid[5];
 struct Global {float CreatureGeneratorRegenDelaySeconds;} global;
 const Global& PeekGlobalDef()const{return global;}
 bool GetDef(long i,CDefPointer<const CCreatureGenerationFamilyDef>* p)const{p->p=valid[i]?family+i:0;return p->Get()!=0;}
 long GetDefGlobalIndexFromName(const CCharString& s)const{return !strcmp(s.p,"MARKER_CREATURE_GENERATOR_INVISIBLE")?41:atoi(s.p);}
 static CGameDefinitionManager& Get();
};static CGameDefinitionManager mgr;
CGameDefinitionManager& CGameDefinitionManager::Get(){return mgr;}
static long frame,fps,pick,created;static bool enabled,spawnOK;static CThing spawned;
struct CThingObject {
 static CThing* Create(long n,const C3DVector&,long p,const void* r,unsigned f,const CCharString& s){Check(p==4&&!r&&!f&&!strcmp(s.p,"creator"));created=n;return spawnOK?&spawned:0;}
};
struct CThingMarker {static CThing* Create(long n,const C3DVector&,const CCharString& s){Check(n==41&&!strcmp(s.p,"creator"));created=n;return spawnOK?&spawned:0;}};
struct CWorld {static long GetFrame(){return frame;}bool IsCreatureGenerationEnabled(const CThing&)const{return enabled;} unsigned long* GetWorldSeed(){static unsigned long seed;return &seed;} const CGameDefinitionManager& PeekDefinitionManager()const{return mgr;}};
struct CMainGameComponent{static long GetConstantFPS(){return fps;}};
struct CPlayerManager {CThingPlayerCreature* hero;const CPlayerManager& GetMainPlayer()const{return *this;}const CThingPlayerCreature* PeekPControlledCreature()const{return hero;}};
unsigned long GFRandom(unsigned long n,unsigned long*){Check(n>0);return pick%n;}
struct CTCCreatureGeneratorCreator {
 CThing parent;CWorld world;CPlayerManager pm;bool Generated;long FrameToRegenerate;CThing* PGenerator;
 CThing& GetParentThing(){return parent;} CWorld& GetWorld(){return world;}const CWorld& PeekWorld()const{return world;}
 const CPlayerManager& PeekPlayerManager()const{return pm;}const CGameDefinitionManager& GetDefinitionManager()const{return mgr;}
 bool IsGeneratorAlive()const{return IS_THING_ALIVE(PGenerator);}void FrameUpdate();
};
void CTCCreatureGeneratorCreator::FrameUpdate()
{
    if (!Generated) {
        if (CWorld::GetFrame() >= FrameToRegenerate && PeekWorld().IsCreatureGenerationEnabled(GetParentThing())) {
            CTCCreatureGenerator* ptc_generator;
            GetParentThing().GetTC(&ptc_generator);
            const CThingPlayerCreature* phero=PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
            if (IS_THING_ALIVE(phero)) {
                const CArray<CDefIndex>& families=ptc_generator->GetCreatureGeneratorFamilies();
                if (!families.empty()) {
                    const CTCHeroExperience* ptc_experience;
                    phero->PeekTC(&ptc_experience);
                    long lowest_level=-1,best_level=-1;
                    long difficulty_level=ptc_experience->GetRoughExperienceLevel()+1;
                    if (false) {} else for (long i=0;i<families.size();++i) {
                        CDefPointer<const CCreatureGenerationFamilyDef> pfamily_def;
                        GetDefinitionManager().GetDef(families[i],&pfamily_def);
                        if (pfamily_def.Get()) {
                            long level=pfamily_def->DifficultyLevel;
                            if(level<=difficulty_level && level>best_level) best_level=level;
                            if(lowest_level==-1 || level<lowest_level) lowest_level=level;
                        }
                    }
                    if(best_level==-1)best_level=lowest_level;
                    if(best_level!=-1) {
                        CArray<CDefPointer<const CCreatureGenerationFamilyDef> > valid_family_defs;
                        if (false) {} else for(long i=0;i<families.size();++i) {
                            CDefPointer<const CCreatureGenerationFamilyDef> pfamily_def;
                            GetDefinitionManager().GetDef(families[i],&pfamily_def);
                            if(pfamily_def.Get() && pfamily_def->DifficultyLevel==best_level)valid_family_defs.push_back(pfamily_def);
                        }
                        CDefPointer<const CCreatureGenerationFamilyDef> pselected_family_def=valid_family_defs[GFRandom(valid_family_defs.size(),GetWorld().GetWorldSeed())];
                        if(pselected_family_def.Get()) {
                            CTCCreatureGenerator* ptc_child_generator=0;
                            CThing* pgenerator=0;
                            if(pselected_family_def->GeneratorObject>0)
                                pgenerator=CThingObject::Create(pselected_family_def->GeneratorObject,GetParentThing().GetPos(),4,0,0,GetParentThing().GetScriptName());
                            else {
                                long marker_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_CREATURE_GENERATOR_INVISIBLE");
                                pgenerator=CThingMarker::Create(marker_def_index,GetParentThing().GetPos(),GetParentThing().GetScriptName());
                            }
                            if(IS_THING_ALIVE(pgenerator)) {
                                pgenerator->SetSerialisationSectionName(GetParentThing().GetSerialisationSectionName());
                                pgenerator->GetTC(&ptc_child_generator);
                                ptc_child_generator->CloneFromCreator(*ptc_generator);
                                if(IS_THING_ALIVE(PGenerator))PGenerator->Kill(false);
                                PGenerator=pgenerator;
                                FrameToRegenerate=0;
                                if(ptc_child_generator) {
                                    long best_index=GetDefinitionManager().GetDefGlobalIndexFromName(pselected_family_def->GetInstantiationName());
                                    CArray<CDefIndex> child_family;
                                    child_family.push_back(best_index);
                                    ptc_child_generator->SetCreatureGeneratorFamilies(child_family);
                                    ptc_child_generator->SetAsReadyToInitialise(true);
                                }
                            }
                        }
                    }
                }
            }
            ptc_generator->SetAsActive(false);
        }
        Generated=true;
    }
    if(FrameToRegenerate==0 && !IsGeneratorAlive()) {
        float regen_time=PeekWorld().PeekDefinitionManager().PeekGlobalDef().CreatureGeneratorRegenDelaySeconds;
        FrameToRegenerate=(long)(CWorld::GetFrame()+CMainGameComponent::GetConstantFPS()*regen_time);
    }
}
int main(){
 for(caseId=0;caseId<4096;++caseId){
    unsigned n=caseId; CTCCreatureGeneratorCreator a;CTCCreatureGenerator g,child;CTCHeroExperience exp;CThingPlayerCreature hero;CThing old;
    g.active=g.clones=g.ready=child.active=child.clones=child.ready=0;spawned.gen=&child;spawned.section="";spawned.dead=(n%19)==0;spawnOK=n%13!=0;created=-1;
    exp.level=(n/31)%6;hero.exp=&exp;hero.dead=(n%17)==0;a.pm.hero=(n%11)?&hero:0;
    old.dead=(n%3)==0;old.kills=0;a.PGenerator=n%5?&old:0;
    a.parent.gen=&g;a.parent.name="creator";a.parent.section="section";a.Generated=(n%7)==0;
    frame=100+n%64;a.FrameToRegenerate=(n%3)==1?frame+10:n%3==2?frame-6:0;
    fps=24+(n%3)*4;mgr.global.CreatureGeneratorRegenDelaySeconds=1.25f;enabled=n%9!=0;pick=n/4;
    int best=-1,lowest=-1;int list[5],cnt=0;
    for(int j=1;j<=4;++j){
        mgr.family[j].id=j;mgr.family[j].DifficultyLevel=(n/(j+3))%8;mgr.family[j].GeneratorObject=((n+j)%3)?j+5:0;
        mgr.valid[j]=(n+j)%7!=0;
        if((n>>j)&1){g.families.push_back(j);if(mgr.valid[j]){int lv=mgr.family[j].DifficultyLevel;if(lowest==-1||lv<lowest)lowest=lv;if(lv<=exp.level+1&&lv>best)best=lv;}}
    }
    if(best<0)best=lowest;
    for(long i=0;i<g.families.size();++i){int j=g.families[i];if(mgr.valid[j]&&mgr.family[j].DifficultyLevel==best)list[cnt++]=j;}
    bool active=!a.Generated&&frame>=a.FrameToRegenerate&&enabled;
    int chosen=active&&a.pm.hero&&!hero.dead&&cnt?list[pick%cnt]:0;
    int expectedCreate=chosen?(mgr.family[chosen].GeneratorObject?mgr.family[chosen].GeneratorObject:41):-1;
    bool replace=chosen&&spawnOK&&!spawned.dead,killOld=replace&&a.PGenerator&&!old.dead;
    CThing* expectedGenerator=replace?&spawned:a.PGenerator;
    long expectedTime=replace?0:a.FrameToRegenerate;
    if(!expectedTime&&(!expectedGenerator||expectedGenerator->dead))expectedTime=long(frame+fps*1.25f);
    a.FrameUpdate();
    Check(a.Generated&&a.FrameToRegenerate==expectedTime&&created==expectedCreate&&g.active==(active?1:0));
    Check(a.PGenerator==expectedGenerator&&old.kills==(killOld?1:0)&&child.clones==(replace?1:0)&&child.ready==child.clones);
    if(replace)Check(child.families.size()==1&&child.families[0]==chosen&&!strcmp(spawned.section.p,"section"));
 }
 puts("007c99f0_TEST PASS");return 0;
}