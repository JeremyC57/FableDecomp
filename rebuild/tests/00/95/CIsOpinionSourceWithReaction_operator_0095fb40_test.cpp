#include <stdio.h>
#define FABLE_RETAIL_OPINION_FILTER_PASS27
#include "engine/EOpinionReactionType.h"
#include "engine/EOpinionAttitudeType.h"
struct CTCOpinionSource;
struct CThing {
    unsigned long low,high; const CTCOpinionSource* component;
    unsigned __int64 GetUniqueID()const{return (static_cast<unsigned __int64>(high)<<32)|low;}
    bool PeekTC(const CTCOpinionSource** out)const{if(!component)return false;*out=component;return true;}
};
struct CTCOpinionSource{
    CThing* parent;bool self,unknown,always,nav;
    bool IsReactToSelfPermitted()const{return self;}
    bool IsUnknownThreatSource()const{return unknown;}
    const CThing& GetParentThing()const{return *parent;}
    bool IsAlwaysAwareOf()const{return always;}
    bool PeekUseNavLineOfSight()const{return nav;}
};
struct CTCCreatureOpinionOfHero { CThing* unknown;const CThing* PeekUnknownThreatOpinionSource()const{return unknown;} };
struct CAIStateGroup_OpinionReactionRoot;
struct Behaviour {bool (CAIStateGroup_OpinionReactionRoot::*IsRunnableFunc)();};
static int allowedCalls,deniedCalls,reactionCalls,awareCalls;
struct CAIStateGroup_OpinionReactionRoot {
    CThing creature;CThing* ThingReactingTo;bool ReactionFlags[79];static Behaviour Behaviours[79];
    bool Allow(){++allowedCalls;return true;} bool Deny(){++deniedCalls;return false;}
    CThing& GetCreature(){return creature;}
};
Behaviour CAIStateGroup_OpinionReactionRoot::Behaviours[79];
static const CThing* who;static const CThing* other;static const CTCOpinionSource* input;
static bool following,awareResult,bad;
struct COpinionReactionManager {
 EOpinionReactionType value;
 bool GetAllowWhileFollowingPlayer(EOpinionReactionType r)const{return (int(r)%2)==0;}
 EOpinionReactionType GetReactionType(const CThing& creature,const bool* flags,const CTCOpinionSource* source)const{
    ++reactionCalls;if(&creature!=who || source!=input)bad=true;
    for(int i=0;i<79;++i)if(flags[i]!=(i>=18 && (!following || i%2==0) && i%3!=2))bad=true;
    return value;
 }
};
namespace NCreatureAI{
 bool IsAwareOfThingIndirectly(const CThing* c,const CThing* t,bool nav){
    ++awareCalls;if(c!=who || t!=other || nav!=input->nav)bad=true;return awareResult;
 }
}
struct CIsOpinionSourceWithReaction {
 CAIStateGroup_OpinionReactionRoot* PStateGroup;CTCCreatureOpinionOfHero* PTCOpinion;
 COpinionReactionManager* ReactionManager;bool IsFollowingPlayer,IsAwareOfPlayerForReactions;
 mutable EOpinionReactionType Reaction;
 bool operator()(const CThing& test_thing)const;
};
bool CIsOpinionSourceWithReaction::operator()(const CThing& test_thing) const
{
    const CTCOpinionSource* ptc_source;
    if (test_thing.PeekTC(&ptc_source) &&
        (ptc_source->IsReactToSelfPermitted() || test_thing.GetUniqueID()!=PStateGroup->GetCreature().GetUniqueID())) {
        PStateGroup->ThingReactingTo=const_cast<CThing*>(&test_thing);
        for (unsigned long c1=0;c1<OPINION_REACTION_TYPE_LAST;++c1) {
            PStateGroup->ReactionFlags[c1]=c1>=OPINION_ATTITUDE_TYPE_LAST &&
                (!IsFollowingPlayer || ReactionManager->GetAllowWhileFollowingPlayer((EOpinionReactionType)c1));
            if (PStateGroup->ReactionFlags[c1]) {
                bool (CAIStateGroup_OpinionReactionRoot::*is_runnable_func)()=PStateGroup->Behaviours[c1].IsRunnableFunc;
                PStateGroup->ReactionFlags[c1]=(is_runnable_func && !(PStateGroup->*is_runnable_func)()) ? false : true;
            }
        }
        Reaction=ReactionManager->GetReactionType(PStateGroup->GetCreature(),PStateGroup->ReactionFlags,ptc_source);
        if (Reaction!=OPINION_REACTION_TYPE_NONE) {
            if (ptc_source->IsUnknownThreatSource() && (IsAwareOfPlayerForReactions || !(PTCOpinion &&
                PTCOpinion->PeekUnknownThreatOpinionSource()==&ptc_source->GetParentThing()))) return false;
            if (ptc_source->IsAlwaysAwareOf()) return true;
            if (NCreatureAI::IsAwareOfThingIndirectly(&PStateGroup->GetCreature(),&test_thing,ptc_source->PeekUseNavLineOfSight())) return true;
        }
    }
    return false;
}

int main(){
 CAIStateGroup_OpinionReactionRoot group;COpinionReactionManager mgr;CThing thing;
 CTCCreatureOpinionOfHero hero;CTCOpinionSource source;CIsOpinionSourceWithReaction f;
 f.PStateGroup=&group;f.ReactionManager=&mgr;who=&group.creature;other=&thing;input=&source;source.parent=&thing;
 for(int i=0;i<79;++i)group.Behaviours[i].IsRunnableFunc=i%3==0?0:i%3==1?&CAIStateGroup_OpinionReactionRoot::Allow:&CAIStateGroup_OpinionReactionRoot::Deny;
 for(int mask=0;mask<2048;++mask)for(int uid=0;uid<3;++uid){
    group.creature.low=17;group.creature.high=32;thing.low=uid==1?18:17;thing.high=uid==2?33:32;
    thing.component=(mask&1)?&source:0;source.self=(mask&2)!=0;following=f.IsFollowingPlayer=(mask&4)!=0;
    mgr.value=(EOpinionReactionType)((mask&8)?4:0);source.unknown=(mask&16)!=0;
    f.IsAwareOfPlayerForReactions=(mask&32)!=0;f.PTCOpinion=(mask&64)?&hero:0;
    hero.unknown=(mask&128)?&thing:&group.creature;source.always=(mask&256)!=0;
    awareResult=(mask&512)!=0;source.nav=(mask&1024)!=0;
    allowedCalls=deniedCalls=reactionCalls=awareCalls=0;bad=false;group.ThingReactingTo=0;f.Reaction=(EOpinionReactionType)31;
    for(int j=0;j<79;++j)group.ReactionFlags[j]=true;
    const bool result=f(thing);bool eligible=thing.component && (source.self || uid!=0);
    bool proceed=eligible && mgr.value!=OPINION_REACTION_TYPE_NONE && (!source.unknown ||
        (!f.IsAwareOfPlayerForReactions&&f.PTCOpinion&&hero.unknown==&thing));
    int allowed=0,denied=0;
    for(int j=0;j<79;++j){
      bool active=eligible && j>=18 && (!following||j%2==0);
      if(active && j%3==1)++allowed;if(active && j%3==2)++denied;
      if(group.ReactionFlags[j]!=(eligible ? active && j%3!=2 : true))return 2;
    }
    if(bad || result!=(proceed&&(source.always||awareResult)) || group.ThingReactingTo!=(eligible?&thing:0) ||
       f.Reaction!=(eligible?mgr.value:(EOpinionReactionType)31) || reactionCalls!=int(eligible) ||
       awareCalls!=int(proceed&&!source.always) || allowedCalls!=allowed || deniedCalls!=denied)return 1;
 }
 puts("0095fb40_TEST PASS");return 0;
}