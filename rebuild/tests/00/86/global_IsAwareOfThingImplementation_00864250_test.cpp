// Behaviour test for 0x00864250 (anonymous namespace) IsAwareOfThingImplementation(creature, thing, bool, predicate)
// The model mirrors the source: alive tests, perception component required, self, perceived thing plus
// awareness predicate, field of view, 2D squared distance inside the squared sight radius, optional navigation line of sight.
#include <cstdio>

struct Vec { float X, Y, Z; };
struct Thing { bool dead; Vec pos; };
struct Perceive { const Thing* perceived; bool aware; float sightRadius; bool IsAware() const { return aware; } };
struct Creature : Thing {
    Perceive* perceive;           // 0 when there is no CTCPerceiveThing
    bool hasNavigation; bool losClear; bool inFOV;
    int losQueries;
};

static const int USE_FOV = 1;
static bool Aware(Creature* creature, const Thing* thing, bool checkLOS, bool (Perceive::*pred)() const)
{
    if (thing == 0 || thing->dead) return false;
    if (creature == 0 || creature->dead) return false;
    if (creature->perceive == 0) return false;
    const Perceive* p = creature->perceive;
    if ((const Thing*)creature == thing) return true;
    if (thing == p->perceived && (p->*pred)()) return true;
    if (USE_FOV && !creature->inFOV) return false;
    float r = p->sightRadius;
    float dx = creature->pos.X - thing->pos.X, dy = creature->pos.Y - thing->pos.Y;
    if (dx * dx + dy * dy < r * r) {
        if (!checkLOS) return true;
        ++creature->losQueries;
        if (creature->hasNavigation && creature->losClear) return true;
    }
    return false;
}

int main()
{
    int fail = 0;
    Perceive per = { 0, false, 10.0f };
    Creature c; c.dead = false; c.pos.X = 0; c.pos.Y = 0; c.pos.Z = 0; c.perceive = &per;
    c.hasNavigation = true; c.losClear = true; c.inFOV = true; c.losQueries = 0;
    Thing near = { false, { 3.0f, 4.0f, 100.0f } };      // 2D distance 5 (Z ignored)
    Thing far = { false, { 8.0f, 8.0f, 0.0f } };         // 2D distance ~11.3
    Thing deadT = { true, { 1.0f, 0.0f, 0.0f } };
    if (!Aware(&c, &near, false, &Perceive::IsAware)) fail |= 1;
    if (Aware(&c, &far, false, &Perceive::IsAware)) fail |= 2;
    if (Aware(&c, &deadT, false, &Perceive::IsAware) || Aware(&c, 0, false, &Perceive::IsAware)) fail |= 4;
    if (!Aware(&c, &c, true, &Perceive::IsAware)) fail |= 8;                 // self
    per.perceived = &far; per.aware = true;
    if (!Aware(&c, &far, true, &Perceive::IsAware)) fail |= 16;              // perceived and aware: no distance test
    per.aware = false;
    c.losClear = false;
    if (Aware(&c, &near, true, &Perceive::IsAware) || c.losQueries != 1) fail |= 32;   // blocked line of sight
    c.losClear = true;
    if (!Aware(&c, &near, true, &Perceive::IsAware) || c.losQueries != 2) fail |= 64;
    c.inFOV = false;
    if (Aware(&c, &near, false, &Perceive::IsAware) != !USE_FOV) fail |= 128;
    c.inFOV = true; c.perceive = 0;
    if (Aware(&c, &near, false, &Perceive::IsAware)) fail |= 256;
    c.perceive = &per; c.dead = true;
    if (Aware(&c, &near, false, &Perceive::IsAware)) fail |= 512;
    if (fail) { std::printf("00864250_TEST FAIL %d\n", fail); return 1; }
    std::printf("00864250_TEST PASS\n");
    return 0;
}