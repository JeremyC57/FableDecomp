#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <deque>
#include <algorithm>
namespace std { template<class T>const T& max(const T& a,const T& b){return a<b?b:a;} template<class T>const T& min(const T& a,const T& b){return a<b?a:b;} }
struct State {
 bool waiting,sub,dead,hero,heroDead,stats,village,events,criminal,look,sees,target,attacker,property,region,crimes,pending,speaks;
 long timeAgo,frame,prior,bribe,money,fine,greeting; float health;
};
static State in;
static int got[64],gn;static long sentFine,reward,paid,throwsCount,seen;static int args[64];
static void check(bool v){if(!v)abort();}
enum Ev{KICK,REMOVE,AMNESTY,APOLOGY,SEEN,GREET,CHARGE,PAY,SCORE,PREP,CLEAR,FORMAT,HEALTH,FADE,SUBTITLE,REPORT,EMOTE,REPLACE};
static void ev(Ev e,long p=0){check(gn<64);args[gn]=p;got[gn++]=e;}
struct CWideString{long v; CWideString(long i=0):v(i){} CWideString& operator+=(const wchar_t*){return *this;} CWideString& operator+=(const CWideString&){return *this;}};
struct CCharString{long v;CCharString(long i):v(i){} CWideString ToWideString()const{return CWideString(v);}};
static CCharString GFIntToCharString(long n){return CCharString(n);}
struct Name{bool apology;bool operator==(const char* text)const{check(!strcmp(text,"EXPRESSION_APOLOGY"));return apology;}};
struct CExpressionDef{Name label;const Name& GetInstantiationName()const{return label;}};
template<class T>struct CDefPointer{T*p;T* Get()const{return p;}T* operator->()const{check(p!=0);return p;}};
template<class T>struct CCountedPointer{T*p;T& operator*()const{check(p!=0);return *p;}};
struct CRegionDef{bool UseCrimes;} def;
struct CRegion{CDefPointer<const CRegionDef> GetPRegionDef()const{CDefPointer<const CRegionDef>x={in.region?&def:0};return x;}}region;
struct CWorld{static long GetFrame(){return in.frame;}}world;
struct CWorldMap{long GetMapNumberAt(long x){check(x==8);return 3;}long GetRegionNumberMapIsIn(long x){check(x==3);return 4;}CRegion& GetRegion(long x){check(x==4);return region;}}worldmap;
namespace NEntityEvents{
struct CEventBase{virtual int GetType()const{return 8;}virtual ~CEventBase(){}};
struct CEventExpressionPerformedTo:CEventBase{CExpressionDef def;int GetType()const{return 3;}static int StatGetType(){return 3;}CDefPointer<const CExpressionDef> GetPExpressionDef()const{CDefPointer<const CExpressionDef>x={&def};return x;}};
struct CEventApologyReceived:CEventBase{bool accepted;explicit CEventApologyReceived(bool b):accepted(b){}int GetType()const{return 4;}};
}
static NEntityEvents::CEventBase other;
static NEntityEvents::CEventExpressionPerformedTo yesExpr,noExpr;
struct CTCEntityEvents{typedef std::list<CCountedPointer<NEntityEvents::CEventBase> > List; List events;
 const List& GetEvents()const{return events;}void append(NEntityEvents::CEventBase*p){CCountedPointer<NEntityEvents::CEventBase> x={p};events.push_back(x);}
 void RemoveEvent(NEntityEvents::CEventBase&e){for(List::iterator i=events.begin();i!=events.end();++i)if(&**i==&e){events.erase(i);ev(REMOVE);return;}abort();}
 void AddEvent(NEntityEvents::CEventApologyReceived*e){ev(APOLOGY,e->accepted);append(e);}
 void reset(){for(List::iterator i=events.begin();i!=events.end();++i)if((**i).GetType()==4)delete i->p;events.clear();append(&yesExpr);append(&other);append(&noExpr);append(&yesExpr);}
} events;
struct CTCVillage{long greeting;bool IsHeroCriminal()const{return in.criminal;}void RemoveApologiseableCrimes(long t){check(t>0);ev(AMNESTY,t);}void SetFramePlayerLastSeenByGuard(long t){ev(SEEN,t);seen=t;}float GetFrameForNextGuardGreeting()const{return (float)greeting;}void SetFrameForNextGuardGreeting(float t){greeting=(long)t;ev(GREET,greeting);}long GetFineAmountForCurrentCrimes(){return in.fine;}void PayFines(long f){ev(PAY,f);}void ClearCrimes(){ev(CLEAR);}} village;
struct CTCVillageMember{CTCVillage*GetPVillage(){return in.village?&village:0;}}member;
struct CTCHeroStats{void KickHeroToMostRecentSafeRegion(){ev(KICK);}long GetMoney()const{return in.money;}void AddMoney(long m,bool b){check(b);ev(CHARGE,m);}long GetFinePaid()const{return paid;}void SetFinePaid(long p){paid=p;}long GetTimesThrownOut()const{return throwsCount;}void SetTimesThrownOut(long n){throwsCount=n;}void PrepareToKickHeroToMostRecentSafeRegion(){ev(PREP);}}stats;
struct CTCHeroOnlineScoreboard{enum{OSA_TOTAL_FINES_PAID=36};void UpdateScoreForActivity(int a,float s,bool b){check(a==36&&!b);ev(SCORE,(long)s);}}score;
struct CTCEmoteIcon{enum{EMOTE_PRIORITY_BRIBE=6};void SetEmoteText(int n,const CWideString&w,float a,float b){check(n==6&&a==1&&b==0.3f);ev(EMOTE,w.v);}void SetReplaceable(bool b){check(b);ev(REPLACE);}}icon;
struct CThing;struct CTCPerceiveThing{const CThing*GetPerceivedThing();bool IsThingPerceived(){return in.sees;}}look;
struct CThing{bool dead;long GetPos()const{return 8;}bool GetTC(CTCVillageMember**p){*p=&member;return true;}bool GetTC(CTCEntityEvents**p){*p=&events;return in.events;}bool GetTC(CTCPerceiveThing**p){*p=&look;return in.look;}bool GetTC(CTCHeroStats**p){if(in.stats)*p=&stats;return in.stats;}bool GetTC(CTCHeroOnlineScoreboard**p){*p=&score;return true;}bool GetTC(CTCEmoteIcon**p){*p=&icon;return true;}};
template<class T> bool IS_THING_ALIVE(T*p){return p&&!p->dead;}
struct CThingCreatureBase:CThing{unsigned long GetProperty()const{return in.property?3:1;}}guard,attacker;
struct CThingPlayerCreature:CThingCreatureBase{float GetHealth(){return static_cast<float>(static_cast<double>(in.health));}float GetMaxHealth(){return 80;}const CThingCreatureBase* PeekPLastAttackedByCreature()const{return in.attacker?&attacker:0;}void ModifyHealth(float f,bool b){check(f==20.0f&&b);ev(HEALTH);}}hero;
const CThing* CTCPerceiveThing::GetPerceivedThing(){return in.target?&hero:0;}
namespace NCreatureAI{bool IsSpeaking(const CThingCreatureBase&g){check(&g==&guard);return in.speaks;}}
struct Player{CThingPlayerCreature*GetPControlledCreature()const{return in.hero?&hero:0;}}player;
struct Manager{Player& GetMainPlayer()const{return player;}}manager;
struct CRGBColour{CRGBColour(int r,int g,int b,int a){check(!r&&!g&&!b&&a==255);}};
struct Display{void StartScreenFadeOut(float a,float b,CRGBColour,float c){check(a==0.5f&&b==0.5f&&c==-1);ev(FADE);}}display;
struct Bank{CWideString GetTextBySymbol(const char*s){check(!strcmp(s,"TEXT_GUI_CRIME_FINE_KICKED_OUT"));return CWideString();}}bank;
namespace NGameText{bool ConstructString(CWideString& text,long n){text.v=n;ev(FORMAT,n);return true;}}
struct CMainGameComponent{static CMainGameComponent*Get(){static CMainGameComponent g;return &g;}const Manager&PeekPlayerManager(){return manager;}Bank&GetTextBank(){return bank;}Display&GetDisplayEngine(){return display;}long PeekCutSceneMainFont(){return 71;}static long GetConstantFPS(){return 30;}};
struct Subtitle{bool IsDisplayingText()const{return in.sub;}void SetText(long f,const CWideString&w,unsigned long flags,float time,bool a,bool b){check(f==71&&flags==10&&time==0&&!a&&b);ev(SUBTITLE,w.v);}}sub;
struct CPlayerGui{static CPlayerGui& Get(){static CPlayerGui g;return g;}Subtitle& GetSubtitleRenderer(){return sub;}};
struct CTCGuard{bool WaitingForRegionKickConfirmation;long FrameLastReceivedApology,FramePendingCrimesAdded,BribePool;std::deque<int>PendingCrimes;
 Manager&GetPlayerManager(){return manager;}CThing&GetParentThing(){return guard;}long GetTimeAgoThatCrimesCanBeApologisedFor(){return in.timeAgo;}CWorld&GetWorld(){return world;}CWorldMap&GetWorldMap(){return worldmap;}void ReportAllPendingCrimes(CTCVillage*v){check(v==(in.village?&village:0));ev(REPORT);}void FrameUpdate();};
const long MIN_FRAMES_BETWEEN_CRIMINALITY_AND_GREETING = 300;
enum { THING_CREATURE_PROPERTY_GUARD=3 };
void CTCGuard::FrameUpdate()
{
    if (WaitingForRegionKickConfirmation) {
        if (!CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
            WaitingForRegionKickConfirmation = false;
            CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroStats* ptc_hero_stats;
            if (IS_THING_ALIVE(phero) && phero->GetTC(&ptc_hero_stats))
                ptc_hero_stats->KickHeroToMostRecentSafeRegion();
        }
        return;
    }
    if (!IS_THING_ALIVE(&GetParentThing()))
        return;
    CTCVillageMember* ptc_village_member;
    GetParentThing().GetTC(&ptc_village_member);
    CTCVillage* pvillage=ptc_village_member->GetPVillage();
    CThingCreatureBase& guard=static_cast<CThingCreatureBase&>(GetParentThing());
    CTCEntityEvents* ptc_entity_events;
    if (guard.GetTC(&ptc_entity_events)) {
        const std::list<CCountedPointer<NEntityEvents::CEventBase> >& event_list = ptc_entity_events->GetEvents();
        std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator it=event_list.begin(), next_it=it;
        for (;it!=event_list.end();it=next_it) {
            ++next_it;
            NEntityEvents::CEventBase& event_base=**it;
            if (event_base.GetType() == NEntityEvents::CEventExpressionPerformedTo::StatGetType()) {
                NEntityEvents::CEventExpressionPerformedTo& devent=static_cast<NEntityEvents::CEventExpressionPerformedTo&>(event_base);
                CDefPointer<const CExpressionDef> expression_def=devent.GetPExpressionDef();
                if (expression_def->GetInstantiationName() == "EXPRESSION_APOLOGY") {
                    ptc_entity_events->RemoveEvent(event_base);
                    if (pvillage) {
                        long time_ago_that_crimes_will_be_removed = GetTimeAgoThatCrimesCanBeApologisedFor();
                        if (time_ago_that_crimes_will_be_removed > 0)
                            pvillage->RemoveApologiseableCrimes(time_ago_that_crimes_will_be_removed);
                        bool apology_accepted = !pvillage->IsHeroCriminal();
                        if (apology_accepted)
                            FrameLastReceivedApology = CWorld::GetFrame();
                        ptc_entity_events->AddEvent(new NEntityEvents::CEventApologyReceived(apology_accepted));
                    }
                }
            }
        }
    }
    if (pvillage) {
        CThingPlayerCreature* pplayer = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (pvillage->IsHeroCriminal()) {
            CThing* perceived_guard = &GetParentThing();
            CTCPerceiveThing* ptc_perceive_thing;
            if (perceived_guard->GetTC(&ptc_perceive_thing)) {
                if (ptc_perceive_thing->GetPerceivedThing() && ptc_perceive_thing->IsThingPerceived() && ptc_perceive_thing->GetPerceivedThing() == pplayer)
                    pvillage->SetFramePlayerLastSeenByGuard(CWorld::GetFrame());
            }
            long until = GetWorld().GetFrame() + MIN_FRAMES_BETWEEN_CRIMINALITY_AND_GREETING;
            long current = (long)pvillage->GetFrameForNextGuardGreeting();
            pvillage->SetFrameForNextGuardGreeting((float)std::max(current, until));
        }
        if (pplayer && pplayer->GetHealth() == 1.0f && pplayer->PeekPLastAttackedByCreature() && pplayer->PeekPLastAttackedByCreature()->GetProperty() == THING_CREATURE_PROPERTY_GUARD) {
            CWorldMap* worldMap = &GetWorldMap();
            const CRegion& region = GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(worldMap->GetMapNumberAt(GetParentThing().GetPos())));
            CDefPointer<const CRegionDef> pregion_def = region.GetPRegionDef();
            if (pregion_def.Get()) {
                if (pregion_def.Get()->UseCrimes) {
                    long fine_amount = pvillage->GetFineAmountForCurrentCrimes();
                    if (fine_amount > 0) {
                        long fine_amount_left_in_village = 0;
                        CTCHeroStats* ptc_hero_stats = 0;
                        if (pplayer->GetTC(&ptc_hero_stats)) {
                            long money_available = ptc_hero_stats->GetMoney();
                            if (money_available < fine_amount)
                                fine_amount = money_available;
                            ptc_hero_stats->AddMoney(-fine_amount, true);
                            pvillage->PayFines(fine_amount);
                            ptc_hero_stats->SetFinePaid(ptc_hero_stats->GetFinePaid() + fine_amount);
                            CThing* player = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
                            CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
                            player->GetTC(&ptc_hero_scoreboard);
                            ptc_hero_scoreboard->UpdateScoreForActivity(CTCHeroOnlineScoreboard::OSA_TOTAL_FINES_PAID, (float)ptc_hero_stats->GetFinePaid(), 0);
                            ptc_hero_stats->PrepareToKickHeroToMostRecentSafeRegion();
                            WaitingForRegionKickConfirmation = true;
                        } else
                            pvillage->ClearCrimes();
                        if (ptc_hero_stats) ptc_hero_stats->SetTimesThrownOut(ptc_hero_stats->GetTimesThrownOut() + 1);
                        CWideString message = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_FINE_KICKED_OUT");
                        NGameText::ConstructString(message, fine_amount);
                        if (fine_amount_left_in_village > 0) {
                            CWideString message2 = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_FINE_STILL_TO_PAY");
                            NGameText::ConstructString(message2, fine_amount_left_in_village);
                            message += L" ";
                            message += message2;
                        }
                        pplayer->ModifyHealth(pplayer->GetMaxHealth() * 0.25f, true);
                        CMainGameComponent::Get()->GetDisplayEngine().StartScreenFadeOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 0xff), -1.0f);
                        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), message, 10, 0, 0, true);
                    }
                }
            }
        }
    }
    if (!PendingCrimes.empty() && CWorld::GetFrame() - FramePendingCrimesAdded > CMainGameComponent::GetConstantFPS() * 15)
        ReportAllPendingCrimes(pvillage);
    if (BribePool) {
        if (!NCreatureAI::IsSpeaking(guard)) {
            CTCEmoteIcon* emote_icon;
            guard.GetTC(&emote_icon);
            CCharString text = GFIntToCharString(BribePool);
            emote_icon->SetEmoteText(CTCEmoteIcon::EMOTE_PRIORITY_BRIBE, text.ToWideString(), 1.0f, 0.3f);
            emote_icon->SetReplaceable(true);
        }
    }
}

static unsigned rng;
static unsigned next(unsigned n){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng%n;}
static int want[64],params[64],wn;static unsigned hits[REPLACE+1];
static void expect(Ev e,long n=0){want[wn]=e;params[wn++]=n;}
int main(){
 rng=923763;yesExpr.def.label.apology=true;noExpr.def.label.apology=false;
 for(unsigned run=0;run<65536;++run){
  in.waiting=next(6)==0;in.sub=next(2)!=0;in.dead=next(7)==0;in.hero=next(5)!=0;in.heroDead=next(2)!=0;in.stats=next(2)!=0;in.village=next(4)!=0;in.events=next(2)!=0;in.criminal=next(2)!=0;in.look=next(2)!=0;in.sees=next(2)!=0;in.target=next(2)!=0;in.attacker=next(4)!=0;in.property=next(3)!=0;in.region=next(4)!=0;in.crimes=next(3)!=0;in.pending=next(2)!=0;in.speaks=next(2)!=0;
  in.timeAgo=(long)next(6)-2;in.frame=705;in.prior=in.frame-449-next(3);in.bribe=next(2)?37:0;in.money=next(3)*45;in.fine=(long)next(4)*32-1;in.greeting=next(2)?200:1200;in.health=next(4)?1.0f:1.25f;
  CTCGuard instance;instance.WaitingForRegionKickConfirmation=in.waiting;instance.FramePendingCrimesAdded=in.prior;instance.FrameLastReceivedApology=-1;instance.BribePool=in.bribe;if(in.pending)instance.PendingCrimes.push_back(1);
  guard.dead=in.dead;hero.dead=in.heroDead;def.UseCrimes=in.crimes;village.greeting=in.greeting;paid=21;throwsCount=4;seen=-1;gn=wn=0;events.reset();
  bool waiting=in.waiting;long apology=-1,newPaid=21,newThrows=4,newSeen=-1,newGreeting=in.greeting;int remaining=4;
  if(in.waiting){if(!in.sub){waiting=false;if(in.hero&&!in.heroDead&&in.stats)expect(KICK);}}
  else if(!in.dead){
   if(in.events){for(int i=0;i<2;++i){expect(REMOVE);--remaining;if(in.village){if(in.timeAgo>0)expect(AMNESTY,in.timeAgo);expect(APOLOGY,!in.criminal);++remaining;if(!in.criminal)apology=in.frame;}}}
   if(in.village&&in.criminal){if(in.look&&in.sees&&in.target&&in.hero){expect(SEEN,in.frame);newSeen=in.frame;}newGreeting=std::max(in.greeting,in.frame+300);expect(GREET,newGreeting);}
   if(in.village&&in.hero&&in.health==1&&in.attacker&&in.property&&in.region&&in.crimes&&in.fine>0){
    long charged=in.fine;
    if(in.stats){charged=std::min(charged,in.money);expect(CHARGE,-charged);expect(PAY,charged);newPaid+=charged;expect(SCORE,newPaid);expect(PREP);waiting=true;++newThrows;}
    else expect(CLEAR);
    expect(FORMAT,charged);expect(HEALTH);expect(FADE);expect(SUBTITLE,charged);
   }
   if(in.pending&&in.frame-in.prior>450)expect(REPORT);
   if(in.bribe&&!in.speaks){expect(EMOTE,in.bribe);expect(REPLACE);}
  }
  instance.FrameUpdate();check(wn==gn);for(int i=0;i<gn;++i){check(want[i]==got[i]&&params[i]==args[i]);++hits[got[i]];}
  check(instance.WaitingForRegionKickConfirmation==waiting&&instance.FrameLastReceivedApology==apology&&village.greeting==newGreeting&&seen==newSeen&&paid==newPaid&&throwsCount==newThrows&&events.events.size()==(unsigned)remaining);
 }
 for(int i=0;i<=REPLACE;++i)check(hits[i]!=0);events.reset();puts("007b3120_TEST PASS");return 0;
}