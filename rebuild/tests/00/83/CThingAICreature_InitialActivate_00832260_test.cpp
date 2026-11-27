#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void ck(bool b){if(!b)exit(2);}
struct CThing; struct CTCFamilyMember;
struct CThing {
 CTCFamilyMember* family;bool dead; CThing():family(0),dead(false){}
 bool GetTC(CTCFamilyMember** rec){if(family)*rec=family;return family!=0;}
};
struct CThingBuilding:CThing{};
struct CTCFamilyMember{const CThing* items[4];int n;void AddRelative(const CThing& t){ck(n<4);items[n++]=&t;}};
struct CThingGameObject:CThing{int active;void InitialActivate(){++active;}};
struct CAIBrain{int enabled;void EnableCreatureAutoPlacing(){++enabled;}};
template<class T>struct Pointer{T* ptr;int resets;Pointer():ptr(0),resets(0){}bool IsNull()const{return ptr==0;}T* operator->(){return ptr;}T& operator*(){return *ptr;}T* Get()const{return ptr;}void Reset(T* t){ptr=t;++resets;}Pointer& operator=(T* t){ptr=t;return *this;}};
template<class F,class T>T GFDebugPDynamicCast(F f,T){return static_cast<T>(f);}
bool IS_THING_ALIVE(CThing* p){return p&&!p->dead;}
typedef unsigned __int64 UID;
struct CThingManager{CThing* targets[5];int remaps,queries;bool less;long map;
 UID RemapUIDForSerialisation(bool a,bool b,unsigned long n,UID u){ck(!a&&b==less&&n==(unsigned long)map&&u>0xffffffffULL);++remaps;return u+17;}
 CThing* GetPThingWithUID(UID u){++queries;int i=(int)(u-17);ck(i>=1&&i<=5);return targets[i-1];}
} manager;
struct CThingAICreature:CThingGameObject{
 struct CRelationshipsInit{UID HomeBuildingUID,WorkBuildingUID,FatherCreatureUID,MotherCreatureUID,SpouseCreatureUID;};
 Pointer<CRelationshipsInit> RelationshipsInit;Pointer<CThingBuilding> HomeBuilding,WorkBuilding;
 Pointer<CThingAICreature> FatherCreature,MotherCreature,SpouseCreature;Pointer<CAIBrain> PBrain;bool EnableCreatureAutoPlacing;
 CThingManager& GetThingManager(){return manager;}bool IsLevelLess(){return manager.less;}long GetCurrentMapNumber(){return manager.map;}void InitialActivate();
};
void CThingAICreature::InitialActivate()
{
    CThingGameObject::InitialActivate();
    if (!RelationshipsInit.IsNull()) {
        CTCFamilyMember* ptc_family_member=0;
        GetTC(&ptc_family_member);
        if (RelationshipsInit->HomeBuildingUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->HomeBuildingUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                HomeBuilding=GFDebugPDynamicCast(pthing,(CThingBuilding*)0);
            }
        }
        if (RelationshipsInit->WorkBuildingUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->WorkBuildingUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                WorkBuilding=GFDebugPDynamicCast(pthing,(CThingBuilding*)0);
            }
        }
        if (RelationshipsInit->FatherCreatureUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->FatherCreatureUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                FatherCreature=GFDebugPDynamicCast(pthing,(CThingAICreature*)0);
                if (ptc_family_member) ptc_family_member->AddRelative(*FatherCreature);
                CTCFamilyMember* ptc_father_family_member;
                if (FatherCreature->GetTC(&ptc_father_family_member)) ptc_father_family_member->AddRelative(*this);
            }
        }
        if (RelationshipsInit->MotherCreatureUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->MotherCreatureUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                MotherCreature=GFDebugPDynamicCast(pthing,(CThingAICreature*)0);
                if (ptc_family_member) ptc_family_member->AddRelative(*MotherCreature);
                CTCFamilyMember* ptc_mother_family_member;
                if (MotherCreature->GetTC(&ptc_mother_family_member)) ptc_mother_family_member->AddRelative(*this);
            }
        }
        if (RelationshipsInit->SpouseCreatureUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->SpouseCreatureUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                SpouseCreature=GFDebugPDynamicCast(pthing,(CThingAICreature*)0);
                if (ptc_family_member) ptc_family_member->AddRelative(*SpouseCreature);
            }
        }
    }
    RelationshipsInit.Reset(0);
    if (EnableCreatureAutoPlacing && PBrain.Get()) PBrain->EnableCreatureAutoPlacing();
}
int main(){
 unsigned long seed=93101;unsigned observed=0;
 for(int k=0;k<32768;++k){
  seed=seed*1664525UL+1013904223UL;
  CThingAICreature c,people[3];CThingBuilding buildings[2];CThing* targets[5]={buildings,buildings+1,people,people+1,people+2};
  CTCFamilyMember fam[4];memset(fam,0,sizeof(fam));c.active=0;CAIBrain brain;brain.enabled=0;
  c.family=(seed&(1ul<<20))?fam:0;bool init=!(k%17==0); CThingAICreature::CRelationshipsInit rel={0};UID* u[5]={&rel.HomeBuildingUID,&rel.WorkBuildingUID,&rel.FatherCreatureUID,&rel.MotherCreatureUID,&rel.SpouseCreatureUID};
  c.RelationshipsInit.ptr=init?&rel:0;c.EnableCreatureAutoPlacing=(seed&(1ul<<27))!=0;c.PBrain.ptr=(seed&(1ul<<28))?&brain:0;
  manager.less=(k&1)!=0;manager.map=k%8;manager.remaps=manager.queries=0;bool live[5];int reads=0;
  for(int i=0;i<5;++i){unsigned st=(seed>>(i*3))&7;*u[i]=(st&1)?(0x100000000ULL+i+1):0;manager.targets[i]=(st&2)?targets[i]:0;targets[i]->dead=(st&4)!=0;live[i]=init&&*u[i]&&manager.targets[i]&&!targets[i]->dead;reads+=init&&*u[i];if(i>=2)people[i-2].family=(seed&(1ul<<(i+19)))?fam+i-1:0;}
  c.InitialActivate();ck(c.active==1&&manager.remaps==reads&&manager.queries==reads&&c.RelationshipsInit.IsNull()&&c.RelationshipsInit.resets==1);
  ck(c.HomeBuilding.Get()==(live[0]?buildings:0)&&c.WorkBuilding.Get()==(live[1]?buildings+1:0));ck(c.FatherCreature.Get()==(live[2]?people:0)&&c.MotherCreature.Get()==(live[3]?people+1:0)&&c.SpouseCreature.Get()==(live[4]?people+2:0));
  int n=0;for(int j=0;j<3;++j){if(c.family&&live[j+2])ck(fam[0].items[n++]==people+j);int expect=j<2&&people[j].family&&live[j+2];ck(fam[j+1].n==expect);if(expect)ck(fam[j+1].items[0]==&c);}
  ck(fam[0].n==n&&brain.enabled==(c.EnableCreatureAutoPlacing&&c.PBrain.Get()));
  for(int j=0;j<5;++j)observed|=1u<<(j*2+live[j]);
 }
 ck(observed==1023);puts("00832260_TEST PASS");return 0;
}