#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUP_MINIONREACTTOTRESPASSING_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTCCREATUREOPINIONOFHERO_H
struct CTCCreatureOpinionOfHero {};
struct CAIStateGroup_MinionReactToTrespassing { unsigned char _pad_0x04[sizeof(void*)]; long LastFrameCheckedIsStillValid; };
struct CThingAICreature;
struct CAIBrain { CThingAICreature* Creature; };
#endif
// BEGIN EXACT CANDIDATE
// CAIStateGroup_MinionReactToTrespassing::IsStillRunnable(bool) -- periodically check ongoing trespass deeds targeting the creature home
// Retail0x009580e0,295 bytes. Home retail+168 differs from donor+190, hence local creature overlay.
#include "engine/CAIStateGroup_MinionReactToTrespassing.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureOpinionOfHero.h"
struct CThing;
struct CBaseIntelligentPointer { virtual void PointerType(); void* Handle; CThing* GetPItem() const; }; // 0xA01B50
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*) const; };
struct CThing {
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead; unsigned char tail[0xd6]; CBaseIntelligentPointer Home; CThing* GetHome() { return Home.GetPItem(); }
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if(bits[type>>5]&(1UL<<(type&31))) {
            long key=type; CInterfaceMap* map=&interfaces; CInterfaceMap::Entry* it=map->LowerBound(&key);
            if(it==map->last || it->type>type) it=map->last;
            *out=(T*)it->component; return true;
        } return false;
    }
};


long GetWorldFrame(); // 0x49D870
extern long g_TrespassCheckInterval; // 0x138D2D4
struct COpinionDeedFilter_IsStillHappening { COpinionDeedFilter_IsStillHappening() {} };
struct COpinionDeedFilter_TargetIs { CThing* Target; COpinionDeedFilter_TargetIs(CThing* p):Target(p){} };
struct CFilter_Bind2 {
 const COpinionDeedFilter_IsStillHappening* F1; const COpinionDeedFilter_TargetIs* F2;
 CFilter_Bind2(const COpinionDeedFilter_IsStillHappening& a,const COpinionDeedFilter_TargetIs& b):F1(&a),F2(&b){}
};
struct CTCCreatureOpinionOfHeroMethods : CTCCreatureOpinionOfHero { void* FindMostRecentDeed(long,const CFilter_Bind2&,long*,bool); }; // 0x92E630
struct CAIStateGroup_MinionReactToTrespassing_Adapter : CAIStateGroup_MinionReactToTrespassing {
 CAIBrain* Brain() { return *(CAIBrain**)&_pad_0x04[0]; }
 bool IsStillRunnable(bool initialCheck);
};
bool CAIStateGroup_MinionReactToTrespassing_Adapter::IsStillRunnable(bool initialCheck) {
 if(!initialCheck && GetWorldFrame()-LastFrameCheckedIsStillValid < g_TrespassCheckInterval) return true;
 LastFrameCheckedIsStillValid=GetWorldFrame();
 CThing* home=((CThing*)Brain()->Creature)->GetHome();
 if(home && !(home->dead&1)) {
  CTCCreatureOpinionOfHeroMethods* opinion;
  if(((CThing*)Brain()->Creature)->GetInterface(&opinion,0x4f)) {
   if(opinion->FindMostRecentDeed(10,CFilter_Bind2(COpinionDeedFilter_IsStillHappening(),COpinionDeedFilter_TargetIs(home)),0,true) ||
      opinion->FindMostRecentDeed(11,CFilter_Bind2(COpinionDeedFilter_IsStillHappening(),COpinionDeedFilter_TargetIs(home)),0,true) ||
      opinion->FindMostRecentDeed(2,CFilter_Bind2(COpinionDeedFilter_IsStillHappening(),COpinionDeedFilter_TargetIs(home)),0,true)) return true;
  }
 }
 return false;
}
// END EXACT CANDIDATE
static int failures, frames, homes, finds, deeds, hit, expectedType, fallback;
static long now;
static CThing* selectedHome;
static CTCCreatureOpinionOfHeroMethods* expectedOpinion;
long g_TrespassCheckInterval=10;
long GetWorldFrame() { ++frames; return now; }
void CBaseIntelligentPointer::PointerType() {}
CThing* CBaseIntelligentPointer::GetPItem() const { ++homes; return selectedHome; }
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key) const { ++finds; if(*key!=0x4f) ++failures; return fallback?last:first; }
void* CTCCreatureOpinionOfHeroMethods::FindMostRecentDeed(long type,const CFilter_Bind2& filter,long* frame,bool active) {
 ++deeds; if(this!=expectedOpinion || !filter.F1 || filter.F2->Target!=selectedHome || frame || !active) ++failures;
 long expected=deeds==1?10:deeds==2?11:2; if(type!=expected) ++failures;
 return hit==deeds?this:0;
}
static void Check(bool ok) { if(!ok) ++failures; }
static void Reset() { frames=homes=finds=deeds=hit=fallback=0; now=100; }
int main() {
 CAIStateGroup_MinionReactToTrespassing_Adapter state; CAIBrain brain; CThing creature, home;
 CTCCreatureOpinionOfHeroMethods opinion; expectedOpinion=&opinion; selectedHome=&home; home.dead=0;
 *(CAIBrain**)state._pad_0x04=&brain; brain.Creature=(CThingAICreature*)&creature;
 for(int j=0;j<9;++j) creature.bits[j]=0;
 creature.bits[0x4f>>5]=1UL<<(0x4f&31);
 CInterfaceMap::Entry entry[2]; entry[0].type=0x4f; entry[0].component=&opinion; entry[1].component=&opinion;
 creature.interfaces.first=entry; creature.interfaces.last=entry+1;
 Reset(); state.LastFrameCheckedIsStillValid=95; Check(state.IsStillRunnable(false)); Check(frames==1 && homes==0 && state.LastFrameCheckedIsStillValid==95);
 Reset(); state.LastFrameCheckedIsStillValid=110; Check(state.IsStillRunnable(false)); Check(homes==0);
 for(int winner=0;winner<=3;++winner) {
  Reset(); hit=winner; state.LastFrameCheckedIsStillValid=90;
  Check(state.IsStillRunnable(false)==(winner!=0)); Check(frames==2 && homes==1 && finds==1 && deeds==(winner?winner:3)); Check(state.LastFrameCheckedIsStillValid==100);
 }
 Reset(); hit=1; state.LastFrameCheckedIsStillValid=100; Check(state.IsStillRunnable(true)); Check(frames==1 && deeds==1);
 Reset(); selectedHome=0; Check(!state.IsStillRunnable(true)); Check(homes==1 && finds==0);
 Reset(); selectedHome=&home; home.dead=1; Check(!state.IsStillRunnable(true)); Check(finds==0);
 Reset(); home.dead=0; creature.bits[0x4f>>5]=0; Check(!state.IsStillRunnable(true)); Check(finds==0);
 creature.bits[0x4f>>5]=1UL<<(0x4f&31);
 Reset(); fallback=1; hit=2; Check(state.IsStillRunnable(true)); Check(deeds==2);
 Reset(); entry[0].type=0x50; hit=3; Check(state.IsStillRunnable(true)); Check(deeds==3);
 if(failures) { std::printf("009580e0_TEST FAIL %d\n",failures); return 1; }
 std::printf("009580e0_TEST PASS\n"); return 0;
}