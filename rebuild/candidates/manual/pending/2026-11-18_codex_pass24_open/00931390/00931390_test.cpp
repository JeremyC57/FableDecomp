#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <list>
#include <vector>

enum ETCInterfaceType { TCI_THING_OWNER, TCI_DOOR, TCI_TALK, TCI_PHYSICS, TCI_NAVIGATION, TCI_VOLUME_CONTAINMENT_TRACKER };
enum EAIActionReturn { AI_ACTION_NULL, AI_ACTION_TERMINATE_PROCESS, AI_ACTION_FINISHED, AI_ACTION_FAILED, AI_ACTION_IN_PROGRESS };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED=1, ACTION_REPEAT_CALLBACK=2 };
enum EMovementType { AI_MOVEMENT_RUNNING=5 };

class C3DVector {
public:
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0) : X(x),Y(y),Z(z) { }
    C3DVector operator-(const C3DVector& b) const { return C3DVector(X-b.X,Y-b.Y,Z-b.Z); }
    float GetMagnitude() const { return std::sqrt(X*X+Y*Y+Z*Z); }
};

template<class T> class CIntelligentPointer {
    T* data;
public:
    CIntelligentPointer(T* p=0) : data(p) { }
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& p) : data(p.Get()) { }
    T* Get() const { return data; }
    T* operator->() const { return data; }
};
class CThing;
class CTCThingOwner {
public:
    std::list<CIntelligentPointer<CThing> > contents;
    const std::list<CIntelligentPointer<CThing> >& GetObjectList() const { return contents; }
};

class CTCPhysicsBase {
public:
    float Radius;
    CTCPhysicsBase() : Radius(6) { }
};

class CTCVolumeContainmentTracker {
public:
    CThing* inside;
    CTCVolumeContainmentTracker() : inside(0) { }
    CThing* GetPThingInside() const { return inside; }
};

class CTCCreatureNavigation {
public:
    bool wander_available, directed_available, navigable;
    mutable unsigned wander_queries, directed_queries;
    CTCCreatureNavigation()
        : wander_available(false), directed_available(false), navigable(true), wander_queries(0), directed_queries(0) { }
    bool GetNearbyWanderPosition(const C3DVector& start,float radius,C3DVector& position,bool obstacles) const
    {
        assert(start.X==5 && radius==3 && !obstacles);
        ++wander_queries;
        position=C3DVector(5);
        return wander_available;
    }
    bool GetNearbyDirectedPosition(const C3DVector& start,float radius,const C3DVector&,float angle,C3DVector& out) const
    {
        assert(radius==5 && angle==60);
        ++directed_queries;
        out=C3DVector(start.X-4);
        return directed_available;
    }
    bool IsPosNavigable(const C3DVector&) const { return navigable; }
};

namespace NSpeechGainManager {
enum ELayer { DIALOGUE_LAYER_1, DIALOGUE_LAYER_2 };
}

class CTCTalk {
public:
    enum ESpeechPlaybackResult { SPEECH_PLAYBACK_OK, SPEECH_PLAYBACK_FAILED_RETRY };
    bool talking;
    unsigned retries, plays;
    const char* last_text;
    CTCTalk() : talking(false), retries(0), plays(0), last_text(0) { }
    bool IsTalking() const { return talking; }
    ESpeechPlaybackResult PlayAISpeech(const char* text, bool, NSpeechGainManager::ELayer, bool queued, bool audible)
    {
        assert(!queued && audible);
        last_text=text;
        ++plays;
        return plays<=retries ? SPEECH_PLAYBACK_FAILED_RETRY : SPEECH_PLAYBACK_OK;
    }
};

class CThing {
public:
    C3DVector position;
    long def_index;
    bool alive, door, in_use, aware, has_talk, has_tracker;
    CTCVolumeContainmentTracker tracker;
    CTCCreatureNavigation nav;
    CTCTalk talk;
    CThing()
        : def_index(0), alive(true), door(false), in_use(false), aware(false),
          has_talk(false), has_tracker(false) { }
    long GetDefGlobalIndex() const { return def_index; }
    C3DVector GetPos() const { return position; }
    bool HasTC(ETCInterfaceType id) const { assert(id==TCI_DOOR); return door; }
    bool PeekTC(ETCInterfaceType id,const CTCCreatureNavigation** ptr) const
    { assert(id==TCI_NAVIGATION); *ptr=&nav; return true; }
    bool GetTC(ETCInterfaceType id, CTCTalk** ptr)
    { assert(id==TCI_TALK); *ptr=&talk; return has_talk; }
    bool GetTC(ETCInterfaceType id, CTCVolumeContainmentTracker** ptr)
    { assert(id==TCI_VOLUME_CONTAINMENT_TRACKER); *ptr=&tracker; return has_tracker; }
};

class CThingBuilding : public CThing {
public:
    CTCThingOwner owner;
    CTCPhysicsBase physics;
    bool locked, lockable, override_locking;
    unsigned lock_changes;
    CThingBuilding() : locked(false), lockable(true), override_locking(false), lock_changes(0) { position.X=5; }
    bool IsLocked() const { return locked; }
    bool IsLockable() const { return lockable; }
    bool IsPointInside(const C3DVector& p) const { return p.X>=4 && p.X<=6; }
    C3DVector GetPhysPos() const { return position; }
    void SetOverrideAutomaticLocking(bool active) { override_locking=active; }
    void LockAllDoors(bool lock,bool propagate) { assert(propagate); locked=lock; ++lock_changes; }
    bool PeekTC(ETCInterfaceType id,const CTCThingOwner** ptr) const
    { assert(id==TCI_THING_OWNER); *ptr=&owner; return true; }
    bool PeekTC(ETCInterfaceType id,const CTCPhysicsBase** ptr) const
    { assert(id==TCI_PHYSICS); *ptr=&physics; return true; }
    bool GetTC(ETCInterfaceType id,CTCThingOwner** ptr)
    { assert(id==TCI_THING_OWNER); *ptr=&owner; return true; }
};

template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() && p->alive; }
bool OrderByHeight2(const CThing* a,const CThing* b) { return a->position.Z<b->position.Z; }

class CThingFilter_IsNotBeingExclusivelyUsed {
public:
    bool operator()(const CThing& thing) const { return !thing.in_use; }
};

class CGameDefinitionManager {
public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager result; return result; }
    long GetDefGlobalIndexFromName(const char* name) const
    { assert(std::strcmp(name,"GAZE_OUT_OF_BUILDING_MARKER")==0); return 12; }
};

namespace NCreatureAI {
bool IsAwareOfThingIndirectly(const CThing* creature,const CThing* hero,bool)
{ return hero && creature->aware; }
}

class CWorld {
public:
    static long frame;
    static long GetFrame() { return frame; }
};
long CWorld::frame;

class CMainGameComponent {
public:
    static long GetConstantFPS() { return 30; }
};
float GFFloatRandom(float, unsigned long*) { return 0; }
unsigned long GFRandom(unsigned long cap,unsigned long* seed) { return cap ? *seed % cap : 0; }

class CSoundPair { };
class CCreatureAction_PlayAnimation {
public:
    CCreatureAction_PlayAnimation(CThing&,const char* anim,const CSoundPair&,bool,bool,bool,long,long)
    { assert(std::strcmp(anim,"ST_IDLE_SUBTLE")==0); }
};

class CAIStateGroup_MinionReactToCriminal {
public:
    CThing creature;
    CIntelligentPointer<CThingBuilding> PHome;
    CIntelligentPointer<CThing> PHero;
    bool MeInsideHouse, HeroInsideHouse, GuardInsideHouse, OverrodeLocking, valid;
    long FrameToNextSpeak;
    unsigned long seed;
    unsigned exits, doors, enter, marks, looks, flee, idle, animations;
    const CThing* selected_door;
    bool enter_stops, enter_retries, marker_stops, idle_retries;
    CAIStateGroup_MinionReactToCriminal()
      : MeInsideHouse(false), HeroInsideHouse(false), GuardInsideHouse(false), OverrodeLocking(false), valid(true),
        FrameToNextSpeak(100), seed(0), exits(0), doors(0), enter(0), marks(0), looks(0), flee(0), idle(0),
        animations(0), selected_door(0), enter_stops(false), enter_retries(false), marker_stops(false), idle_retries(false) { }
    bool IsStillValid() const { return valid; }
    void Terminate() { ++exits; }
    CThing& GetCreature() { return creature; }
    unsigned long* GetPWorldSeed() { return &seed; }
    EAIActionReturn AIAction_DoNothing()
    { ++idle; return idle_retries ? AI_ACTION_IN_PROGRESS : AI_ACTION_TERMINATE_PROCESS; }
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> door, float near,
        bool& route, EMovementType move, bool a, bool b, bool c, bool d, float speed)
    {
        assert(repeat==ACTION_REPEAT_CALLBACK && near==1.2f && move==AI_MOVEMENT_RUNNING && a && !b && c && d && speed==0);
        route=true;
        ++doors;
        selected_door=door.Get();
        return AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat,C3DVector p,float near,bool& route,
        EMovementType move,bool a,bool b,bool c,const CThing* d,bool e,float speed)
    {
        assert(repeat==ACTION_REPEAT_CALLBACK && p.X==5 && near==1.f && move==AI_MOVEMENT_RUNNING && a && b && c && !d && e && speed==0);
        route=true;
        ++enter;
        if (enter_retries && enter==1) return AI_ACTION_IN_PROGRESS;
        return enter_stops ? AI_ACTION_TERMINATE_PROCESS : AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat,C3DVector pos,bool& route,EMovementType move,bool a,bool b,
        const CThing* d,bool e,float speed)
    {
        assert(move==AI_MOVEMENT_RUNNING && a && b && !d && e && speed==0);
        route=true;
        if (repeat==ACTION_REPEAT_CALLBACK) {
            assert(pos.X==5);
            ++marks;
            return marker_stops ? AI_ACTION_TERMINATE_PROCESS : AI_ACTION_FINISHED;
        }
        assert(repeat==ACTION_REPEAT_UNTIL_FINISHED && pos.X==creature.position.X-4);
        ++flee;
        return AI_ACTION_TERMINATE_PROCESS;
    }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<const CThing> other,long frames,float speed)
    {
        assert(repeat==ACTION_REPEAT_CALLBACK && other.Get()==PHero.Get() && frames==39 && speed==0.0027777f);
        ++looks;
        CWorld::frame+=100;
        HeroInsideHouse=true;
        return AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_PerformActionTargetFace(const CCreatureAction_PlayAnimation&, CIntelligentPointer<CThing> target)
    {
        assert(target.Get()==PHero.Get());
        ++animations;
        return AI_ACTION_TERMINATE_PROCESS;
    }
    void Update();
};

// The selected implementation is reproduced verbatim for this standalone fixture.
void CAIStateGroup_MinionReactToCriminal::Update()
{
    if (!IsStillValid()) {
        Terminate();
        return;
    }
    CIntelligentPointer<CThingBuilding>& home = PHome;
    EAIActionReturn ret = AI_ACTION_FAILED;
    do {
        if (!MeInsideHouse && IS_THING_ALIVE(home) && PHome->IsLocked()) {
            std::vector<const CThing*> doors;
            const CTCThingOwner* owner;
            if (PHome->PeekTC(TCI_THING_OWNER, &owner)) {
                std::list<CIntelligentPointer<CThing> >::const_iterator it = owner->GetObjectList().begin();
                while (it != owner->GetObjectList().end()) {
                    if (IS_THING_ALIVE(*it) && (*it)->HasTC(TCI_DOOR))
                        doors.push_back((*it).Get());
                    ++it;
                }
            }
            if (doors.size() != 0) {
                std::sort(doors.begin(), doors.end(), OrderByHeight2);
                bool nav_route_calculated;
                CIntelligentPointer<const CThing> door(doors[0]);
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            CTCTalk::ESpeechPlaybackResult esp_ret;
                            do
                                esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                            while (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                            if (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                                Terminate();
                                return;
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    if (!IS_THING_ALIVE(door)) {
                        Terminate();
                        break;
                    }
                    ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, door.Get(), 1.2f, nav_route_calculated, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
                } while (ret == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (ret == AI_ACTION_TERMINATE_PROCESS) {
                    Terminate();
                    return;
                }
                if (ret == AI_ACTION_FINISHED) {
                    OverrodeLocking = true;
                    PHome->SetOverrideAutomaticLocking(true);
                    PHome->LockAllDoors(false, true);
                }
            }
        }
        if (!MeInsideHouse && IS_THING_ALIVE(home) && !PHome->IsLocked()) {
            C3DVector home_pos;
            const CTCCreatureNavigation* ptc_nav;
            const CTCPhysicsBase* ptc_physics;
            PHome->PeekTC(TCI_PHYSICS, &ptc_physics);
            GetCreature().PeekTC(TCI_NAVIGATION, &ptc_nav);
            if (ptc_nav->GetNearbyWanderPosition(PHome->GetPhysPos(), ptc_physics->Radius * 0.5f, home_pos, false) &&
                PHome->IsPointInside(home_pos) && ptc_nav->IsPosNavigable(home_pos)) {
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            CTCTalk::ESpeechPlaybackResult esp_ret;
                            do
                                esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                            while (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                            if (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                                Terminate();
                                return;
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, home_pos, 1.0f, ret_bool, AI_MOVEMENT_RUNNING, true, true, true, 0, true, 0.0f);
                } while (ret == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (ret == AI_ACTION_TERMINATE_PROCESS) {
                    Terminate();
                    return;
                }
            }
        }
        if (MeInsideHouse && !HeroInsideHouse && !GuardInsideHouse && IS_THING_ALIVE(home) && PHome->IsLockable() &&
            !PHome->IsLocked()) {
            CTCVolumeContainmentTracker* ptc_volume_containment_tracker;
            if (PHero->GetTC(TCI_VOLUME_CONTAINMENT_TRACKER, &ptc_volume_containment_tracker)) {
                CThing* inside = ptc_volume_containment_tracker->GetPThingInside();
                if (inside != PHome.Get() && !PHome->IsLocked()) {
                    PHome->SetOverrideAutomaticLocking(true);
                    OverrodeLocking = true;
                    PHome->LockAllDoors(true, true);
                }
            }
            CIntelligentPointer<CThing> pmarker(0);
            CTCThingOwner* owner;
            if (PHome->GetTC(TCI_THING_OWNER, &owner)) {
                std::vector<CThing*> available_markers;
                long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");
                CThingFilter_IsNotBeingExclusivelyUsed not_used;
                std::list<CIntelligentPointer<CThing> >::const_iterator it = owner->GetObjectList().begin();
                if (it != owner->GetObjectList().end())
                for (; it != owner->GetObjectList().end(); ++it) {
                    if (IS_THING_ALIVE(*it) && (*it)->GetDefGlobalIndex() == def_index &&
                        PHome->IsPointInside((*it)->GetPos()) && not_used(*(*it).Get())) {
                        CThing* thing = (*it).Get();
                        available_markers.push_back(thing);
                    }
                }
                if (!available_markers.empty()) {
                    CIntelligentPointer<CThing> chosen(available_markers[GFRandom(available_markers.size(), GetPWorldSeed())]);
                    pmarker = chosen;
                }
            }
            if (IS_THING_ALIVE(pmarker) && !HeroInsideHouse) {
                bool ret_bool;
                do
                    ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pmarker->GetPos(), ret_bool, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
                while (ret == AI_ACTION_IN_PROGRESS && !HeroInsideHouse);
                if (ret != AI_ACTION_FINISHED && !HeroInsideHouse) {
                    Terminate();
                    return;
                }
            }
            if (!HeroInsideHouse) {
                long FrameToStopLooking = (long)(CWorld::GetFrame() + (GFFloatRandom(3.0f, GetPWorldSeed()) + 2.0f) * CMainGameComponent::GetConstantFPS());
                do {
                    ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PHero, 0x27, 0.0027777f);
                    if (ret != AI_ACTION_IN_PROGRESS && ret != AI_ACTION_FINISHED)
                        break;
                } while (CWorld::GetFrame() < FrameToStopLooking && !HeroInsideHouse);
                if (ret != AI_ACTION_FINISHED && CWorld::GetFrame() < FrameToStopLooking && !HeroInsideHouse) {
                    Terminate();
                    return;
                }
            }
        } else if (IS_THING_ALIVE(PHero) && NCreatureAI::IsAwareOfThingIndirectly(&GetCreature(), PHero.Get(), false)) {
            const CTCCreatureNavigation* ptc_nav;
            GetCreature().PeekTC(TCI_NAVIGATION, &ptc_nav);
            C3DVector flee_pos;
            if (ptc_nav->GetNearbyDirectedPosition(GetCreature().GetPos(), 5.0f, GetCreature().GetPos() - PHero->GetPos(), 60.0f, flee_pos) &&
                (GetCreature().GetPos() - flee_pos).GetMagnitude() > 0.5f &&
                (!MeInsideHouse || HeroInsideHouse || !IS_THING_ALIVE(home) || PHome->IsPointInside(flee_pos))) {
                bool ret_bool;
                if (CWorld::GetFrame() > FrameToNextSpeak) {
                    CTCTalk* ptc_talk;
                    if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                        CTCTalk::ESpeechPlaybackResult esp_ret;
                        do
                            esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                        while (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS);
                        if (esp_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                            Terminate();
                            return;
                        }
                    }
                    FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                }
                ret = AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, flee_pos, ret_bool, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
            } else {
                do {
                    bool ret_bool;
                    if (CWorld::GetFrame() > FrameToNextSpeak) {
                        CTCTalk* ptc_talk;
                        if (GetCreature().GetTC(TCI_TALK, &ptc_talk) && !ptc_talk->IsTalking()) {
                            for (;;) {
                                CTCTalk::ESpeechPlaybackResult esp_ret;
                                if (GFRandom(5, GetPWorldSeed()) > 0)
                                    esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_COWER_TO_SELF", true, NSpeechGainManager::DIALOGUE_LAYER_2, false, true);
                                else
                                    esp_ret = ptc_talk->PlayAISpeech("TEXT_AI_OPINION_ACT_SERVILE_GROVELLING_LARGE_TO_HERO", false, NSpeechGainManager::DIALOGUE_LAYER_1, false, true);
                                if (esp_ret != CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
                                    break;
                                if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                                    Terminate();
                                    return;
                                }
                            }
                        }
                        FrameToNextSpeak = (long)(CWorld::GetFrame() + (GFFloatRandom(10.0f, GetPWorldSeed()) + 5.0f) * CMainGameComponent::GetConstantFPS());
                    }
                    ret = AIAction_PerformActionTargetFace(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE_SUBTLE",
                        CSoundPair(), false, true, false, 0, 0), PHero);
                } while (NCreatureAI::IsAwareOfThingIndirectly(&GetCreature(), PHero.Get(), false) &&
                         (ret == AI_ACTION_IN_PROGRESS || ret == AI_ACTION_FINISHED));
            }
        } else
            ret = AIAction_DoNothing();
    } while (ret == AI_ACTION_IN_PROGRESS || ret == AI_ACTION_FINISHED);
    Terminate();
}

int main()
{
    CWorld::frame=10;
    CAIStateGroup_MinionReactToCriminal invalid;
    invalid.valid=false;
    invalid.Update();
    assert(invalid.exits==1 && invalid.idle==0);

    CThingBuilding home;
    home.locked=true;
    CThing lower, upper, dead;
    lower.door=upper.door=dead.door=true;
    lower.position.Z=3;
    upper.position.Z=8;
    dead.alive=false;
    home.owner.contents.push_back(&upper);
    home.owner.contents.push_back(&dead);
    home.owner.contents.push_back(&lower);
    CAIStateGroup_MinionReactToCriminal escape;
    escape.PHome=&home;
    escape.creature.nav.wander_available=true;
    escape.enter_retries=escape.enter_stops=true;
    escape.Update();
    assert(escape.doors==1 && escape.selected_door==&lower);
    assert(escape.exits==1 && escape.enter==2 && escape.OverrodeLocking && home.override_locking && !home.locked);

    CAIStateGroup_MinionReactToCriminal retry;
    home.locked=true;
    retry.PHome=&home;
    retry.FrameToNextSpeak=0;
    retry.creature.has_talk=true;
    retry.creature.talk.retries=1;
    retry.Update();
    assert(retry.idle==1 && retry.exits==1 && retry.doors==0 && home.locked);
    assert(retry.creature.talk.plays==1);

    CAIStateGroup_MinionReactToCriminal hiding;
    home.locked=false;
    CThing hero, marker;
    hero.has_tracker=true;
    marker.def_index=12;
    marker.position.X=5;
    home.owner.contents.push_back(&marker);
    hiding.PHome=&home;
    hiding.PHero=&hero;
    hiding.MeInsideHouse=true;
    hiding.Update();
    assert(hiding.OverrodeLocking && home.locked && hiding.marks==1 && hiding.looks==1 && hiding.exits==1);

    CAIStateGroup_MinionReactToCriminal fleeing;
    fleeing.PHero=&hero;
    fleeing.creature.aware=true;
    fleeing.creature.nav.directed_available=true;
    fleeing.creature.has_talk=true;
    fleeing.FrameToNextSpeak=0;
    fleeing.Update();
    assert(fleeing.flee==1 && fleeing.animations==0 && fleeing.exits==1 && fleeing.creature.talk.plays==1);

    CAIStateGroup_MinionReactToCriminal cowering;
    cowering.PHero=&hero;
    cowering.creature.aware=true;
    cowering.creature.has_talk=true;
    cowering.FrameToNextSpeak=0;
    cowering.Update();
    assert(cowering.animations==1 && cowering.flee==0 && cowering.exits==1);
    assert(std::strcmp(cowering.creature.talk.last_text,"TEXT_AI_OPINION_ACT_SERVILE_GROVELLING_LARGE_TO_HERO")==0);

    CAIStateGroup_MinionReactToCriminal idle;
    idle.Update();
    assert(idle.idle==1 && idle.exits==1);
    std::puts("00931390_TEST PASS");
}
