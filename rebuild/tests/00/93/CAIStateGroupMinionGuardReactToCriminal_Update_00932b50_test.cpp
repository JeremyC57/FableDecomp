#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <stdlib.h>
typedef std::string CCharString;
enum EAIActionReturn { OTHER, AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FINISHED, PROGRESS };
enum {ACTION_REPEAT_CALLBACK, ACTION_REPEAT_UNTIL_FINISHED, AI_MOVEMENT_RUNNING,AI_MOVEMENT_WALKING,TCI_GUARD,TCI_VILLAGE_MEMBER};
enum {CRIME_SEVERITY_NONE,CRIME_SEVERITY_MINOR,CRIME_SEVERITY_MODERATE,CRIME_SEVERITY_SERIOUS};
static void check(bool ok){if(!ok)abort();}
struct C3DVector {float x,y,z; C3DVector(float a=0,float b=0,float c=0):x(a),y(b),z(c){}
 C3DVector operator-(C3DVector b)const{return C3DVector(x-b.x,y-b.y,z-b.z);}
 C3DVector operator+(C3DVector b)const{return C3DVector(x+b.x,y+b.y,z+b.z);}
 C3DVector operator*(float f)const{return C3DVector(x*f,y*f,z*f);}
 float GetMagnitude2D()const{return (float)sqrt(x*x+y*y);}
 void Normalise(){float m=(float)sqrt(x*x+y*y+z*z);x/=m;y/=m;z/=m;}
};
static unsigned seed;
static unsigned pick(unsigned k,unsigned n){unsigned v=seed^(k*2654435761u);v^=v>>16;v*=2246822519u;v^=v>>13;return v%n;}
static int tick,start,frame,fps,limit,queries,marked,terminated,guardset,move,step,look,randoms,spoken,runchecks;
static bool allowed,guardpresent,villagepresent,emptyfirst,otherguard,nearby;
static int severity,crimeage,jump,initialWarnings,initialDeed,elapsed;
static std::vector<std::string> speeches;
static int point(int n){return pick(50+n,8)==0?1:(pick(55+n,3)==0?3:2);}
static int levelDeed(int round){return int(pick(120+round,4))-1;}
static bool emptyAt(int round){return round==0?emptyfirst:pick(80+round,3)==0;}
static bool must(int round){return pick(180+round,3)!=0;}
static bool runs(int t){return pick(230+t,4)==0;}
static bool stops(int t){return pick(280+t,8)==0;}
static const char *utter(int round,int level){return emptyAt(round)?"":level==0?"first":"escalated";}
struct CWorld{static int GetFrame(){return frame;}};
struct CMainGameComponent{static int GetConstantFPS(){return fps;}};
struct CTCGuard {int GetFrameLastCrimeSeen(){return start-crimeage;}int GetLastCrimeSeenSeverity(){return severity;}} guard;
struct Actor;
struct CTCVillage {
 Actor* GetGuardWarningHero();
 void SetGuardWarningHero(Actor* a);
 CCharString GetWarningStringForCurrentCrimes(int level,bool& b,long& d){int r=queries/2;if(!(queries%2))check(level==0);++queries;b=must(r);d=levelDeed(r);return utter(r,level);}
 bool IsHeroCriminal(){return tick<limit;}
 void SetHeroAsEnemyBecauseOfCrime(){++marked;}
} village;
struct CTCVillageMember{CTCVillage* GetPVillage(){return villagepresent?&village:0;}} member;
struct Actor {bool alive;C3DVector pos;C3DVector GetPos(){return pos;}bool GetTC(int id,CTCVillageMember** p){check(id==TCI_VILLAGE_MEMBER);*p=&member;return true;}
 bool GetTC(int id,CTCGuard** p){check(id==TCI_GUARD);*p=&guard;return guardpresent;}} player,guardactor,other;
Actor* CTCVillage::GetGuardWarningHero(){return otherguard?&other:0;}
void CTCVillage::SetGuardWarningHero(Actor* a){check(a==&guardactor);++guardset;}
static bool IS_THING_ALIVE(Actor* a){return a && a->alive;}
namespace NCreatureAI {bool IsThingRunningAwayFromThing(Actor& a,Actor& b,C3DVector p){check(&a==&player && &b==&guardactor && p.x==player.pos.x);++runchecks;return runs(tick);}}
static int* GetPWorldSeed(){static int worldseed;return &worldseed;}
static float GFFloatRandom(float max,int* p){check(max==1&&p==GetPWorldSeed());++randoms;return pick(320+tick,4)*0.25f;}
struct CAIStateGroup_MinionGuardReactToCriminal { Actor* PHero; int WarningsGiven,FrameWarningStarted,DeedLastWarnedAbout;C3DVector PositionPlayerWasWhenWarned;
 Actor& GetCreature(){return guardactor;}void Terminate(){++terminated;}void Update();
 EAIActionReturn AIAction_MoveToThingProximity(int a,Actor* p,float d,bool& finished,int mode,bool b1,bool b2,bool b3,bool b4,float f){check(a==ACTION_REPEAT_CALLBACK&&p==&player&&d==3&&mode==AI_MOVEMENT_RUNNING&&b1&&!b2&&b3&&b4&&f==0);++tick;++move;frame+=jump;finished=true;check(tick<=9);return (EAIActionReturn)point(3*tick);}
 EAIActionReturn AIAction_MoveToPosProximity(int a,C3DVector target,float d,bool& finished,int mode,bool b1,bool b2,bool b3,int v,bool b4,float f){check(a==ACTION_REPEAT_CALLBACK&&nearby&&fabs(target.x-player.pos.x-4.8)<0.001&&target.y==player.pos.y&&target.z==player.pos.z&&d==2.4f&&finished&&mode==AI_MOVEMENT_WALKING&&!b1&&!b2&&b3&&!v&&b4&&f==0);++step;return (EAIActionReturn)point(3*tick+1);}
 EAIActionReturn AIAction_LookAtThing(int a,Actor* p,int n,float angle){check(a==ACTION_REPEAT_CALLBACK&&p==&player&&n==11&&angle==0.0027777f);++look;return (EAIActionReturn)point(3*tick+2);}
 EAIActionReturn AIAction_SpeakAISpeechToTarget(int a,Actor* g,Actor* h,const CCharString& s,bool b,int duration){check(a==ACTION_REPEAT_UNTIL_FINISHED&&g==&guardactor&&h==&player&&b&&duration==147);speeches.push_back(s);++spoken;return stops(tick)?AI_ACTION_TERMINATE_PROCESS:AI_ACTION_FINISHED;}
};
namespace {
CCharString HEED_STRING("TEXT_AI_GUARD_HEED_WARNING");
CCharString IGNORE_STRING("TEXT_AI_GUARD_IGNORE_WARNING");
CCharString RUN_AWAY_STRING("TEXT_AI_GUARD_RUN_AWAY_WHILE_WARNING");
CCharString SEEN_MINOR_CRIME_STRING("TEXT_AI_GUARD_SEEN_MINOR_CRIME");
CCharString SEEN_MODERATE_CRIME_STRING("TEXT_AI_GUARD_SEEN_MODERATE_CRIME");
CCharString SEEN_SERIOUS_CRIME_STRING("TEXT_AI_GUARD_SEEN_SERIOUS_CRIME");
}

void CAIStateGroup_MinionGuardReactToCriminal::Update()
{
    if (IS_THING_ALIVE(PHero)) {
        CTCVillageMember* ptc_village_member;
        GetCreature().GetTC(TCI_VILLAGE_MEMBER, &ptc_village_member);
        CTCVillage* pvillage = ptc_village_member->GetPVillage();
        if (pvillage) {
            long time_since_last_warnings = CWorld::GetFrame() - FrameWarningStarted;
            WarningsGiven += time_since_last_warnings / (CMainGameComponent::GetConstantFPS() * -20);
            if (WarningsGiven <= 0) WarningsGiven = 0;
            FrameWarningStarted = CWorld::GetFrame();
            bool must_not_run_when_warned = false;
            long deed_being_warned = -1;
            CCharString warning_string = pvillage->GetWarningStringForCurrentCrimes(0,must_not_run_when_warned,deed_being_warned);
            if (deed_being_warned != -1 && deed_being_warned != DeedLastWarnedAbout) {
                WarningsGiven=0;
                DeedLastWarnedAbout=deed_being_warned;
            }
            warning_string=pvillage->GetWarningStringForCurrentCrimes(WarningsGiven,must_not_run_when_warned,deed_being_warned);
            CCharString final_string;
            if (warning_string!="") {
                if (IS_THING_ALIVE(pvillage->GetGuardWarningHero())) { Terminate();return; }
                pvillage->SetGuardWarningHero(&GetCreature());
                PositionPlayerWasWhenWarned=PHero->GetPos();
                long frame_for_next_warning=CWorld::GetFrame();
                do {
                    bool finished_calculating_route;
                    EAIActionReturn ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PHero,3.0f,finished_calculating_route,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f);
                    if(ret==AI_ACTION_FINISHED) {
                        C3DVector dir=GetCreature().GetPos()-PHero->GetPos();
                        if(dir.GetMagnitude2D()<2.39f) {
                            dir.Normalise();
                            C3DVector target=PHero->GetPos()+dir*2.4f*2.0f;
                            ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,target,2.4f,finished_calculating_route,AI_MOVEMENT_WALKING,false,false,true,0,true,0.0f);
                        }
                        if(ret==AI_ACTION_FINISHED) ret=AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,PHero,11,0.0027777f);
                    }
                    if(ret==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
                    if(must_not_run_when_warned&&NCreatureAI::IsThingRunningAwayFromThing(*PHero,GetCreature(),PositionPlayerWasWhenWarned)) {final_string=RUN_AWAY_STRING;break;}
                    long frames_overdue_from_warning=CWorld::GetFrame()-frame_for_next_warning;
                    if((ret==AI_ACTION_FINISHED&&frames_overdue_from_warning>=0)||frames_overdue_from_warning>CMainGameComponent::GetConstantFPS()*4) {
                        if(warning_string=="") {final_string=IGNORE_STRING;break;}
                        if(AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED,&GetCreature(),PHero,warning_string,true,147)==AI_ACTION_TERMINATE_PROCESS){Terminate();return;}
                        ++WarningsGiven;
                        frame_for_next_warning += CMainGameComponent::GetConstantFPS()*5;
                        frame_for_next_warning += GFFloatRandom(1.0f,GetPWorldSeed())*(CMainGameComponent::GetConstantFPS()*5);
                        deed_being_warned=-1;
                        warning_string=pvillage->GetWarningStringForCurrentCrimes(0,must_not_run_when_warned,deed_being_warned);
                        if(deed_being_warned!=-1 && deed_being_warned!=DeedLastWarnedAbout){WarningsGiven=0;DeedLastWarnedAbout=deed_being_warned;}
                        warning_string=pvillage->GetWarningStringForCurrentCrimes(WarningsGiven,must_not_run_when_warned,deed_being_warned);
                    }
                } while(pvillage->IsHeroCriminal());
                if(!pvillage->IsHeroCriminal()) final_string=HEED_STRING;
            } else {
                CTCGuard* ptc_guard;
                if(GetCreature().GetTC(TCI_GUARD,&ptc_guard)) {
                    long time_since_seen_crime=CWorld::GetFrame()-ptc_guard->GetFrameLastCrimeSeen();
                    if(time_since_seen_crime<CMainGameComponent::GetConstantFPS()*3) {
                        switch(ptc_guard->GetLastCrimeSeenSeverity()) {
                        case CRIME_SEVERITY_MINOR:final_string=SEEN_MINOR_CRIME_STRING;break;
                        case CRIME_SEVERITY_MODERATE:final_string=SEEN_MODERATE_CRIME_STRING;break;
                        case CRIME_SEVERITY_SERIOUS:final_string=SEEN_SERIOUS_CRIME_STRING;break;
                        }
                    }
                }
            }
            if(final_string!="") AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED,&GetCreature(),PHero,final_string,true,147);
            if(pvillage->IsHeroCriminal()) pvillage->SetHeroAsEnemyBecauseOfCrime();
        }
    }
    Terminate();
}
static void run(){
 tick=queries=marked=terminated=guardset=move=step=look=randoms=spoken=runchecks=0;speeches.clear();
 start=frame=15000;fps=10+pick(1,3)*5;limit=pick(2,9);jump=pick(3,101);severity=pick(4,4);crimeage=3*fps+int(pick(5,5))-2;
 guardpresent=pick(6,5)!=0;villagepresent=pick(7,10)!=0;otherguard=pick(8,7)==0;other.alive=pick(9,4)!=0;emptyfirst=pick(10,4)==0;nearby=pick(11,2)!=0;
 player.alive=pick(12,10)!=0;player.pos=C3DVector(6,2,4);guardactor.pos=C3DVector(nearby?7:10,2,4);
 initialWarnings=pick(13,6);initialDeed=int(pick(14,4))-1;elapsed=pick(15,1000);
 CAIStateGroup_MinionGuardReactToCriminal s;s.PHero=pick(16,30)?&player:0;s.WarningsGiven=initialWarnings;s.DeedLastWarnedAbout=initialDeed;s.FrameWarningStarted=start-elapsed;s.PositionPlayerWasWhenWarned=C3DVector(-1);
 int w=initialWarnings,d=initialDeed,t=0,q=0,m=0,g=0,p=0,l=0,r=0,rc=0;
 bool valid=s.PHero&&player.alive&&villagepresent,early=false;
 std::vector<std::string> expected;
 if(valid){
  w-=elapsed/(fps*20);if(w<0)w=0;int round=0;int nd=levelDeed(round);if(nd>=0&&nd!=d){w=0;d=nd;}q=2;
  if(emptyfirst){if(guardpresent&&crimeage<3*fps&&severity>0){const char* tags[]={"","TEXT_AI_GUARD_SEEN_MINOR_CRIME","TEXT_AI_GUARD_SEEN_MODERATE_CRIME","TEXT_AI_GUARD_SEEN_SERIOUS_CRIME"};expected.push_back(tags[severity]);}}
  else if(otherguard&&other.alive)early=true;
  else {
   g=1;int due=start;std::string last;
   for(t=1;;++t){
    int action=point(3*t);if(action==2&&nearby){++p;action=point(3*t+1);}if(action==2){++l;action=point(3*t+2);}
    if(action==1){early=true;break;}
    if(must(round)){++rc;if(runs(t)){last="TEXT_AI_GUARD_RUN_AWAY_WHILE_WARNING";break;}}
    int now=start+t*jump;
    if((action==2&&now>=due)||now>due+4*fps){
     if(emptyAt(round)){last="TEXT_AI_GUARD_IGNORE_WARNING";break;}
     expected.push_back(utter(round,w));if(stops(t)){early=true;break;}
     ++r;++w;due+=5*fps+int(5*fps*pick(320+t,4)/4);
     ++round;q+=2;nd=levelDeed(round);if(nd>=0&&nd!=d){w=0;d=nd;}
    }
    if(t>=limit)break;
   }
   if(!early){if(t>=limit)last="TEXT_AI_GUARD_HEED_WARNING";if(last.size())expected.push_back(last);}
  }
  m=!early&&t<limit;
 }
 s.Update();
 check(terminated==1&&tick==t&&move==t&&step==p&&look==l&&randoms==r&&runchecks==rc&&queries==q&&marked==m&&guardset==g&&speeches==expected);
 check(s.WarningsGiven==w&&s.DeedLastWarnedAbout==d&&s.FrameWarningStarted==(valid?start:start-elapsed)&&s.PositionPlayerWasWhenWarned.x==(g?6:-1));
}
int main(){for(seed=0;seed<65536;++seed)run();puts("00932b50_TEST PASS");return 0;}