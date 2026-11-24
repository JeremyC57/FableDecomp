#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
// Original ret/nav_route_calculated and scoped ptc_flammable locals; true shared owner/action types.
// Leaf: ?Update@CAIStateGroup_MinionWatchFires@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_MINIONWATCHFIRES_UPDATE_PASS26
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCFlammable.h"
#include "engine/CAIStateGroup_MinionWatchFires.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_AlertToPlayer.h"
#include "engine/ThingAlive.h"


#include <cstdio>
#include <cmath>
static int failures=0;
#define CHECK(x) do{if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
struct FireState {bool alive,present,burning;int moves,alerts,idles,terminates,queries,constructed,destroyed;int move_results[4],alert_results[4],idle_results[4];int disappear_after_move,extinguish_after_idle;FireState():alive(true),present(true),burning(true),moves(0),alerts(0),idles(0),terminates(0),queries(0),constructed(0),destroyed(0),disappear_after_move(-1),extinguish_after_idle(-1){for(int i=0;i<4;++i){move_results[i]=2;alert_results[i]=2;idle_results[i]=2;}}};
struct FixtureFlammable{FireState* s;bool IsOnFire()const{return s->burning;}};
struct FixtureBuilding{FireState* s;FixtureFlammable f;C3DVector pos;FixtureBuilding(FireState& state):s(&state),pos(1,2,3){f.s=s;}const C3DVector& GetPos()const{return pos;}bool GetTC(FixtureFlammable**p){++s->queries;if(s->present)*p=&f;return s->present;}bool PeekTC(const FixtureFlammable**p)const{++s->queries;if(s->present)*p=&f;return s->present;}};
struct FixturePointer{FixtureBuilding*p;FixtureBuilding*operator->()const{return p;}};
static bool FABLE_FASTCALL IS_THING_ALIVE(const FixturePointer&p){return p.p&&p.p->s->alive;}
struct FixtureAction{FireState*s;FixtureAction(FireState& state):s(&state){++s->constructed;}~FixtureAction(){++s->destroyed;}};
struct FixtureGroup{FireState&s;FixturePointer PBuildingOnFire;FixtureGroup(FireState& state,FixtureBuilding*p):s(state){PBuildingOnFire.p=p;}void Update();void Terminate(){++s.terminates;}FireState&GetCreature(){return s;}
 EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat r,C3DVector p,float radius,bool& route,EMovementType m,bool a,bool b,bool c,const CThing*t,bool e,float f){CHECK((int)r==2&&radius==5&&(int)m==5&&a&&!b&&c&&!t&&e&&f==0);CHECK(p.X==1&&p.Y==2&&p.Z==3);CHECK(s.moves<4);route=true;int result=s.move_results[s.moves++];if(s.moves==s.disappear_after_move)s.alive=false;return(EAIActionReturn)result;}
 EAIActionReturn AIAction_PerformActionAndWait(const FixtureAction&,bool wait){CHECK(!wait&&s.alerts<4);return(EAIActionReturn)s.alert_results[s.alerts++];}
 EAIActionReturn AIAction_DoNothing(){CHECK(s.idles<4);int result=s.idle_results[s.idles++];if(s.idles==s.extinguish_after_idle)s.burning=false;return(EAIActionReturn)result;}
};
#define CAIStateGroup_MinionWatchFires FixtureGroup
#define CTCFlammable FixtureFlammable
#define CCreatureAction_AlertToPlayer FixtureAction
void CAIStateGroup_MinionWatchFires::Update()
{
    bool nav_route_calculated = false;
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(PBuildingOnFire))
            { Terminate(); return; }
        CTCFlammable* ptc_flammable;
        if (!PBuildingOnFire->GetTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
            { Terminate(); return; }
        ret = AIAction_MoveToPosProximity((EAIActionRepeat)2, PBuildingOnFire->GetPos(), 5.0f, nav_route_calculated,
                                             (EMovementType)5, true, false, true, 0, true, 0.0f);
    } while (ret == 4);
    if(ret!=2){Terminate();return;}
    {
        for (;;) {
            if (!IS_THING_ALIVE(PBuildingOnFire))
                {Terminate();return;}
            const CTCFlammable* ptc_flammable;
            if (!PBuildingOnFire->PeekTC(&ptc_flammable) || !ptc_flammable->IsOnFire())
                {Terminate();return;}
            if (AIAction_PerformActionAndWait(CCreatureAction_AlertToPlayer(GetCreature()), false) != 2)
                {Terminate();return;}
            if (AIAction_DoNothing() != 4)
                return;
        }
    }

}

#undef CAIStateGroup_MinionWatchFires
#undef CTCFlammable
#undef CCreatureAction_AlertToPlayer
int main(){for(int mode=0;mode<11;++mode){FireState s;FixtureBuilding building(s);FixtureGroup g(s,mode==0?0:&building);
 if(mode==1)s.alive=false;if(mode==2)s.present=false;if(mode==3)s.burning=false;
 if(mode==4)s.move_results[0]=0;
 if(mode==5){s.move_results[0]=4;s.move_results[1]=2;}
 if(mode==6)s.disappear_after_move=1;
 if(mode==7)s.alert_results[0]=0;
 if(mode==8){s.idle_results[0]=4;s.extinguish_after_idle=1;}
 if(mode==9){s.idle_results[0]=4;s.idle_results[1]=2;}
 if(mode==10){s.move_results[0]=4;s.disappear_after_move=1;}
 g.Update();CHECK(s.terminates==((mode<=4||mode==6||mode==7||mode==8||mode==10)?1:0));CHECK(s.moves==(mode<=3?0:(mode==5?2:1)));CHECK(s.alerts==((mode<=4||mode==6||mode==10)?0:(mode==9?2:1)));CHECK(s.idles==((mode<=4||mode==6||mode==7||mode==10)?0:(mode==9?2:1)));CHECK(s.constructed==s.alerts&&s.destroyed==s.alerts);}
 std::puts(failures?"009363d0_TEST FAIL":"009363d0_TEST PASS");return failures?1:0;}