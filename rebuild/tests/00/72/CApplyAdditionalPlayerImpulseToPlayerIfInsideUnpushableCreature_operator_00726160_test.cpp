// Behaviour test for 0x00726160 CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature::operator()(CThing*) const:
// the same logic over a small self-contained model (solid-to-thing exclusion lists, pushable / boss flags, positions, radii,
// collision flags, a recorded impulse, a scripted random source and frame counter).
#include <cstdio>
#include <cmath>

struct Vec3 { float X, Y, Z; };
struct Thing;
struct SolidToThing {
    const Thing* NotSolidTo;
    bool IsSolidToThing(const Thing* t) const { return t != NotSolidTo; }
};
struct Physics {
    Thing* ParentThing;
    Vec3   Position;
    float  Radius;
    bool   Collides;
    bool   PushableByHero;
    Vec3   Facing;
    int    ImpulseCount;
    Vec3   Impulse;
    bool CanCollideWithThings() const { return Collides; }
    bool IsPushableByHero() const { return PushableByHero; }
    Vec3 GetFacingVector() const { return Facing; }
    void ApplyControlledImpulse(const Vec3& v) { ++ImpulseCount; Impulse = v; }
};
struct Thing {
    SolidToThing* Solid;      // TCI_SOLID_TO_THING component, or null
    bool          Boss;       // TCI_BOSS present
    Physics*      PhysicsTC;
    const Vec3& GetPos() const { return PhysicsTC->Position; }
};
static long g_frame;
static float g_random_unit;      // scripted GFFloatRandom fraction of the range
static float g_last_range;
static float g_last_turns;
static long GetFrame() { return g_frame; }
static float FloatRandom(float range) { g_last_range = range; return range * g_random_unit; }
static void RotateXY(Vec3& v, float turns)
{
    g_last_turns = turns;
    float a = turns * 6.2831853f, c = (float)std::cos(a), s = (float)std::sin(a);
    float x = v.X * c - v.Y * s, y = v.X * s + v.Y * c;
    v.X = x; v.Y = y;
}
struct Functor {
    Physics* Player;
    void operator()(Thing* thing) const
    {
        SolidToThing* solid;
        if ((solid = Player->ParentThing->Solid) != 0 && !solid->IsSolidToThing(thing))
            return;
        if ((solid = thing->Solid) != 0 && !solid->IsSolidToThing(Player->ParentThing))
            return;
        if (thing->PhysicsTC->IsPushableByHero())
            return;
        if (!thing->Boss)
            return;
        const Vec3& player_pos = Player->Position;
        const Vec3& pos = thing->GetPos();
        float dx = pos.X - player_pos.X, dy = pos.Y - player_pos.Y;
        float radii = Player->Radius + thing->PhysicsTC->Radius;
        if (std::fabs(pos.Z - player_pos.Z) < radii) {
            if (Player->CanCollideWithThings() && thing->PhysicsTC->CanCollideWithThings()) {
                if (dx * dx + dy * dy < radii) {
                    Vec3 f = Player->GetFacingVector();
                    Vec3 push = { -f.X, -f.Y, -f.Z };
                    float sign = (GetFrame() % 2 == 0) ? -1.0f : 1.0f;
                    float angle = FloatRandom(30.0f) * sign;
                    float turns = angle * (1.0f / 360.0f);
                    RotateXY(push, turns * sign);
                    Player->ApplyControlledImpulse(push);
                }
            }
        }
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    Thing player_thing, boss;
    Physics pp, bp;
    SolidToThing ps, bs;
    int fails = 0;
#define RESET() \
    pp.ParentThing = &player_thing; pp.Position.X = 1.0f; pp.Position.Y = 2.0f; pp.Position.Z = 3.0f; pp.Radius = 0.5f; \
    pp.Collides = true; pp.PushableByHero = true; pp.Facing.X = 1.0f; pp.Facing.Y = 0.0f; pp.Facing.Z = 0.25f; pp.ImpulseCount = 0; \
    bp.ParentThing = &boss; bp.Position.X = 1.5f; bp.Position.Y = 2.5f; bp.Position.Z = 3.5f; bp.Radius = 2.0f; \
    bp.Collides = true; bp.PushableByHero = false; bp.ImpulseCount = 0; \
    player_thing.Solid = 0; player_thing.Boss = false; player_thing.PhysicsTC = &pp; \
    boss.Solid = 0; boss.Boss = true; boss.PhysicsTC = &bp; \
    ps.NotSolidTo = 0; bs.NotSolidTo = 0; g_frame = 0; g_random_unit = 0.0f; g_last_range = 0.0f; g_last_turns = 99.0f;
    Functor f; f.Player = &pp;

    // 1. inside an unpushable boss, no rotation (random 0): impulse is exactly the reversed facing, applied to the PLAYER.
    RESET(); f(&boss);
    if (pp.ImpulseCount != 1 || bp.ImpulseCount != 0) ++fails;
    if (!Near(pp.Impulse.X, -1.0f) || !Near(pp.Impulse.Y, 0.0f) || !Near(pp.Impulse.Z, -0.25f)) ++fails;
    if (g_last_range != 30.0f || !Near(g_last_turns, 0.0f)) ++fails;
    // 2. the sign is applied twice, so the turn is range*unit/360 on even and odd frames alike; a quarter of the range = 7.5 degrees.
    RESET(); g_random_unit = 0.25f; g_frame = 4; f(&boss);
    float even_turns = g_last_turns; Vec3 even_push = pp.Impulse;
    RESET(); g_random_unit = 0.25f; g_frame = 7; f(&boss);
    if (!Near(even_turns, 7.5f / 360.0f) || !Near(g_last_turns, even_turns)) ++fails;
    if (!Near(even_push.X, pp.Impulse.X) || !Near(even_push.Y, pp.Impulse.Y)) ++fails;
    if (!Near(even_push.X, -(float)std::cos(7.5f * 3.14159265f / 180.0f)) || !Near(even_push.Z, -0.25f)) ++fails;
    // negative frame: C++ signed remainder (-3 % 2 == -1, not zero) still takes the +1 sign.
    RESET(); g_random_unit = 0.25f; g_frame = -3; f(&boss);
    if (pp.ImpulseCount != 1 || !Near(g_last_turns, 7.5f / 360.0f)) ++fails;
    // 3. the player's solid-to-thing component excludes the boss: nothing.
    RESET(); player_thing.Solid = &ps; ps.NotSolidTo = &boss; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;
    //    present but solid to the boss: still pushed.
    RESET(); player_thing.Solid = &ps; ps.NotSolidTo = &player_thing; f(&boss);
    if (pp.ImpulseCount != 1) ++fails;
    // 4. the boss's component excludes the player: nothing; present and solid: pushed.
    RESET(); boss.Solid = &bs; bs.NotSolidTo = &player_thing; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;
    RESET(); boss.Solid = &bs; bs.NotSolidTo = &boss; f(&boss);
    if (pp.ImpulseCount != 1) ++fails;
    // 5. pushable by the hero, or not a boss: nothing.
    RESET(); bp.PushableByHero = true; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;
    RESET(); boss.Boss = false; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;
    // 6. vertical separation: |dz| must be strictly below the radius sum (2.5).
    RESET(); bp.Position.Z = 3.0f + 2.5f; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;
    RESET(); bp.Position.Z = 3.0f - 2.4f; f(&boss);
    if (pp.ImpulseCount != 1) ++fails;
    // 7. either side not colliding with things: nothing.
    RESET(); pp.Collides = false; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;
    RESET(); bp.Collides = false; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;
    // 8. the SQUARED XY distance is compared with the radius sum itself: dx=1.5,dy=0 -> 2.25 < 2.5 pushes; dx=1.6 -> 2.56 does not
    //    (although 1.6 is well inside the 2.5 radius sum).
    RESET(); bp.Position.X = 1.0f + 1.5f; bp.Position.Y = 2.0f; f(&boss);
    if (pp.ImpulseCount != 1) ++fails;
    RESET(); bp.Position.X = 1.0f + 1.6f; bp.Position.Y = 2.0f; f(&boss);
    if (pp.ImpulseCount != 0) ++fails;

    if (fails) { std::printf("00726160_TEST FAIL\n"); return 1; }
    std::printf("00726160_TEST PASS\n");
    return 0;
}