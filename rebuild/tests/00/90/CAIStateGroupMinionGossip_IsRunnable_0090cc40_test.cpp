#include <stdio.h>
struct C3DVector { float x,y,z; };
enum EMessageEventType { MESSAGE_EVENT_REQUEST_GOSSIP=44, MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST=45 };
#include "engine/NCreatureMode.h"
struct CTCGossip {};
struct CTCCreatureModeManager {
 unsigned long mode;
 CTCCreatureModeManager& GetCurrentMode(){return *this;}
 unsigned long GetType()const{return mode;}
};
struct CThingAICreature {
 C3DVector pos; CTCCreatureModeManager mode; CTCGossip component;
 bool gossip,valid,sight;
 const C3DVector& GetPos()const{return pos;}
 const C3DVector& PeekPos()const{return pos;}
 bool GetTC(CTCCreatureModeManager** p){*p=&mode;return true;}
 bool GetTC(CTCGossip** p){*p=gossip?&component:0;return gossip;}
};
struct CMessageEvent { EMessageEventType type; CThingAICreature* who; };
struct CMessageEventFilter_IsOfType {
 EMessageEventType t;
 CMessageEventFilter_IsOfType(EMessageEventType n):t(n){}
 bool operator()(const CMessageEvent& e)const{return e.type==t;}
};
struct CMessageEventFilter_IsOriginatedByThing {
 const CThingAICreature& who;
 CMessageEventFilter_IsOriginatedByThing(const CThingAICreature& w):who(w){}
 bool operator()(const CMessageEvent& e)const{return e.who==&who;}
};
struct CThingFilter_IsValidGossipee {
 const CThingAICreature& self;
 CThingFilter_IsValidGossipee(const CThingAICreature& w):self(w){}
 bool operator()(const CThingAICreature& w)const{return &w!=&self && w.valid;}
};
struct CThingFilter_IsNavLineOfSightToThing {
 CThingFilter_IsNavLineOfSightToThing(const CThingAICreature&){}
 bool operator()(const CThingAICreature& w)const{return w.sight;}
};
struct CThingFilter_In2DRange {
 const C3DVector& center; float range;
 CThingFilter_In2DRange(const C3DVector& p,float r):center(p),range(r){}
 bool operator()(const CThingAICreature& w)const {
  float x=w.pos.x-center.x, y=w.pos.y-center.y;return x*x+y*y<range*range;
 }
};
template<class T> struct CMessageEventFilter_ThingSearchFilter {
 const T& filter; CMessageEventFilter_ThingSearchFilter(const T& f):filter(f){}
 bool operator()(const CMessageEvent& e)const{return e.who && filter(*e.who);}
};
template<class A,class B,class C> struct Filter3 {
 const A& a; const B& b; const C& c;
 Filter3(const A& x,const B& y,const C& z):a(x),b(y),c(z){}
 bool operator()(const CMessageEvent& e)const{return a(e)&&b(e)&&c(e);}
};
template<class A,class B,class C> Filter3<A,B,C> GFFilter_Bind3(const A& a,const B& b,const C& c){return Filter3<A,B,C>(a,b,c);}
template<class A,class B,class C,class D> struct Filter4:Filter3<A,B,C> {
 const D& d; Filter4(const A& x,const B& y,const C& z,const D& q):Filter3<A,B,C>(x,y,z),d(q){}
 bool operator()(const CMessageEvent& e)const{return Filter3<A,B,C>::operator()(e)&&d(e);}
};
template<class A,class B,class C,class D> Filter4<A,B,C,D> GFFilter_Bind4(const A& a,const B& b,const C& c,const D& d){return Filter4<A,B,C,D>(a,b,c,d);}
struct CMainGameComponent { static int GetConstantFPS(){return 4;} };
struct MessageQueue {
 CMessageEvent data[6]; int n,removed,added,searches; bool addok; const CThingAICreature* expected;
 template<class F> const CMessageEvent* FindMostRecentMessage(const F& f) {
  ++searches;
  for(int i=0;i<n;++i) if(f(data[i])) return &data[i];
  return 0;
 }
 void RemoveMessage(const CMessageEvent* p){removed=static_cast<int>(p-data);}
 void AddMessage(EMessageEventType t,int duration,const C3DVector& p,CThingAICreature* who,
                 CThingAICreature* dest,float sight,float hearing,long extra){
  ++added;addok=t==MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST && duration==16 && who==expected &&
                &p==&expected->pos && !dest && sight==16.0f && hearing==16.0f && !extra;
 }
};
struct CWorld {
 static long frame;
 static long GetFrame(){return frame;}
 MessageQueue queue;
 MessageQueue& GetMessageEventManager(){return queue;}
};
long CWorld::frame=0;
struct CAIStateGroup_MinionGossip {
 CWorld world; CThingAICreature self; long FrameOfLastGossip; bool use_sight;
 CWorld& GetWorld(){return world;} CThingAICreature& GetCreature(){return self;}
 long GetTimeToRespond() const{return 2;} long GetTimeToInitiate() const{return 4;}
 bool UseLineOfSight()const{return use_sight;}
 bool IsRunnable();
};
bool CAIStateGroup_MinionGossip::IsRunnable()
{
    CTCCreatureModeManager* ptc_mode_manager;
    GetCreature().GetTC(&ptc_mode_manager);
    unsigned long mode = ptc_mode_manager->GetCurrentMode().GetType();
    if (mode == NCreatureMode::MODE_CARRYING_GENERIC_BOX || mode == NCreatureMode::MODE_CARRYING_GOODS_STAND ||
        mode == NCreatureMode::MODE_CARRYING_PLATE_STAND || mode == NCreatureMode::MODE_HOLD_JUG)
        return false;
    CTCGossip* gossip;
    if (!GetCreature().GetTC(&gossip))
        return false;
    long time = (CWorld::GetFrame() - FrameOfLastGossip) / CMainGameComponent::GetConstantFPS();
    if (time > GetTimeToRespond()) {
        const CMessageEvent* event;
        if (UseLineOfSight()) {
            CThingFilter_IsNavLineOfSightToThing line_of_sight(GetCreature());
            event = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind4(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f)),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>(line_of_sight)));
        } else
            event = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f))));
        if (event) return true;
    }
    const CMessageEvent* event_request;
    if (UseLineOfSight()) {
        CThingFilter_IsNavLineOfSightToThing line_of_sight(GetCreature());
        event_request = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind4(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f)),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>(line_of_sight)));
    } else
        event_request = GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(
                CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature())),
                CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f))));
    if (time > GetTimeToInitiate() && event_request) {
        GetWorld().GetMessageEventManager().RemoveMessage(event_request);
        return true;
    }
    if (!GetWorld().GetMessageEventManager().FindMostRecentMessage(GFFilter_Bind3(
            CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST),
            CMessageEventFilter_IsOriginatedByThing(GetCreature()),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f))))) {
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST,
            CMainGameComponent::GetConstantFPS()*4, GetCreature().PeekPos(), &GetCreature(), 0, 16.0f, 16.0f, 0);
    }
    return false;
}

int main() {
 const int modes[]={0,NCreatureMode::MODE_CARRYING_GENERIC_BOX,NCreatureMode::MODE_CARRYING_GOODS_STAND,
                    NCreatureMode::MODE_CARRYING_PLATE_STAND,NCreatureMode::MODE_HOLD_JUG,12};
 const long elapsed[]={-4,0,8,11,12,16,19,20,36};
 for(int m=0;m<6;++m)for(int g=0;g<2;++g)for(int l=0;l<2;++l)for(int f=0;f<9;++f)
  for(int scenario=0;scenario<256;++scenario){
   CAIStateGroup_MinionGossip ai; CThingAICreature actors[4];MessageQueue& q=ai.world.queue;
   ai.self.mode.mode=modes[m];ai.self.gossip=g!=0;ai.self.pos.x=ai.self.pos.y=ai.self.pos.z=0;
   ai.use_sight=l!=0;ai.FrameOfLastGossip=40;CWorld::frame=40+elapsed[f];
   q.n=scenario%7;q.expected=&ai.self;q.removed=-1;q.added=q.searches=0;q.addok=false;
   int req=-1,offer=-1;bool own=false;
   for(int j=0;j<4;++j){
    actors[j].valid=((scenario+j)%3)!=0;actors[j].sight=((scenario>>j)&1)!=0;
    actors[j].pos.x=((scenario>>j)&3)*8.0f;actors[j].pos.y=0;actors[j].pos.z=1024;
   }
   for(int i=0;i<q.n;++i){
    int k=(i+scenario)%6;CThingAICreature* who=k==5?0:k==4?&ai.self:&actors[k];
    bool request=((scenario>>(i%6))&1)!=0;
    q.data[i].who=who;q.data[i].type=request?MESSAGE_EVENT_REQUEST_GOSSIP:MESSAGE_EVENT_REQUEST_GOSSIP_REQUEST;
    if(!request && who==&ai.self)own=true;
    if(who && who!=&ai.self && who->valid && (!l || who->sight) && who->pos.x<16){
     if(request && req<0)req=i;
     if(!request && offer<0)offer=i;
    }
   }
   const bool can=(m==0 || m==5) && g;const long seconds=elapsed[f]/4;
   const bool respond=can && seconds>2 && req>=0;
   const bool take=can && !respond && seconds>4 && offer>=0;
   const bool post=can && !respond && !take && !own;
   const int searches=can?int(seconds>2)+int(!respond)+int(!respond&&!take):0;
   if(ai.IsRunnable()!=(respond||take) || q.removed!=(take?offer:-1) || q.added!=int(post) ||
      (post&&!q.addok) || searches!=q.searches) return 1;
  }
 puts("0090cc40_TEST PASS");return 0;
}