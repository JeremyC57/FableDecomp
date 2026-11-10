// 006d4e60 selected body copied verbatim; dependency fixtures isolate control flow.
#include <stdio.h>
#include <assert.h>
enum ETCInterfaceType {TCI_HERO_OPINION_DEED_LOG=81,TCI_CREATURE_OPINION_OF_HERO=79};
class CTCHeroStats;class CTCCreatureOpinionOfHero;class CTCFollowed;
class CThing { public:
 bool supervised,heroOpinion,hasOpinion;mutable int SupervisionCalls,StatsCalls,OpinionCalls,FollowedCalls;
 const CTCHeroStats* Stats;const CTCCreatureOpinionOfHero* Opinion;const CTCFollowed* Followed;
 CThing():supervised(false),heroOpinion(false),hasOpinion(false),SupervisionCalls(0),StatsCalls(0),OpinionCalls(0),FollowedCalls(0),Stats(0),Opinion(0),Followed(0){}
 bool IsUnderScriptSupervision()const{++SupervisionCalls;return supervised;}
 bool HasTC(ETCInterfaceType id)const{return id==TCI_HERO_OPINION_DEED_LOG?heroOpinion:hasOpinion;}
 bool PeekTC(const CTCHeroStats** p)const{++StatsCalls;*p=Stats;return Stats!=0;}
 bool PeekTC(const CTCCreatureOpinionOfHero** p)const{++OpinionCalls;*p=Opinion;return Opinion!=0;}
 bool PeekTC(const CTCFollowed** p)const{++FollowedCalls;*p=Followed;return Followed!=0;}
};
class CThingAICreature:public CThing { public:bool allowed;bool IsAllowedToFollowHero()const{return allowed;}};
class CTCHeroStats {public:
 long RenownLevel,Limit;mutable int Calls;mutable long SeenLevel;
 CTCHeroStats():RenownLevel(0),Limit(0),Calls(0),SeenLevel(-1){}
 long GetRenownLevel()const{return RenownLevel;}
 long GetMaxFollowersForRenownLevel(long n)const{++Calls;SeenLevel=n;return Limit;}
};
class CTCCreatureOpinionOfHero {public:
 bool Continue,Start;mutable int ContinueCalls,StartCalls;
 CTCCreatureOpinionOfHero():Continue(false),Start(false),ContinueCalls(0),StartCalls(0){}
 bool IsWillingToContinueFollowingHero()const{++ContinueCalls;return Continue;}
 bool IsWillingToStartFollowingHero()const{++StartCalls;return Start;}
};
class CTCFollowed {public:
 const CThing* ParentThing;const CThing* ExpectedCreature;bool Follower;long Count;
 mutable int FollowerCalls,CountCalls;
 CTCFollowed():ParentThing(0),ExpectedCreature(0),Follower(false),Count(0),FollowerCalls(0),CountCalls(0){}
 const CThing& GetParentThing()const{return *ParentThing;}
 bool IsFollower(const CThing* p)const{assert(p==ExpectedCreature);++FollowerCalls;return Follower;}
 long GetNumFollowers()const{++CountCalls;return Count;}
 bool IsCreatureValidFollower(const CThingAICreature&,bool*)const;
};
bool CTCFollowed::IsCreatureValidFollower(const CThingAICreature& creature,bool* pis_ai_follower)const
{
 bool local_bool;
 bool& is_ai_follower=pis_ai_follower==0?local_bool:*pis_ai_follower;
 is_ai_follower=!creature.IsUnderScriptSupervision() && GetParentThing().HasTC(TCI_HERO_OPINION_DEED_LOG) && creature.HasTC(TCI_CREATURE_OPINION_OF_HERO);
 if(creature.IsAllowedToFollowHero()){
  if(is_ai_follower){
   const CTCHeroStats* ptc_hero_stats;
   GetParentThing().PeekTC(&ptc_hero_stats);
   if(!(IsFollower(&creature)||GetNumFollowers()<ptc_hero_stats->GetMaxFollowersForRenownLevel(ptc_hero_stats->GetRenownLevel())))return false;
   const CTCCreatureOpinionOfHero* ptc_opinion;
   creature.PeekTC(&ptc_opinion);
   const CTCFollowed* ptc_followed;
   GetParentThing().PeekTC(&ptc_followed);
   return ptc_followed->IsFollower(&creature)?ptc_opinion->IsWillingToContinueFollowingHero():ptc_opinion->IsWillingToStartFollowingHero();
  }
  return true;
 }
 return false;
}

int main(){unsigned cases=0;
 for(int bits=0;bits<256;++bits)for(int count=0;count<5;++count)for(int limit=0;limit<5;limit+=2)for(int out=0;out<2;++out){
  CThing parent;CThingAICreature creature;CTCFollowed task,found;CTCHeroStats stats;CTCCreatureOpinionOfHero opinion;
  creature.supervised=(bits&1)!=0;creature.allowed=(bits&2)!=0;parent.heroOpinion=(bits&4)!=0;creature.hasOpinion=(bits&8)!=0;
  task.Follower=(bits&16)!=0;found.Follower=(bits&32)!=0;opinion.Continue=(bits&64)!=0;opinion.Start=(bits&128)!=0;
  task.Count=count;stats.Limit=limit;stats.RenownLevel=37;
  task.ParentThing=&parent;task.ExpectedCreature=&creature;found.ExpectedCreature=&creature;
  parent.Stats=&stats;parent.Followed=&found;creature.Opinion=&opinion;
  bool temporary=false;bool result=task.IsCreatureValidFollower(creature,out?&temporary:0);
  bool expectedTemporary=!creature.supervised && parent.heroOpinion && creature.hasOpinion;
  bool capacity=task.Follower || count<limit;
  bool expected=creature.allowed && (!expectedTemporary || (capacity && (found.Follower?opinion.Continue:opinion.Start)));
  assert(result==expected);if(out)assert(temporary==expectedTemporary);
  bool checked=creature.allowed && expectedTemporary;
  bool deep=checked && capacity;
  assert(creature.SupervisionCalls==1);
  assert(parent.StatsCalls==(checked?1:0));assert(task.FollowerCalls==(checked?1:0));
  assert(task.CountCalls==(checked&&!task.Follower?1:0));assert(stats.Calls==task.CountCalls);
  if(stats.Calls)assert(stats.SeenLevel==37);
  assert(creature.OpinionCalls==(deep?1:0));assert(parent.FollowedCalls==(deep?1:0));assert(found.FollowerCalls==(deep?1:0));
  assert(opinion.ContinueCalls==(deep&&found.Follower?1:0));assert(opinion.StartCalls==(deep&&!found.Follower?1:0));
  ++cases;
 }
 assert(cases==7680);puts("006d4e60_TEST PASS");return 0;
}