// VC7.1 /Od fixture. State transition code is verbatim, with test doubles at game service boundaries.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#include "engine/C3DVector.h"
#include "engine/GFSqr.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/EMessageEventType.h"
#include "fable_assert.h"
#include <algorithm>
#include <vector>
#include <assert.h>
#include <stdio.h>

namespace NCreatureMode { enum { MODE_SCREAMER_AGGRESSIVE=68 }; }
namespace NStealth { enum EDangerStatus { DANGER_STATUS_CHASING=2 }; }
struct CThing;
struct CTCCreatureModeManager {
    CTCCreatureModeManager() : active(false), checks(0) {}
    bool IsModeActive(unsigned long mode) {
        assert(mode==NCreatureMode::MODE_SCREAMER_AGGRESSIVE);
        ++checks;
        return active;
    }
    bool active;
    unsigned checks;
};
struct CTCTargetingAI {
    CTCTargetingAI() : target(0),checks(0) {}
    CThing* GetPBestEnemyCreatureForAttack() {
        ++checks;
        return target;
    }
    CThing* target;
    unsigned checks;
};
struct CTCStealth {
    CTCStealth() : warnings(0),pursuer(0) {}
    void SetAsInDanger(CThing& from,NStealth::EDangerStatus danger) {
        assert(danger==NStealth::DANGER_STATUS_CHASING);
        ++warnings;
        pursuer=&from;
    }
    unsigned warnings;
    CThing* pursuer;
};
struct CCombatTypeDef { float FrontFarLimit,PreferredCombatDistance; };
struct CThing {
    CThing() : pos(0,0,0),alive(true),have_stealth(true) { type.FrontFarLimit=1;type.PreferredCombatDistance=7; }
    const C3DVector& GetPos() const { return pos; }
    const CCombatTypeDef& GetCombatTypeDef() const { return type; }
    bool GetTC(CTCCreatureModeManager** out) { *out=&modes; return true; }
    bool GetTC(CTCTargetingAI** out) { *out=&targeting; return true; }
    bool GetTC(CTCStealth** out) { *out=&stealth; return have_stealth; }
    C3DVector pos;
    bool alive,have_stealth;
    CCombatTypeDef type;
    CTCCreatureModeManager modes;
    CTCTargetingAI targeting;
    CTCStealth stealth;
};
template<class T> class CIntelligentPointer {
public:
    CIntelligentPointer(T* p=0) : p(p) {}
    template<class U> CIntelligentPointer(CIntelligentPointer<U>& other) : p(other.Get()) {}
    T* Get() const { return p; }
    T* operator->() const { return p; }
    T& operator*() const { assert(p); return *p; }
private:
    T* p;
};
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() && p->alive; }

#include "engine/CThingCompare_Nearest.h"
#include "engine/CAdaptThingCompareToSTLComparitor.h"

struct CMessageEvent { EMessageEventType type; const CThing* recipient; };
struct CMessageEventFilter_IsOfType {
    explicit CMessageEventFilter_IsOfType(EMessageEventType type) : type(type) {}
    bool operator()(const CMessageEvent& event) const { return type==event.type; }
    EMessageEventType type;
};
struct CMessageEventFilter_IsTargettedAtThing {
    explicit CMessageEventFilter_IsTargettedAtThing(const CThing& thing) : thing(thing) {}
    bool operator()(const CMessageEvent& event) const { return &thing==event.recipient; }
    const CThing& thing;
};
#include "engine/CFilter_Bind2.h"

struct CMessageEventManager {
    CMessageEventManager() : event(0),searches(0) {}
    template<class Filter> const CMessageEvent* FindMostRecentMessage(const Filter& filter) {
        ++searches;
        return event && filter(*event) ? event : 0;
    }
    const CMessageEvent* event;
    unsigned searches;
};
struct CWorld {
    CWorld() : seed(1) {}
    CMessageEventManager& GetMessageEventManager() { return messages; }
    unsigned long* GetWorldSeed() { return &seed; }
    static long GetFrame() { return frame; }
    CMessageEventManager messages;
    static long frame;
    unsigned long seed;
};
long CWorld::frame;
unsigned long GFRandom(unsigned long range,unsigned long* seed) {
    assert(range==20);
    ++*seed;
    return 4;
}
struct CPlayer {
    CThing* GetPControlledCreature() { return hero; }
    CThing* hero;
};
struct CPlayerManager {
    CPlayer& GetMainPlayer() { return player; }
    CPlayer player;
};
struct CCreatureAction_ScreamerIntoAggressive {
    CCreatureAction_ScreamerIntoAggressive(CThing& actor,CThing& target) : actor(actor),target(target) {}
    CThing& actor;
    CThing& target;
};

class CAIStateGroup_ScreamerPursueTarget {
public:
    CAIStateGroup_ScreamerPursueTarget(CThing& actor,CThing& target) : actor(actor),target(target),
        TickToGetAggressive(999), polls(0),stop_after(2), moves(0),screams(0),terminations(0),
        move_result(AI_ACTION_IN_PROGRESS),scream_result(AI_ACTION_FINISHED),advance(0),activate_during_move(false) {
        players.player.hero=&target;
        actor.targeting.target=&target;
        CWorld::frame=100;
    }
    void Update();
    CThing& GetCreature() { return actor; }
    CPlayerManager& GetPlayerManager() { return players; }
    CWorld& GetWorld() { return world; }
    bool IsStopping() { return polls++>=stop_after; }
    void Terminate() { ++terminations; }
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureAction_ScreamerIntoAggressive& action,bool wait) {
        assert(!wait && &action.actor==&actor && &action.target==&target);
        ++screams;
        return scream_result;
    }
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing> item,float proximity,
        bool& nav, EMovementType movement,bool a,bool b,bool c,bool d,float speed) {
        assert(repeat==ACTION_REPEAT_CALLBACK && item.Get()==&target && proximity==7.0f);
        assert(movement==AI_MOVEMENT_WALKING && a && b && c && d && speed==0.0f);
        nav=true;
        ++moves;
        CWorld::frame+=advance;
        if (activate_during_move) actor.modes.active=true;
        return move_result;
    }
    CThing& actor;
    CThing& target;
    CPlayerManager players;
    CWorld world;
    CIntelligentPointer<CThing> PTarget;
    unsigned long TickToGetAggressive;
    unsigned polls,stop_after,moves,screams,terminations;
    EAIActionReturn move_result,scream_result;
    long advance;
    bool activate_during_move;
};

void CAIStateGroup_ScreamerPursueTarget::Update()
{
    CTCCreatureModeManager* ptc_cmm;
    if (!GetCreature().GetTC(&ptc_cmm)) {
        FABLE_RETAIL_ASSERT(false);
    }
    CTCTargetingAI* pTCTarget;
    if (!GetCreature().GetTC(&pTCTarget)) {
        FABLE_RETAIL_ASSERT(false);
    }

    std::vector<CThing*> targets;
    targets.push_back(GetPlayerManager().GetMainPlayer().GetPControlledCreature());
    if (targets.empty()) {
        Terminate();
        return;
    }
    std::sort(targets.begin(), targets.end(),
        AdaptThingCompareToSTLComparitor(CThingCompare_Nearest(GetCreature().GetPos())));
    PTarget = CIntelligentPointer<CThing>(targets.front());
    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(PTarget));
    CTCStealth* ptc_stealth;
    if (PTarget->GetTC(&ptc_stealth))
        ptc_stealth->SetAsInDanger(GetCreature(),NStealth::DANGER_STATUS_CHASING);

    TickToGetAggressive = 0;
    bool quit_out = false;
    bool nav;
    EAIActionReturn ret;
    do {
        if (IsStopping() || !IS_THING_ALIVE(PTarget)) {
            quit_out = true;
        } else {
            bool getAggressive = false;
            if (pTCTarget->GetPBestEnemyCreatureForAttack() == PTarget.Get()) {
                C3DVector deltaToTarg = PTarget->GetPos() - GetCreature().GetPos();
                if (deltaToTarg.GetSquaredMagnitude() < GFSqr(GetCreature().GetCombatTypeDef().FrontFarLimit))
                    quit_out = true;
            }
            if (TickToGetAggressive == 0) {
                if (!ptc_cmm->IsModeActive(NCreatureMode::MODE_SCREAMER_AGGRESSIVE)) {
                    const CMessageEvent* pevent = GetWorld().GetMessageEventManager().FindMostRecentMessage(
                        GFFilter_Bind2(CMessageEventFilter_IsOfType(MESSAGE_EVENT_YELL_I_SEE_HIM),
                            CMessageEventFilter_IsTargettedAtThing(*PTarget)));
                    if (pevent) {
                        C3DVector delta = PTarget->GetPos() - GetCreature().GetPos();
                        float dist = delta.GetMagnitude2D();
                        TickToGetAggressive = GetWorld().GetFrame() + 5;
                        TickToGetAggressive += GFRandom(20,GetWorld().GetWorldSeed());
                        TickToGetAggressive += static_cast<long>(dist * 3.0f);
                    }
                }
            } else if (GetWorld().GetFrame() >= TickToGetAggressive) {
                getAggressive = !ptc_cmm->IsModeActive(NCreatureMode::MODE_SCREAMER_AGGRESSIVE);
            }
            if (getAggressive) {
                ret = AIAction_PerformActionAndWait(CCreatureAction_ScreamerIntoAggressive(GetCreature(),*PTarget),false);
                if (ret != AI_ACTION_FINISHED)
                    quit_out = true;
            } else {
                ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTarget,GetCreature().GetCombatTypeDef().PreferredCombatDistance,
                    nav,AI_MOVEMENT_WALKING,true,true,true,true,0.0f);
                if (ret != AI_ACTION_IN_PROGRESS)
                    quit_out = true;
            }
        }
    } while (!quit_out);
    Terminate();
}

int main()
{
    {
        CThing actor,hero;
        CAIStateGroup_ScreamerPursueTarget stopped(actor,hero);
        stopped.stop_after=0;
        stopped.Update();
        assert(stopped.terminations==1 && stopped.moves==0 && stopped.screams==0 && stopped.TickToGetAggressive==0);
        assert(hero.stealth.warnings==1 && hero.stealth.pursuer==&actor);
    }
    {
        CThing actor,hero;
        hero.alive=false;
        hero.have_stealth=false;
        CAIStateGroup_ScreamerPursueTarget dead(actor,hero);
        dead.Update();
        assert(dead.terminations==1 && dead.moves==0 && dead.world.messages.searches==0 && hero.stealth.warnings==0);
    }
    {
        CThing actor,hero;
        hero.pos.X=10;
        CAIStateGroup_ScreamerPursueTarget failure(actor,hero);
        failure.move_result=AI_ACTION_FAILED;
        failure.Update();
        assert(failure.terminations==1 && failure.moves==1 && failure.polls==1 && failure.TickToGetAggressive==0);
    }
    {
        CThing actor,hero;
        hero.pos.X=0.5f;
        CAIStateGroup_ScreamerPursueTarget close(actor,hero);
        close.Update();
        assert(close.terminations==1 && close.moves==1 && close.polls==1 && close.screams==0);
    }
    {
        CThing actor,hero,other;
        hero.pos=C3DVector(3,4,100);
        CAIStateGroup_ScreamerPursueTarget timed(actor,hero);
        CMessageEvent signal={MESSAGE_EVENT_YELL_I_SEE_HIM,&hero};
        timed.world.messages.event=&signal;
        timed.advance=50;
        timed.scream_result=AI_ACTION_FAILED;
        timed.Update();
        assert(timed.terminations==1 && timed.moves==1 && timed.screams==1);
        assert(timed.TickToGetAggressive==124 && timed.world.seed==2 && timed.world.messages.searches==1);
    }
    {
        CThing actor,hero,other;
        hero.pos.X=0.5f;
        CAIStateGroup_ScreamerPursueTarget different_target(actor,hero);
        actor.targeting.target=&other;
        CMessageEvent signal={MESSAGE_EVENT_YELL_I_SEE_HIM,&other};
        different_target.world.messages.event=&signal;
        different_target.Update();
        assert(different_target.terminations==1 && different_target.moves==2 && different_target.screams==0);
        assert(different_target.TickToGetAggressive==0 && different_target.world.messages.searches==2);
    }
    {
        CThing actor,hero;
        hero.pos.X=9;
        actor.modes.active=true;
        CAIStateGroup_ScreamerPursueTarget aggressive(actor,hero);
        CMessageEvent signal={MESSAGE_EVENT_YELL_I_SEE_HIM,&hero};
        aggressive.world.messages.event=&signal;
        aggressive.Update();
        assert(aggressive.terminations==1 && aggressive.moves==2 && aggressive.screams==0);
        assert(aggressive.world.messages.searches==0 && aggressive.TickToGetAggressive==0);
    }
    puts("0096df50_TEST PASS");
    return 0;
}