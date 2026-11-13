// Behaviour test for 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable(): a host model of the row's decision chain with
// stand-in things / components (the row's callees are out-of-line game functions), checking each exit and the stored state.
#include <cstdio>
#include <cmath>
#include <cstring>

struct Deed { long DeedType, DeedHandle; const void* Target; };
struct EffectsDef { bool ReactionRestrictToTarget, BlockWhileFollowingPlayer; };
struct Thing {
    bool dead;
    bool hasSource; bool alwaysAware; long virtualDeed, virtualDeedID;  // CTCOpinionSource
    bool hasLog; EffectsDef effects;                                     // CTCHeroOpinionDeedLog
    bool hasFollowed; const Thing* follower;                             // CTCFollowed
    bool hasRegion; const Thing* regionFollower;                         // CTCRegionFollower
    bool fresh;                                                          // the filter's verdict
};
struct Creature {
    Thing self;                                                          // the creature as a thing
    bool hasPerceive; float extendedSight; const Thing* perceived; bool aware, ignoreFOV;
    bool hasOpinion; const Deed* headDeed; float x, y;
};
struct World { const Thing* exclusiveSource; const Thing* nearest; const Thing* hero; long lastBox[4]; };
struct Root { const Thing* ThingReactingTo; long CurrentDeedType, CurrentDeedHandle; bool DeedIsVirtual; };

// GFFloatToLongCeil: fistp(value + 0.5) under round-to-nearest-even, minus one when (result - 1) == value.
static long Ceil(float value)
{
    double s = (double)value + 0.5; double f = std::floor(s); long r = (long)f;
    if (s - f > 0.5 || (s - f == 0.5 && (r & 1))) ++r;
    if ((float)(r - 1) == value) --r;
    return r;
}
static bool Alive(const Thing* t) { return t != 0 && !t->dead; }

static bool IsRunnable(Root& self, Creature& creature, World& world)
{
    const Thing* thing = world.exclusiveSource;
    if (thing == 0 || !thing->fresh) {
        long range = Ceil(15.0f);
        if (creature.hasPerceive) range = Ceil(creature.extendedSight);
        long x = (long)creature.x, y = (long)creature.y;
        world.lastBox[0] = x - range; world.lastBox[1] = y - range; world.lastBox[2] = x + range; world.lastBox[3] = y + range;
        thing = world.nearest;
    }
    if (thing && thing != &creature.self) {
        self.ThingReactingTo = thing; self.DeedIsVirtual = true;
        self.CurrentDeedType = thing->virtualDeed; self.CurrentDeedHandle = thing->virtualDeedID;
        return true;
    }
    if (creature.hasOpinion) {
        const Deed* deed = creature.headDeed;
        if (deed) {
            const Thing* hero = world.hero;
            if (Alive(hero)) {
                bool blocked = hero->hasLog && hero->effects.BlockWhileFollowingPlayer;
                if (blocked) {
                    if (hero->hasFollowed && hero->follower == &creature.self) return false;
                    if (hero->hasRegion && hero->regionFollower == &creature.self) return false;
                }
            }
            if (Alive(hero) && hero->hasSource && hero->alwaysAware) self.ThingReactingTo = hero;
            else {
                if (!creature.hasPerceive || !creature.perceived || !creature.aware || !creature.ignoreFOV) return false;
                self.ThingReactingTo = creature.perceived;
            }
            const Thing* r = self.ThingReactingTo;
            bool reacts = r->hasLog && (!r->effects.ReactionRestrictToTarget || deed->Target == &creature.self);
            if (reacts) {
                self.DeedIsVirtual = false; self.CurrentDeedType = deed->DeedType; self.CurrentDeedHandle = deed->DeedHandle;
                return true;
            }
        }
    }
    return false;
}

int main()
{
    int fail = 0;
    Creature c; std::memset(&c, 0, sizeof c); c.x = 100.7f; c.y = -20.2f;
    Thing src; std::memset(&src, 0, sizeof src); src.fresh = true; src.hasSource = true; src.virtualDeed = 7; src.virtualDeedID = 99;
    World w; std::memset(&w, 0, sizeof w); Root r; std::memset(&r, 0, sizeof r);
    w.exclusiveSource = &src;
    if (!IsRunnable(r, c, w) || !r.DeedIsVirtual || r.CurrentDeedType != 7 || r.CurrentDeedHandle != 99 || r.ThingReactingTo != &src) fail |= 1;
    // stale exclusive source: search box from the default 15.0f range, nothing found, no opinion component
    src.fresh = false; w.nearest = 0; std::memset(&r, 0, sizeof r);
    if (IsRunnable(r, c, w) || w.lastBox[0] != 100 - 15 || w.lastBox[1] != -20 - 15 || w.lastBox[2] != 115 || w.lastBox[3] != -5) fail |= 2;
    // perception widens the box (ceil of 15.2 = 16; an exact 16.0 stays 16)
    c.hasPerceive = true; c.extendedSight = 15.2f; IsRunnable(r, c, w);
    if (w.lastBox[2] != 116) fail |= 4;
    c.extendedSight = 16.0f; IsRunnable(r, c, w);
    if (w.lastBox[2] != 116) fail |= 4;
    // the search returning the creature itself is not a reactee
    w.nearest = &c.self; if (IsRunnable(r, c, w)) fail |= 8;
    // pending hero deed, always-aware hero, unrestricted deed effects
    Deed d = { 12, 345, 0 }; c.hasOpinion = true; c.headDeed = &d;
    Thing hero; std::memset(&hero, 0, sizeof hero); hero.hasSource = true; hero.alwaysAware = true; hero.hasLog = true; w.hero = &hero;
    std::memset(&r, 0, sizeof r); r.DeedIsVirtual = true;
    if (!IsRunnable(r, c, w) || r.DeedIsVirtual || r.CurrentDeedType != 12 || r.CurrentDeedHandle != 345 || r.ThingReactingTo != &hero) fail |= 16;
    // blocked while the creature follows the hero
    hero.effects.BlockWhileFollowingPlayer = true; hero.hasFollowed = true; hero.follower = &c.self;
    if (IsRunnable(r, c, w)) fail |= 32;
    hero.follower = 0; hero.hasRegion = true; hero.regionFollower = &c.self;
    if (IsRunnable(r, c, w)) fail |= 32;
    // dead hero: falls to the perceived thing; restricted deed needs the creature as target
    hero.dead = true; hero.regionFollower = 0;
    Thing seen; std::memset(&seen, 0, sizeof seen); seen.hasLog = true; seen.effects.ReactionRestrictToTarget = true;
    c.perceived = &seen; c.aware = true; c.ignoreFOV = true;
    if (IsRunnable(r, c, w)) fail |= 64;
    d.Target = &c.self;
    if (!IsRunnable(r, c, w) || r.ThingReactingTo != &seen) fail |= 64;
    c.ignoreFOV = false;
    if (IsRunnable(r, c, w)) fail |= 128;
    if (Ceil(15.0f) != 15 || Ceil(14.5f) != 15 || Ceil(-2.5f) != -2) fail |= 256;
    if (fail) { std::printf("009609b0_TEST FAIL %d\n", fail); return 1; }
    std::printf("009609b0_TEST PASS\n");
    return 0;
}
