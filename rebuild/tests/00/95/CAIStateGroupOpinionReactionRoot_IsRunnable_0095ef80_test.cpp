#include <list>
#include <stdio.h>
#include <string.h>
#include "engine/NCreatureMode.h"
#define FABLE_RETAIL_OPINION_STRENGTH_PASS26
#include "engine/EOpinionTargetingConditionType.h"
#include "engine/ETCInterfaceType.h"

enum EOpinionReactionType { NONE = 0, SAMPLE_REACTION = 30 };
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector operator-(const C3DVector& rhs) const {
        C3DVector result;
        result.X = X - rhs.X;
        result.Y = Y - rhs.Y;
        result.Z = Z - rhs.Z;
        return result;
    }
    float GetSquaredMagnitude() const { return X*X + Y*Y + Z*Z; }
};
struct CBaseObject {};
struct CThing;
template<class T> class CIntelligentPointer {
    T* pointer;
public:
    CIntelligentPointer(T* p = 0) : pointer(p) {}
    T* Get() const { return pointer; }
    T* operator->() const { return pointer; }
    void Set(T* p) { pointer = p; }
};
template<class T> bool operator==(const CBaseObject* p, const CIntelligentPointer<T>& other)
{
    return p == other.Get();
}
template<class T> bool IS_THING_ALIVE(const T* pointer) { return pointer && !pointer->dead; }
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& pointer) { return IS_THING_ALIVE(pointer.Get()); }
struct CTCPerceiveThing {
    const CThing* seen;
    float AwarenessTimer;
    bool ignores;
    const CThing* GetPerceivedThing() const { return seen; }
    bool IsAwareOfThing() const;
    bool IsThingIgnoreFOVLineOfSight() const { return ignores; }
};
struct CTCFollowed {
    const CThing* follower;
    bool IsFollower(const CThing* thing) const { return thing == follower; }
};
struct CTCRegionFollower {
    const CThing* follower;
    bool IsRegionFollower(const CThing* thing) const { return thing == follower; }
};
struct CTCTargetingBase {
    CThing* target;
    CThing* GetPTargetedThing() const { return target; }
};
struct CTCCreatureModeManager {
    unsigned long mode;
    const CTCCreatureModeManager& GetCurrentMode() const { return *this; }
    unsigned long GetType() const { return mode; }
};
struct CPlayer {
    CThing* controlled;
    bool z;
    CThing* GetPControlledCreature() const { return controlled; }
    bool IsZTargeting() const { return z; }
};
struct CThing : CBaseObject {
    C3DVector pos;
    bool dead, itemSource, heroDeeds, candidate;
    int type;
    unsigned long property;
    EOpinionReactionType available;
    const CTCPerceiveThing* perception;
    const CTCFollowed* followed;
    const CTCRegionFollower* region;
    const CTCTargetingBase* targeting;
    const CTCCreatureModeManager* modes;
    CThing() : dead(false), itemSource(false), heroDeeds(false), candidate(false), type(1),
        property(0), available(NONE), perception(0), followed(0), region(0), targeting(0), modes(0) {}
    const C3DVector& GetPos() const { return pos; }
    long GetType() const { return type; }
    unsigned long GetProperty() const { return property; }
    bool HasTC(ETCInterfaceType tc) const { return tc == TCI_OPINION_SOURCE ? itemSource : heroDeeds; }
    bool PeekTC(const CTCPerceiveThing** out) const { *out = perception; return perception != 0; }
    bool PeekTC(const CTCFollowed** out) const { *out = followed; return followed != 0; }
    bool PeekTC(const CTCRegionFollower** out) const { *out = region; return region != 0; }
    bool PeekTC(const CTCTargetingBase** out) const { *out = targeting; return targeting != 0; }
    bool PeekTC(const CTCCreatureModeManager** out) const { *out = modes; return modes != 0; }
};
struct COpinionReactionManager {
    const CThing* forced;
    std::list<CIntelligentPointer<const CThing> > sources;
    int targetCondition[79];
    bool allowWhileCarrying[79];
    EOpinionReactionType next;
    mutable int reactionReads;
    mutable int predicateCalls;
    mutable const CThing* candidateSeen;
    mutable bool observedFlags[79];
    COpinionReactionManager() : forced(0), next(NONE), reactionReads(0), predicateCalls(0), candidateSeen(0)
    {
        for (int i = 0; i < 79; ++i) {
            targetCondition[i] = 0;
            allowWhileCarrying[i] = true;
            observedFlags[i] = true;
        }
    }
    const CThing* PeekThingOpinionSource() const { return forced; }
    const std::list<CIntelligentPointer<const CThing> >& PeekOpinionSourcesList() const { return sources; }
    EOpinionTargetingConditionType GetTargetingCondition(EOpinionReactionType n) const {
        return (EOpinionTargetingConditionType)targetCondition[n];
    }
    bool GetAllowWhileCarrying(EOpinionReactionType n) const { return allowWhileCarrying[n]; }
    EOpinionReactionType GetReactionType(const CThing&, const bool* flags, int) const {
        ++reactionReads;
        for (int i = 0; i < 79; ++i) observedFlags[i] = flags[i];
        return next;
    }
};
struct CWorld {
    const CPlayer* player;
    COpinionReactionManager reactions;
    CWorld() : player(0) {}
    const COpinionReactionManager& PeekOpinionReactionManager() const { return reactions; }
    const CPlayer* PeekMyPlayer() const { return player; }
};
struct CAIStateGroup_OpinionReactionRoot;
struct COpinionReactionBehaviour {
    bool (CAIStateGroup_OpinionReactionRoot::*UpdateFunc)();
    bool (CAIStateGroup_OpinionReactionRoot::*IsRunnableFunc)();
    COpinionReactionBehaviour() : UpdateFunc(0), IsRunnableFunc(0) {}
};
struct CAIStateGroup_OpinionReactionRoot {
    CThing creature;
    CWorld world;
    float ExtraDataFloat;
    long ExtraDataSLONG;
    CThing* ExtraDataPThing;
    CIntelligentPointer<CThing> ThingReactingTo;
    EOpinionReactionType CurrentReactionType;
    bool ReactingToPlayer;
    bool ReactionFlags[79];
    bool (CAIStateGroup_OpinionReactionRoot::*CurrentUpdateFunc)();
    static COpinionReactionBehaviour Behaviours[79];
    int predicateRejected;
    int predicateAccepted;
    CAIStateGroup_OpinionReactionRoot() : ExtraDataFloat(1), ExtraDataSLONG(7), ExtraDataPThing(&creature),
        CurrentReactionType(NONE), ReactingToPlayer(false), CurrentUpdateFunc(0),
        predicateRejected(0), predicateAccepted(0) {
        memset(ReactionFlags, 1, sizeof(ReactionFlags));
    }
    bool IsRunnable();
    CThing& GetCreature() { return creature; }
    const CWorld& PeekWorld() const { return world; }
    bool PerformUpdate() { return true; }
    bool RejectCandidate() { ++predicateRejected; return false; }
    bool AcceptCandidate() { ++predicateAccepted; return true; }
};
COpinionReactionBehaviour CAIStateGroup_OpinionReactionRoot::Behaviours[79];

class CIsOpinionSourceWithReaction {
    CAIStateGroup_OpinionReactionRoot* owner;
    mutable EOpinionReactionType result;
public:
    CIsOpinionSourceWithReaction(CAIStateGroup_OpinionReactionRoot* p) : owner(p), result(NONE) {}
    bool operator()(const CThing& c) const {
        ++owner->world.reactions.predicateCalls;
        owner->world.reactions.candidateSeen = &c;
        result = c.available;
        return c.candidate;
    }
    EOpinionReactionType PeekReaction() const { return result; }
};

bool CAIStateGroup_OpinionReactionRoot::IsRunnable()
{
    ExtraDataFloat = 0;
    ExtraDataSLONG = 0;
    ExtraDataPThing = 0;
    if (GetCreature().GetProperty() == 3)
        return false;
    const COpinionReactionManager& reaction_mgr = PeekWorld().PeekOpinionReactionManager();
    const CThing* pthing_opinion_source = reaction_mgr.PeekThingOpinionSource();
    CIsOpinionSourceWithReaction is_candidate(this);
    if (!pthing_opinion_source) {
        const std::list<CIntelligentPointer<const CThing> >& sources_list = PeekWorld().PeekOpinionReactionManager().PeekOpinionSourcesList();
        float best_sq_dist = 0.0f;
        for (std::list<CIntelligentPointer<const CThing> >::const_iterator it = sources_list.begin(); it != sources_list.end(); ++it) {
            CIntelligentPointer<const CThing> psource(*it);
            if (IS_THING_ALIVE(psource)) {
                float sq_dist = (GetCreature().GetPos() - psource->GetPos()).GetSquaredMagnitude();
                if ((!pthing_opinion_source || sq_dist < best_sq_dist) && is_candidate(*psource.Get()))
                    pthing_opinion_source = psource.Get();
            }
        }
    }
    if (pthing_opinion_source && is_candidate(*pthing_opinion_source) && is_candidate.PeekReaction() != 0) {
        CurrentReactionType = is_candidate.PeekReaction();
        CurrentUpdateFunc = Behaviours[CurrentReactionType].UpdateFunc;
        return true;
    }
    ThingReactingTo.Set(0);
    if (reaction_mgr.PeekThingOpinionSource())
        return false;
    {
        const CTCPerceiveThing* ptc_perception;
        GetCreature().PeekTC(&ptc_perception);
        const CThing* pperceived_thing = ptc_perception->GetPerceivedThing();
        if (IS_THING_ALIVE(pperceived_thing) && ptc_perception->IsAwareOfThing()) {
            if (pperceived_thing->HasTC(TCI_OPINION_SOURCE))
                return false;
            ThingReactingTo = const_cast<CThing*>(pperceived_thing);
        }
    }
    if (!IS_THING_ALIVE(ThingReactingTo))
        return false;
    // CThing class type 2 identifies a player creature.
    ReactingToPlayer = ThingReactingTo->GetType() == 2 && ThingReactingTo->HasTC(TCI_HERO_OPINION_DEED_LOG);
    const CTCFollowed* ptc_followed;
    const CTCRegionFollower* ptc_region_follow;
    bool is_following = (ThingReactingTo->PeekTC(&ptc_followed) && ptc_followed->IsFollower(&GetCreature())) ||
                        (ThingReactingTo->PeekTC(&ptc_region_follow) && ptc_region_follow->IsRegionFollower(&GetCreature()));
    if (ReactingToPlayer && is_following)
        return false;
    const CTCTargetingBase* ptc_targeting;
    bool targeted = false;
    bool z_targeted = false;
    if (ThingReactingTo->PeekTC(&ptc_targeting) && ptc_targeting->GetPTargetedThing() == &GetCreature()) {
        targeted = true;
        const CPlayer* pplayer = PeekWorld().PeekMyPlayer();
        if (pplayer && pplayer->GetPControlledCreature() == ThingReactingTo && pplayer->IsZTargeting())
            z_targeted = true;
    }
    const CTCCreatureModeManager* ptc_mode_manager;
    unsigned long mode_type = NCreatureMode::MODE_NULL;
    if (GetCreature().PeekTC(&ptc_mode_manager))
        mode_type = ptc_mode_manager->GetCurrentMode().GetType();
    for (long c1 = 0; c1 < 79; ++c1) {
        if (c1 < 18) {
            ReactionFlags[c1] = false;
            continue;
        }
        EOpinionReactionType r = (EOpinionReactionType)c1;
        switch (reaction_mgr.GetTargetingCondition(r)) {
        case OPINION_TARGETING_CONDITION_TYPE_NONE: ReactionFlags[c1] = true; break;
        case 1: ReactionFlags[c1] = !targeted; break;
        case 2: ReactionFlags[c1] = targeted && !z_targeted; break;
        case 3: ReactionFlags[c1] = !z_targeted; break;
        case 4: ReactionFlags[c1] = z_targeted; break;
        case 5: ReactionFlags[c1] = targeted; break;
        }
        if (!ReactionFlags[c1])
            continue;
        if (!reaction_mgr.GetAllowWhileCarrying(r) &&
            (mode_type == NCreatureMode::MODE_CARRYING_GOODS_STAND ||
             mode_type == NCreatureMode::MODE_HOLD_JUG ||
             mode_type == NCreatureMode::MODE_FILL_JUG ||
             mode_type == NCreatureMode::MODE_SERVE_CUSTOMER ||
             mode_type == NCreatureMode::MODE_ARMS_OFF_TABLE ||
             mode_type == NCreatureMode::MODE_SIT_TAVERN_STOOL)) {
            ReactionFlags[c1] = false;
            continue;
        }
        if (ReactionFlags[c1])
            ReactionFlags[c1] = (Behaviours[c1].IsRunnableFunc == 0 ||
                                 (this->*Behaviours[c1].IsRunnableFunc)()) ? true : false;
    }
    EOpinionReactionType reaction = reaction_mgr.GetReactionType(GetCreature(), ReactionFlags, 0);
    if (reaction != (EOpinionReactionType)0) {
        const CTCPerceiveThing* ptc_perception;
        GetCreature().PeekTC(&ptc_perception);
        if (ptc_perception->IsThingIgnoreFOVLineOfSight()) {
            CurrentReactionType = reaction;
            CurrentUpdateFunc = Behaviours[CurrentReactionType].UpdateFunc;
            return true;
        }
    }
    return false;
}

inline bool CTCPerceiveThing::IsAwareOfThing() const
{
    return AwarenessTimer != 0.0f;
}

#define CHECK(expr) do { if (!(expr)) { printf("0095ef80_TEST failure at %d\n", __LINE__); return 1; } } while (0)
struct Fixture {
    CAIStateGroup_OpinionReactionRoot root;
    CThing seen;
    CTCPerceiveThing perception;
    CTCFollowed followed;
    CTCRegionFollower region;
    CTCTargetingBase target;
    CTCCreatureModeManager modes;
    CPlayer player;
    Fixture()
    {
        perception.seen = &seen;
        perception.AwarenessTimer = 1.0f;
        perception.ignores = true;
        root.creature.perception = &perception;
        root.world.reactions.next = SAMPLE_REACTION;
        followed.follower = 0;
        region.follower = 0;
        target.target = 0;
        modes.mode = 0;
        player.controlled = &seen;
        player.z = false;
    }
};
int main()
{
    CAIStateGroup_OpinionReactionRoot::Behaviours[SAMPLE_REACTION].UpdateFunc =
        &CAIStateGroup_OpinionReactionRoot::PerformUpdate;
    CAIStateGroup_OpinionReactionRoot::Behaviours[24].IsRunnableFunc =
        &CAIStateGroup_OpinionReactionRoot::RejectCandidate;
    CAIStateGroup_OpinionReactionRoot::Behaviours[25].IsRunnableFunc =
        &CAIStateGroup_OpinionReactionRoot::AcceptCandidate;
    {
        Fixture f;
        f.root.creature.property = 3;
        CHECK(!f.root.IsRunnable());
        CHECK(f.root.ExtraDataFloat == 0 && f.root.ExtraDataSLONG == 0 && !f.root.ExtraDataPThing);
        CHECK(f.root.world.reactions.reactionReads == 0);
    }
    {
        Fixture f;
        f.seen.candidate = true;
        f.seen.available = SAMPLE_REACTION;
        f.root.world.reactions.forced = &f.seen;
        CHECK(f.root.IsRunnable());
        CHECK(f.root.CurrentReactionType == SAMPLE_REACTION && (f.root.*f.root.CurrentUpdateFunc)());
        CHECK(f.root.world.reactions.predicateCalls == 1 && !f.root.world.reactions.reactionReads);
    }
    {
        Fixture f;
        f.root.world.reactions.forced = &f.seen;
        f.seen.candidate = true;
        CHECK(!f.root.IsRunnable() && !f.root.ThingReactingTo.Get());
    }
    {
        Fixture f;
        CThing dead, ignored, later;
        dead.dead = true;
        ignored.candidate = false;
        f.seen.candidate = true;
        f.seen.available = SAMPLE_REACTION;
        f.seen.pos.X = 7.0f;
        later.candidate = true;
        f.root.world.reactions.sources.push_back(&dead);
        f.root.world.reactions.sources.push_back(&ignored);
        f.root.world.reactions.sources.push_back(&f.seen);
        f.root.world.reactions.sources.push_back(&later);
        CHECK(f.root.IsRunnable() && f.root.world.reactions.predicateCalls == 3);
        CHECK(f.root.world.reactions.candidateSeen == &f.seen);
    }
    {
        Fixture f;
        f.perception.seen = 0;
        CHECK(!f.root.IsRunnable());
    }
    {
        Fixture f;
        f.seen.dead = true;
        CHECK(!f.root.IsRunnable());
    }
    {
        Fixture f;
        f.perception.AwarenessTimer = 0.0f;
        CHECK(!f.root.IsRunnable());
    }
    {
        Fixture f;
        f.seen.itemSource = true;
        CHECK(!f.root.IsRunnable());
    }
    {
        Fixture f;
        f.seen.type = 2;
        f.seen.heroDeeds = true;
        f.seen.followed = &f.followed;
        f.followed.follower = &f.root.creature;
        CHECK(!f.root.IsRunnable() && f.root.ReactingToPlayer);
    }
    {
        Fixture f;
        f.seen.type = 2;
        f.seen.heroDeeds = true;
        f.seen.region = &f.region;
        f.region.follower = &f.root.creature;
        CHECK(!f.root.IsRunnable() && f.root.ReactingToPlayer);
    }
    for (int z = 0; z < 2; ++z) {
        Fixture f;
        f.seen.targeting = &f.target;
        f.target.target = &f.root.creature;
        f.root.world.player = &f.player;
        f.player.z = z != 0;
        for (int i = 0; i < 6; ++i) f.root.world.reactions.targetCondition[18 + i] = i;
        CHECK(f.root.IsRunnable() && f.root.world.reactions.reactionReads == 1);
        const bool* flags = f.root.world.reactions.observedFlags;
        for (int i = 0; i < 18; ++i) CHECK(!flags[i]);
        CHECK(flags[18] && !flags[19] && flags[20] == (z == 0));
        CHECK(flags[21] == (z == 0) && flags[22] == (z != 0) && flags[23]);
        CHECK(!flags[24] && flags[25]);
        CHECK(f.root.predicateRejected == 1 && f.root.predicateAccepted == 1);
    }
    const unsigned long carrying[] = {6, 0x29, 0x2b, 0x2a, 0x2d, 0xb};
    for (unsigned int i = 0; i < sizeof(carrying)/sizeof(*carrying); ++i) {
        Fixture f;
        f.root.creature.modes = &f.modes;
        f.modes.mode = carrying[i];
        f.root.world.reactions.allowWhileCarrying[25] = false;
        CHECK(f.root.IsRunnable() && !f.root.world.reactions.observedFlags[25] && f.root.predicateAccepted == 0);
        CHECK(f.root.CurrentReactionType == SAMPLE_REACTION && f.root.ThingReactingTo.Get() == &f.seen);
    }
    {
        Fixture f;
        f.root.world.reactions.next = NONE;
        CHECK(!f.root.IsRunnable());
    }
    {
        Fixture f;
        f.perception.ignores = false;
        CHECK(!f.root.IsRunnable() && f.root.world.reactions.reactionReads == 1);
    }
    puts("0095ef80_TEST PASS");
    return 0;
}