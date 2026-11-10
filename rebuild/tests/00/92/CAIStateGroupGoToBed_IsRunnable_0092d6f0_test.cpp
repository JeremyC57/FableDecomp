#include <stdio.h>
#include <stdlib.h>
#include <string>
static std::string trace;
static void check(bool v) { if(!v) { puts("FAIL"); exit(1); } }
class CThing { public: bool dead; bool IsDead() const { return dead; } };
template<class T> class CIntelligentPointer { public: T* value; mutable int reads; T* Get() const { ++reads; return value; } T* operator->() const { return Get(); } };
template<class T> inline bool operator!=(const CIntelligentPointer<T>& pointer,int) { return pointer.Get()!=0; }
template<class T> inline bool IS_THING_ALIVE(const T& thing) { if(thing!=0) { if(!thing->IsDead()) return true; } return false; }
class CTCSleep { public: long LastFrameWokeUp; long GetLastFrameWokeUp() { return LastFrameWokeUp; } };
class Creature { public: CTCSleep* sleep; template<class T> bool GetTC(T** out) { trace+='T'; if(!sleep)return false; *out=sleep; return true; } };
class CWorld { public: static long frame; static long GetFrame() { trace+='F';return frame; } }; long CWorld::frame;
class CAIStateGroup_GoToBed { public:
 CIntelligentPointer<CThing> PBed; Creature creature; CWorld world;
 bool crime,trespass,sleep_time,free_bed,bandit; int replacements;
 Creature& GetCreature() const { return const_cast<Creature&>(creature); }
 CWorld& GetWorld() const { trace+='W';return const_cast<CWorld&>(world); }
 void OnReplace() { trace+='R';++replacements; }
 bool IsUnreportedCrimeKnown() const { trace+='C';return crime; }
 bool IsHeroTrespassingPerceived() const { trace+='H';return trespass; }
 bool IsInSleepTime() const { trace+='S';return sleep_time; }
 bool FindFreeBed(bool v) { check(!v);trace+='B';return free_bed; }
 bool FindFreeBanditBed() { trace+='D';return bandit; }
 bool IsRunnable();
};
bool CAIStateGroup_GoToBed::IsRunnable() {
    if(IS_THING_ALIVE(PBed)) CAIStateGroup_GoToBed::OnReplace();
    CTCSleep* tc_sleep;
    if(GetCreature().GetTC(&tc_sleep)) {
        if(GetWorld().GetFrame()-tc_sleep->GetLastFrameWokeUp()<120) return false;
    }
    if(IsUnreportedCrimeKnown()) return false;
    if(IsHeroTrespassingPerceived()) return false;
    if(IsInSleepTime()) {
        if(FindFreeBed(false)) {
            CAIStateGroup_GoToBed::OnReplace();
            return true;
        }
        if(FindFreeBanditBed()) {
            CAIStateGroup_GoToBed::OnReplace();
            return true;
        }
    }
    return false;
}
int main() {
 const long deltas[]={-121,-1,0,1,118,119,120,121,10000};
 for(int bed=0;bed!=3;++bed)for(int have=0;have!=2;++have)for(int d=0;d!=9;++d)for(int bits=0;bits!=32;++bits) {
  CThing thing; thing.dead=bed==2; CTCSleep sleep; sleep.LastFrameWokeUp=500;
  CAIStateGroup_GoToBed x; x.PBed.value=bed?&thing:0;x.PBed.reads=0;x.creature.sleep=have?&sleep:0;
  x.crime=(bits&1)!=0;x.trespass=(bits&2)!=0;x.sleep_time=(bits&4)!=0;x.free_bed=(bits&8)!=0;x.bandit=(bits&16)!=0;x.replacements=0;
  CWorld::frame=500+deltas[d];trace="";bool got=x.IsRunnable();
  std::string expected;int reps=0;bool want=false;
  if(bed==1){expected+='R';++reps;}expected+='T';
  if(have)expected+="WF";
  if(!have||deltas[d]>=120) {
   expected+='C';if(!x.crime){expected+='H';if(!x.trespass){expected+='S';if(x.sleep_time){expected+='B';if(x.free_bed){want=true;expected+='R';++reps;}else{expected+='D';if(x.bandit){want=true;expected+='R';++reps;}}}}}
  }
  check(got==want);check(trace==expected);check(x.replacements==reps);check(x.PBed.reads==(bed?2:1));
 }
 puts("0092d6f0_TEST PASS");return 0;
}