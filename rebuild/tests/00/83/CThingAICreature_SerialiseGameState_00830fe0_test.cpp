#include <cassert>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __int64 long long
#endif
typedef unsigned __int64 UID;
struct CThing { UID id; bool dead; bool IsDead() const{return dead;} UID GetUniqueID() const{return id;} };
struct CLink { CThing* ptr; CThing* Get() const{return ptr;} };
struct CRelationshipsInit {
    UID HomeBuildingUID, WorkBuildingUID, FatherCreatureUID, MotherCreatureUID, SpouseCreatureUID;
    CRelationshipsInit():HomeBuildingUID(0), WorkBuildingUID(0), FatherCreatureUID(0), MotherCreatureUID(0), SpouseCreatureUID(0) {}
};
struct CRecord {
    CRelationshipsInit* ptr;
    CRecord():ptr(0) {} ~CRecord(){delete ptr;}
    void Reset(CRelationshipsInit* p){delete ptr;ptr=p;}
    void ReleaseData(){delete ptr;ptr=0;}
    CRelationshipsInit* operator->(){return ptr;}
};
struct C3DVector {float X,Y,Z;};
static const char* flag_names[]={"EnableCreatureAutoPlacing","HasInformation","WanderWithInformation","WaveWithInformation","ContinueAIWithInformation","AllowedToFollowHero","RegionFollowingOverriddenFromScript","RespondingToFollowAndWait","CanBeCourted","CanBeMarried"};
static const char* uid_names[]={"HomeBuildingUID","WorkBuildingUID","FatherCreatureUID","MotherCreatureUID","SpouseCreatureUID"};
struct CPersistContext {
    unsigned mode; bool base_ok, relationships;
    unsigned nflag,nuid,npos,nname;
    bool flags[10]; UID uids[5]; float pos[3];
    bool IsSaving() const{return mode==1 || mode==3;}
    void Transfer(const char* key,bool& value,bool default_value){
        if(!std::strcmp(key,"HasRelationships")){assert(!default_value && value);value=relationships;return;}
        assert(nflag<10 && !std::strcmp(key,flag_names[nflag]));
        assert(default_value==(nflag==7));if(IsSaving()) flags[nflag]=value;else value=flags[nflag]; ++nflag;
    }
    void Transfer(const char* key,UID& value,UID def){
        assert(nuid<5 && !def && !std::strcmp(key,uid_names[nuid]));
        if(IsSaving()) uids[nuid]=value;else value=uids[nuid];++nuid;
    }
    void Transfer(float& value){if(mode==2||mode==3){assert(npos<3);if(IsSaving())pos[npos]=value;else value=pos[npos];++npos;}}
    void Transfer(const char* key,int& value){assert(!std::strcmp(key,"OverridingBrainName"));++nname;if(!IsSaving())value=36;}
};
struct CThingCreatureBase { bool SerialiseGameState(CPersistContext& c){return c.base_ok;} };
struct CThingAICreature: CThingCreatureBase {
    CRecord RelationshipsInit;
    C3DVector InitialPos;
    CLink HomeBuilding, WorkBuilding, FatherCreature, MotherCreature, SpouseCreature;
    bool EnableCreatureAutoPlacing, HasInformation, WanderWithInformation, WaveWithInformation, ContinueAIWithInformation, AllowedToFollowHero, RegionFollowingOverriddenFromScript, RespondingToFollowAndWait, CanBeCourted, CanBeMarried;
    int OverridingBrainName;
    bool SerialiseGameState(CPersistContext&);
};

bool CThingAICreature::SerialiseGameState(CPersistContext& context)
{
    if (CThingCreatureBase::SerialiseGameState(context)) {
        bool has_relationships = true;
        context.Transfer("HasRelationships", has_relationships, false);
        RelationshipsInit.Reset(new CRelationshipsInit);
        if (context.IsSaving()) {
            if (HomeBuilding.Get() && !HomeBuilding.Get()->IsDead())
                RelationshipsInit->HomeBuildingUID = HomeBuilding.Get()->GetUniqueID();
            if (WorkBuilding.Get() && !WorkBuilding.Get()->IsDead())
                RelationshipsInit->WorkBuildingUID = WorkBuilding.Get()->GetUniqueID();
            if (FatherCreature.Get() && !FatherCreature.Get()->IsDead())
                RelationshipsInit->FatherCreatureUID = FatherCreature.Get()->GetUniqueID();
            if (MotherCreature.Get() && !MotherCreature.Get()->IsDead())
                RelationshipsInit->MotherCreatureUID = MotherCreature.Get()->GetUniqueID();
            if (SpouseCreature.Get() && !SpouseCreature.Get()->IsDead())
                RelationshipsInit->SpouseCreatureUID = SpouseCreature.Get()->GetUniqueID();
        }
        context.Transfer(InitialPos.X);
        context.Transfer(InitialPos.Y);
        context.Transfer(InitialPos.Z);
        if (has_relationships) {
            context.Transfer("HomeBuildingUID", RelationshipsInit->HomeBuildingUID, (unsigned __int64)0);
            context.Transfer("WorkBuildingUID", RelationshipsInit->WorkBuildingUID, (unsigned __int64)0);
            context.Transfer("FatherCreatureUID", RelationshipsInit->FatherCreatureUID, (unsigned __int64)0);
            context.Transfer("MotherCreatureUID", RelationshipsInit->MotherCreatureUID, (unsigned __int64)0);
            context.Transfer("SpouseCreatureUID", RelationshipsInit->SpouseCreatureUID, (unsigned __int64)0);
        }
        if (context.IsSaving())
            RelationshipsInit.ReleaseData();
        context.Transfer("OverridingBrainName", OverridingBrainName);
        {
            bool temp = EnableCreatureAutoPlacing;
            context.Transfer("EnableCreatureAutoPlacing", temp, false);
            EnableCreatureAutoPlacing = temp;
        }
        {
            bool temp = HasInformation;
            context.Transfer("HasInformation", temp, false);
            HasInformation = temp;
        }
        {
            bool temp = WanderWithInformation;
            context.Transfer("WanderWithInformation", temp, false);
            WanderWithInformation = temp;
        }
        {
            bool temp = WaveWithInformation;
            context.Transfer("WaveWithInformation", temp, false);
            WaveWithInformation = temp;
        }
        {
            bool temp = ContinueAIWithInformation;
            context.Transfer("ContinueAIWithInformation", temp, false);
            ContinueAIWithInformation = temp;
        }
        {
            bool temp = AllowedToFollowHero;
            context.Transfer("AllowedToFollowHero", temp, false);
            AllowedToFollowHero = temp;
        }
        {
            bool temp = RegionFollowingOverriddenFromScript;
            context.Transfer("RegionFollowingOverriddenFromScript", temp, false);
            RegionFollowingOverriddenFromScript = temp;
        }
        {
            bool temp = RespondingToFollowAndWait;
            context.Transfer("RespondingToFollowAndWait", temp, true);
            RespondingToFollowAndWait = temp;
        }
        {
            bool temp = CanBeCourted;
            context.Transfer("CanBeCourted", temp, false);
            CanBeCourted = temp;
        }
        {
            bool temp = CanBeMarried;
            context.Transfer("CanBeMarried", temp, false);
            CanBeMarried = temp;
        }
        return true;
    }
    return false;
}

int main() {
    CThing live={(UID(1)<<44)+12,false},dead={34,true};
    for(unsigned mode=0;mode<5;++mode) for(unsigned state=0;state<18;++state) {
        CPersistContext c={};c.mode=mode;c.base_ok=state!=0;c.relationships=(state&1)!=0;
        CThingAICreature creature;
        CLink* links[]={&creature.HomeBuilding,&creature.WorkBuilding,&creature.FatherCreature,&creature.MotherCreature,&creature.SpouseCreature};
        bool* fields[]={&creature.EnableCreatureAutoPlacing,&creature.HasInformation,&creature.WanderWithInformation,&creature.WaveWithInformation,&creature.ContinueAIWithInformation,&creature.AllowedToFollowHero,&creature.RegionFollowingOverriddenFromScript,&creature.RespondingToFollowAndWait,&creature.CanBeCourted,&creature.CanBeMarried};
        for(unsigned i=0;i<5;++i){links[i]->ptr=(state+i)%3==0?&live:((state+i)%3==1?&dead:0);c.uids[i]=40+i;}
        for(unsigned i=0;i<10;++i){c.flags[i]=(i&1)!=0;*fields[i]=!c.flags[i];}
        creature.OverridingBrainName=8;creature.InitialPos.X=2;creature.InitialPos.Y=4;creature.InitialPos.Z=8;
        c.pos[0]=-1;c.pos[1]=-2;c.pos[2]=-4;
        assert(creature.SerialiseGameState(c)==c.base_ok);
        assert(c.nflag==(state?10U:0U) && c.nuid==(state&&c.relationships?5U:0U));
        assert(c.npos==(state&&(mode==2||mode==3)?3U:0U) && c.nname==(state?1U:0U));
        assert((creature.RelationshipsInit.ptr!=0)==(state&&!c.IsSaving()));
        if(!state)continue;
        for(unsigned i=0;i<10;++i)assert(*fields[i]==(c.IsSaving()==((i&1)==0)));
        if(c.IsSaving()&&c.relationships)for(unsigned i=0;i<5;++i)assert(c.uids[i]==(links[i]->ptr==&live?live.id:0));
        if(!c.IsSaving()){UID values[]={creature.RelationshipsInit->HomeBuildingUID,creature.RelationshipsInit->WorkBuildingUID,creature.RelationshipsInit->FatherCreatureUID,creature.RelationshipsInit->MotherCreatureUID,creature.RelationshipsInit->SpouseCreatureUID};
            for(unsigned i=0;i<5;++i)assert(values[i]==(c.relationships?40U+i:0U));
            assert(creature.OverridingBrainName==36);
        }
        if(mode==2)assert(creature.InitialPos.X==-1 && creature.InitialPos.Z==-4);
        if(mode==3)assert(c.pos[0]==2 && c.pos[2]==8);
    }
    std::puts("00830fe0_TEST PASS");return 0;
}