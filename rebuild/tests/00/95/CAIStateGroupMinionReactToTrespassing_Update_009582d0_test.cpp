#include <stdlib.h>
#include <stdio.h>
#include "engine/fable_ai_action_enums.h"
#include "engine/EOpinionDeedType.h"
static void check(bool b){if(!b) abort();}
class CThing {public: bool dead; CThing():dead(false){}};
class CThingPlayerCreature : public CThing {};
template<class T> class CIntelligentPointer {public:
    CIntelligentPointer(T* p=0):value(p){} T* Get()const{return value;}
private: T* value;
};
template<class T> bool IS_THING_ALIVE(T* p) { return p && !p->dead; }
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& p){return IS_THING_ALIVE(p.Get());}
enum EComponent { TCI_TRESPASSABLE, TCI_CREATURE_OPINION_OF_HERO };
class CTCTrespassable {};
class CThingBuilding:public CThing {public: int lookups; CTCTrespassable* tc;void GetTC(EComponent id,CTCTrespassable** out){check(id==TCI_TRESPASSABLE);++lookups;*out=tc;}};
class COpinionDeedFilter_IsStillHappening {};
class COpinionDeedFilter_TargetIs {public: const CThing& target;COpinionDeedFilter_TargetIs(const CThing& t):target(t){}};
struct TestFilter {const CThing* target;};
static TestFilter GFFilter_Bind2(const COpinionDeedFilter_IsStillHappening&,const COpinionDeedFilter_TargetIs& t){TestFilter v={&t.target};return v;}
class CTCCreatureOpinionOfHero {public:
    mutable int queries; int rank;const CThing* home;
    bool FindMostRecentDeed(EOpinionDeedType type,TestFilter filter,long* index,bool include) const {
        const EOpinionDeedType expected[]={OPINION_DEED_TYPE_CRIME_TRESPASS_FIRST,OPINION_DEED_TYPE_CRIME_TRESPASS_SECOND,OPINION_DEED_TYPE_CRIME_TRESPASS_THIRD};
        int slot=queries++;check(slot<3 && type==expected[slot] && filter.target==home && !index && include);
        return slot+1==rank;
    }
};
class CThingAICreature {public:
    CIntelligentPointer<CThingBuilding> HomeBuilding;CTCCreatureOpinionOfHero* opinion;
    CThingBuilding* GetHomeBuilding()const;
    bool GetTC(EComponent id,const CTCCreatureOpinionOfHero** out){check(id==TCI_CREATURE_OPINION_OF_HERO);*out=opinion;return opinion!=0;}
};
class CPlayerManager {public:
    CThingPlayerCreature* hero; CPlayerManager& GetMainPlayer(){return *this;} CThingPlayerCreature* GetPControlledCreature(){return hero;}
};
class CWorld {public: static long frame; static long GetFrame(){return frame;}};long CWorld::frame;
class CAIStateGroup_MinionReactToTrespassing { public:
    CIntelligentPointer<CThing> PHero;CThingAICreature actor; CPlayerManager players;
    int exits,moves,looks,case_id;long LastFrameCheckedIsStillValid;
    CThingAICreature& GetCreature(){return actor;}
    CPlayerManager& GetPlayerManager(){return players;}
    bool IsStillRunnable(bool);
    void Update(); void Terminate(){++exits;}
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat r,const CIntelligentPointer<CThing>& h,float distance,bool& route,EMovementType m,bool a,bool b,bool c,bool d,float last){
        check(r==ACTION_REPEAT_CALLBACK && h.Get()==players.hero && distance==3.0f && m==AI_MOVEMENT_WALKING && a && !b && c && d && last==0.0f);
        ++moves;check(moves<=3);route=true;
        if(case_id==3)players.hero->dead=true;
        if(case_id==4 && moves==1)return AI_ACTION_IN_PROGRESS;
        return case_id==5?AI_ACTION_TERMINATE_PROCESS:AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat r,const CIntelligentPointer<CThing>& h,long count,float speed){
        ++looks;check(looks<=2 && r==ACTION_REPEAT_CALLBACK && h.Get()==players.hero && count==19 && speed==0.0027777f);
        if(case_id==6 && looks==1)return AI_ACTION_IN_PROGRESS;
        if(case_id==7 && looks==1)return AI_ACTION_FINISHED;
        if(case_id==8)players.hero->dead=true;
        return AI_ACTION_TERMINATE_PROCESS;
    }
};
namespace { long FRAMES_BETWEEN_CHECKING_IS_STILL_VALID = 30; }
class CTCTrespassable;

inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

inline bool CAIStateGroup_MinionReactToTrespassing::IsStillRunnable(bool ignore_frame_check)
{
    if (!ignore_frame_check && CWorld::GetFrame()-LastFrameCheckedIsStillValid < FRAMES_BETWEEN_CHECKING_IS_STILL_VALID)
        return true;
    LastFrameCheckedIsStillValid = CWorld::GetFrame();
    CThingBuilding* home_building = GetCreature().GetHomeBuilding();
    if (IS_THING_ALIVE(home_building)) {
        const CTCCreatureOpinionOfHero* ptc_opinion;
        if (GetCreature().GetTC(TCI_CREATURE_OPINION_OF_HERO, &ptc_opinion)) {
            if (ptc_opinion->FindMostRecentDeed(OPINION_DEED_TYPE_CRIME_TRESPASS_FIRST, GFFilter_Bind2(COpinionDeedFilter_IsStillHappening(), COpinionDeedFilter_TargetIs(*home_building)), 0, true)
                || ptc_opinion->FindMostRecentDeed(OPINION_DEED_TYPE_CRIME_TRESPASS_SECOND, GFFilter_Bind2(COpinionDeedFilter_IsStillHappening(), COpinionDeedFilter_TargetIs(*home_building)), 0, true)
                || ptc_opinion->FindMostRecentDeed(OPINION_DEED_TYPE_CRIME_TRESPASS_THIRD, GFFilter_Bind2(COpinionDeedFilter_IsStillHappening(), COpinionDeedFilter_TargetIs(*home_building)), 0, true))
                return true;
        }
    }
    return false;
}

void CAIStateGroup_MinionReactToTrespassing::Update()
{
    CTCTrespassable* trespassable;
    if (IS_THING_ALIVE(GetCreature().GetHomeBuilding()))
        GetCreature().GetHomeBuilding()->GetTC(TCI_TRESPASSABLE, &trespassable);
    PHero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    bool moving = true;
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(PHero)) break;
        if (moving) {
            do {
                if (!IS_THING_ALIVE(PHero)) { Terminate(); return; }
                bool finished_calculating_route;
                ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PHero,3.0f,finished_calculating_route,AI_MOVEMENT_WALKING,true,false,true,true,0.0f);
                if (!IsStillRunnable(false) || !IS_THING_ALIVE(PHero)) ret = AI_ACTION_TERMINATE_PROCESS;
            } while (ret == AI_ACTION_IN_PROGRESS);
            moving = false;
        } else {
            do {
                if (!IS_THING_ALIVE(PHero)) { Terminate(); return; }
                ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,PHero,19,0.0027777f);
                if (!IsStillRunnable(false) || !IS_THING_ALIVE(PHero)) ret = AI_ACTION_TERMINATE_PROCESS;
            } while (ret == AI_ACTION_IN_PROGRESS);
            moving = true;
        }
    } while (ret != AI_ACTION_TERMINATE_PROCESS);
    Terminate();
}

int main() {
    for(int i=0;i<14;++i) {
        CThingPlayerCreature hero;CThingBuilding home;CTCCreatureOpinionOfHero opinion;CTCTrespassable component;
        CAIStateGroup_MinionReactToTrespassing state;state.exits=state.moves=state.looks=0;state.case_id=i;
        state.players.hero=i==0?0:&hero;hero.dead=i==1;home.dead=i==10;home.lookups=0;home.tc=&component;
        state.actor.HomeBuilding=i==9?0:&home;opinion.rank=i==12?0:2;opinion.queries=0;opinion.home=&home;
        state.actor.opinion=i==11?0:&opinion;CWorld::frame=100;state.LastFrameCheckedIsStillValid=i==2?95:70;
        state.Update();
        check(state.exits==1 && state.PHero.Get()==state.players.hero);
        check(home.lookups==((i==9||i==10)?0:1));
        check(state.moves==(i<2?0:(i==4||i==7)?2:1));
        check(state.looks==((i==6||i==7)?2:(i==2||i==4||i==8||i==13)?1:0));
        check(opinion.queries==(i<3||i==9||i==10||i==11?0:i==12?3:2));
    }
    for(int rank=0;rank<=3;++rank) {
        CThingBuilding home;CTCCreatureOpinionOfHero opinion; CAIStateGroup_MinionReactToTrespassing state;
        state.actor.HomeBuilding=&home;state.actor.opinion=&opinion;opinion.home=&home;opinion.rank=rank;
        state.LastFrameCheckedIsStillValid=100;CWorld::frame=101;opinion.queries=0;
        check(state.IsStillRunnable(false)&&opinion.queries==0);
        check(state.IsStillRunnable(true)==(rank>0));check(opinion.queries==(rank?rank:3));
    }
    puts("009582d0_TEST PASS"); return 0;
}