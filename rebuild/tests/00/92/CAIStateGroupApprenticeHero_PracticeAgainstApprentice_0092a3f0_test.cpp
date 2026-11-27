#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
struct CCharString {std::string text;CCharString(const char*s=""):text(s){}};
struct C3DVector {float x,y,z;C3DVector(float a=0,float b=0,float c=0):x(a),y(b),z(c){}};
enum {ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2,AI_MOVEMENT_WALKING=3,MESSAGE_EVENT_APPRENTICE_PRACTICE=47};
enum EAIActionReturn { AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4 };
struct CThing {int id;CThing(int n=0):id(n){}};
struct CThingGameObject:CThing {C3DVector pos;const C3DVector&GetPos()const{return pos;}};
struct CThingAICreature:CThing {CThingAICreature(int n):CThing(n){}};
template<class T>struct CIntelligentPointer {T*p;CIntelligentPointer(T*q):p(q){}};
struct CMessageEvent{};
struct CMessageEventFilter_IsOfType {int n;CMessageEventFilter_IsOfType(int v):n(v){}};
struct CMessageEventFilter_IsOriginatedByThing {int id;CMessageEventFilter_IsOriginatedByThing(const CThing&t):id(t.id){}};
struct CMessageEventFilter_IsTargettedAtThing {int id;CMessageEventFilter_IsTargettedAtThing(const CThing&t):id(t.id){}};
struct Filter{int kind,from,to;};
Filter GFFilter_Bind3(CMessageEventFilter_IsOfType a,CMessageEventFilter_IsOriginatedByThing b,CMessageEventFilter_IsTargettedAtThing c){Filter f={a.n,b.id,c.id};return f;}
struct Settings {int role,wait,stop,interrupt,external,route,fightBlock,retry,event,handle,rounds,style,step;bool marker;};
struct Trace {int look,moved,checks,idles,sent,created,destroyed,interact,find,terminated,randCalls,waiting;std::vector<int> roles;Trace():look(0),moved(0),checks(0),idles(0),sent(0),created(0),destroyed(0),interact(0),find(0),terminated(0),randCalls(0),waiting(0){}};
static Settings cfg;static Trace tr;static CThingAICreature self(19),other(71),third(22);static CThingGameObject markerObj;static int now;static bool criticalNow;static CMessageEvent message;
struct Manager {
 const CMessageEvent* FindMostRecentMessage(const Filter&f) {assert(criticalNow&&f.kind==47&&f.to==19&&f.from==71);++tr.find;return tr.find==cfg.event?&message:0;}
 void AddMessage(int kind,int ms,const C3DVector&p,const CThing*a,const CThing*b,float r1,float r2,long d){assert(kind==47&&ms==1000&&!p.x&&!p.y&&!p.z&&a==&self&&b==&other&&!r1&&!r2&&!d);++tr.sent;}
};
struct CWorld {Manager manager;Manager& GetMessageEventManager(){return manager;}static long GetFrame(){return now;}};
struct CMainGameComponent {static long GetConstantFPS(){return 2;}};
static unsigned long seed;static CWorld world;
float GFFloatRandom(float max,unsigned long*s){assert(max==15&&s==&seed);return 6.5f;}
unsigned long GFRandom(unsigned long bound,unsigned long*s){assert(s==&seed&&bound==(tr.randCalls?10u:2u));++tr.randCalls;return bound==2?cfg.style:cfg.rounds;}
struct CAIStateGroup_ApprenticeHero {
 static CThingAICreature *PracticeOpponent1,*PracticeOpponent2; CThingAICreature*PracticeOpponent;CCharString Interaction;bool Submissive,Critical;long NumFights;
 CAIStateGroup_ApprenticeHero():PracticeOpponent(&other),Submissive(false),Critical(false),NumFights(0){}
 bool PracticeAgainstApprentice();
 CWorld&GetWorld(){return world;}unsigned long*GetPWorldSeed(){return &seed;}
 CThingAICreature&GetCreature(){return self;}
 CThingGameObject*GetObjectWithScriptName(const CCharString& n){assert(n.text=="MeleePractice");++tr.look;return cfg.marker?&markerObj:0;}
 EAIActionReturn AIAction_MoveToPosProximity(int rep,C3DVector pos,float distance,bool&got,int movement,bool a,bool b,bool c,const CThing*avoid,bool d,float speed){assert(rep==1&&pos.x==3&&distance==11.5f&&movement==3&&a&&!b&&c&&!avoid&&d&&!speed);++tr.moved;got=cfg.route==2;return (EAIActionReturn)cfg.route;}
 bool IsStopping(){++tr.checks;return cfg.stop>0&&tr.checks>=cfg.stop||tr.terminated!=0;}
 void Terminate(){++tr.terminated;}
 void AIAction_DoNothing(){
  ++tr.idles;
  if(!Critical&&!tr.created){
   ++tr.waiting;now+=cfg.step;
   if(tr.waiting==cfg.wait){if(PracticeOpponent1==&self)PracticeOpponent2=&other;else if(PracticeOpponent2==&third)PracticeOpponent2=0;}
  }
 }
 void CreateItem(const CCharString&ob,const CCharString&slot){assert(Critical&&ob.text=="OBJECT_IRON_LONGSWORD"&&slot.text=="CARRY_SLOT_RIGHT_HAND");criticalNow=true;++tr.created;}
 void DestroyItem(const CCharString&slot){assert(Critical&&criticalNow&&slot.text=="CARRY_SLOT_RIGHT_HAND");criticalNow=false;++tr.destroyed;}
 EAIActionReturn AIAction_InteractWith(int rep,CIntelligentPointer<CThingAICreature> t,CCharString name,int frames,bool sub){assert(Critical&&criticalNow&&rep==2&&t.p==&other&&frames==30);assert(name.text=="VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_01"||name.text=="VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_02");tr.roles.push_back(sub);++tr.interact;if(tr.interact==cfg.fightBlock)return (EAIActionReturn)cfg.handle;if(tr.interact<=cfg.retry)return AI_ACTION_IN_PROGRESS;return AI_ACTION_FINISHED;}
};
CThingAICreature*CAIStateGroup_ApprenticeHero::PracticeOpponent1=0;CThingAICreature*CAIStateGroup_ApprenticeHero::PracticeOpponent2=0;
bool CAIStateGroup_ApprenticeHero::PracticeAgainstApprentice()
{
    CThingGameObject* marker = GetObjectWithScriptName("MeleePractice");
    if (!marker)
        return true;
    bool finished_calculating_route;
    if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, marker->GetPos(), GFFloatRandom(15.0f, GetPWorldSeed()) + 5.0f, finished_calculating_route, AI_MOVEMENT_WALKING,
            true, false, true, 0, true, 0.0f) != AI_ACTION_FINISHED)
        return false;
    static long num_seconds_to_wait = 40;
    long frame_to_give_up_waiting_and_go_do_something_less_boring_instead = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * num_seconds_to_wait;
    do {
        if (IsStopping())
            return false;
        if (!PracticeOpponent1) {
            PracticeOpponent1 = &GetCreature();
            do {
                if (PracticeOpponent2) {
                    PracticeOpponent = PracticeOpponent2;
                    PracticeOpponent1 = PracticeOpponent2 = 0;
                    break;
                }
                AIAction_DoNothing();
                if (frame_to_give_up_waiting_and_go_do_something_less_boring_instead < CWorld::GetFrame())
                    Terminate();
            } while (!IsStopping());
            if (IsStopping()) {
                if (PracticeOpponent2) {
                    GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_APPRENTICE_PRACTICE, 1000, C3DVector(0.0f, 0.0f, 0.0f), &GetCreature(), PracticeOpponent, 0, 0, 0);
                    PracticeOpponent2 = 0;
                }
                PracticeOpponent1 = 0;
                return false;
            }
            break;
        } else {
            if (!PracticeOpponent2) {
                PracticeOpponent2 = &GetCreature();
                PracticeOpponent = PracticeOpponent1;
                break;
            }
            AIAction_DoNothing();
            if (frame_to_give_up_waiting_and_go_do_something_less_boring_instead < CWorld::GetFrame())
                Terminate();
        }
    } while (!IsStopping());
    if (IsStopping())
        return false;
    static long num_fights;
    static CCharString interaction("");
    if (PracticeOpponent1 && PracticeOpponent2) {
        Interaction = interaction = GFRandom(2, GetPWorldSeed()) == 0 ? "VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_01"
                                              : "VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_02";
        Submissive = true;
        NumFights = num_fights = GFRandom(10, GetPWorldSeed()) + 10;
    } else {
        Interaction = interaction;
        Submissive = false;
        NumFights = num_fights;
    }
    EAIActionReturn ret = AI_ACTION_FAILED;
    Critical = true;
    CreateItem("OBJECT_IRON_LONGSWORD", "CARRY_SLOT_RIGHT_HAND");
    const CMessageEvent* event = 0;
    for (int i = 0; i < NumFights; ++i) {
        if (i == NumFights / 2)
            Submissive = !Submissive;
        for (;;) {
            ret = AIAction_InteractWith(ACTION_REPEAT_CALLBACK, CIntelligentPointer<CThingAICreature>(PracticeOpponent), Interaction, 30, Submissive);
            event = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_APPRENTICE_PRACTICE), CMessageEventFilter_IsOriginatedByThing(*PracticeOpponent), CMessageEventFilter_IsTargettedAtThing(GetCreature())));
            if (ret == AI_ACTION_IN_PROGRESS && !event && !IsStopping())
                continue;
            break;
        }
        if (ret != AI_ACTION_FINISHED || event)
            break;
        AIAction_DoNothing();
    }
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    Critical = false;
    if (!event && (ret != AI_ACTION_FINISHED || IsStopping())) {
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_APPRENTICE_PRACTICE, 1000, C3DVector(0.0f, 0.0f, 0.0f), &GetCreature(), PracticeOpponent, 0, 0, 0);
        AIAction_DoNothing();
        return false;
    }
    AIAction_DoNothing();
    return true;
}
static int previousRounds,previousStyle;
static void run(Settings s){
 cfg=s;tr=Trace();now=100;criticalNow=false;markerObj.pos.x=3;CAIStateGroup_ApprenticeHero ai;
 CThingAICreature*&one=ai.PracticeOpponent1;CThingAICreature*&two=ai.PracticeOpponent2;
 one=s.role==0?0:&other;two=s.role==2?&third:0;
 bool actual=ai.PracticeAgainstApprentice();assert(tr.look==1&&tr.moved==int(s.marker));
 if(!s.marker){assert(actual&&tr.checks==0&&tr.created==0);return;}
 if(s.route!=2){assert(!actual&&tr.checks==0&&tr.created==0);return;}
 int checks=0,waits=0,frame=100,term=0,sent=0;bool paired=false;int p1=s.role?71:0,p2=s.role==2?22:0;bool stop=false;
 // Model the two-role rendezvous by advancing the scripted scheduler at each idle.
 for(;;){
  stop=++checks>=s.stop&&s.stop>0||term!=0;if(stop)break;
  bool first=p1==0;
  if(first)p1=19;
  do{
   if(first&&p2){p2=p1=0;paired=true;break;}
   if(!first&&!p2){p2=19;paired=true;break;}
   ++waits;frame+=s.step;
   if(waits==s.wait){if(first)p2=71;else if(p2==22)p2=0;}
   if(frame>180)++term;
   stop=++checks>=s.stop&&s.stop>0||term!=0;
   if(!first||stop)break;
  }while(true);
  if(first){stop=++checks>=s.stop&&s.stop>0||term!=0;if(stop){if(p2)++sent;p1=p2=0;break;}}
  if(paired||stop){stop=++checks>=s.stop&&s.stop>0||term!=0;break;}
 }
 assert(tr.waiting==waits&&tr.terminated==term&&(!one?0:one->id)==p1&&(!two?0:two->id)==p2);
 if(!paired||stop){assert(!actual&&tr.created==0&&tr.destroyed==0&&tr.sent==sent&&tr.checks==checks);return;}
 bool leader=p1!=0&&p2!=0;
 if(leader){previousRounds=10+s.rounds;previousStyle=s.style;}
 assert(ai.NumFights==previousRounds&&tr.randCalls==(leader?2:0));
 if(previousRounds)assert(ai.Interaction.text==(previousStyle?"VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_02":"VILLAGER_INTERACTION_HERO_APPRENTICE_PRACTICE_FIGHT_01"));
 int rounds=0,calls=0,completed=0,out=3;bool messageFound=false,sub=leader;
 while(rounds<previousRounds){
  if(rounds==previousRounds/2)sub=!sub;
  for(;;){
   assert(calls<(int)tr.roles.size()&&tr.roles[calls]==int(sub));++calls;
   out=calls==s.fightBlock?s.handle:calls<=s.retry?4:2;messageFound=calls==s.event;
   if(out!=4||messageFound)break;
   bool ended=++checks>=s.stop&&s.stop>0||term!=0;if(ended)break;
  }
  if(out!=2||messageFound)break;
  ++completed;++rounds;
 }
 bool endingStop=false;
 if(!messageFound&&out==2)endingStop=++checks>=s.stop&&s.stop>0||term!=0;
 bool failure=!messageFound&&(out!=2||endingStop);
 assert(actual==!failure&&tr.interact==calls&&tr.find==calls&&tr.idles==waits+completed+1);
 assert(tr.sent==sent+int(failure)&&tr.created==1&&tr.destroyed==1&&!ai.Critical&&tr.checks==checks);
}
int main(){
 Settings warm={1,1,0,0,0,2,0,0,0,2,0,0,1,true};run(warm);
 unsigned random=178191u;
 for(int i=0;i<65536;++i){
  random=1664525u*random+1013904223u;Settings s=warm;
  s.role=i%3;s.wait=1+((random>>8)%4);s.step=1+((random>>14)%50);
  s.stop=(i%5)?0:1+((random>>6)%16);s.route=(i%11)?2:(i%4)+1;s.marker=i%31!=0;
  s.fightBlock=(i%4)?0:1+((random>>5)%15);s.handle=i%3?3:1;s.retry=(random>>23)%4;s.event=(i%7)?0:1+((random>>17)%15);s.style=(random>>30)&1;s.rounds=(random>>19)%10;run(s);
 }puts("0092a3f0_TEST PASS");return 0;
}