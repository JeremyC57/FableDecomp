// Behaviour test for 0x00735FD0 CTCCombat::AttemptToStartParticle: a weapon without a weapon sub-def changes nothing; otherwise the
// flourish timer is the set's period in frames, the particle list is resized to the number of start points, and for each start / end
// pair (helper point first, dummy object as the fallback; a pair with a missing point ends the whole function) the effect and the
// blend-to effect are created at the weapon; when both are alive the blend emitter is put to sleep, the first emitter is interpolated
// towards it by 2 * (distance - length) / max(blend_length - length, DELTA_NOTIONAL_ZERO), set to always update, attached to the
// weapon (slot 10, start point) and the blend thing is killed. An effect id <= 0 creates nothing for that pair.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct V { float X, Y, Z; };
struct Emitter { float length; bool sleeping, alwaysUpdate; const Emitter* blendedTo; float blendAmount; int attachSlot; const char* attachPoint;
    Emitter() : length(0), sleeping(false), alwaysUpdate(false), blendedTo(0), blendAmount(0), attachSlot(-1), attachPoint(0) {} };
struct Thing { bool dead; bool hasEmitter; Emitter emitter; bool killed; Thing() : dead(false), hasEmitter(true), killed(false) {} };
struct Point { const char* name; V pos; };
struct Weapon {
    bool hasWeaponDef; float period; long effect, blendEffect; std::vector<const char*> starts, ends;
    std::vector<Point> helpers, dummies; int helperCalls, dummyCalls;
    bool Find(const std::vector<Point>& list, const char* name, V& out) const {
        for (size_t i = 0; i < list.size(); ++i) if (!std::strcmp(list[i].name, name)) { out = list[i].pos; return true; }
        return false;
    }
    bool GetHelperPoint(const char* name, V& out) { ++helperCalls; return Find(helpers, name, out); }
    bool GetDummyObject(const char* name, V& out) { ++dummyCalls; return Find(dummies, name, out); }
};
static std::vector<Thing*> created; static std::vector<long> createdEffects;
static float nextLength[8]; static bool nextDead[8];
static Thing* Create(long effect) {
    Thing* t = new Thing; t->emitter.length = nextLength[created.size()]; t->dead = nextDead[created.size()];
    created.push_back(t); createdEffects.push_back(effect); return t;
}
static bool IsThingAlive(const Thing* t) { if (t != 0) { if (!t->dead) return true; } return false; }
struct Combat {
    long CurFlourishTimer; std::vector<Thing*> FlourishParticles;
    void AttemptToStartParticle(Weapon* weapon, float fps) {
        if (weapon->hasWeaponDef) {
            CurFlourishTimer = (long)(fps * weapon->period);
            long count = (long)weapon->starts.size();
            FlourishParticles.resize(count);
            for (long i = 0; i < count; ++i) {
                long blend_effect = weapon->blendEffect;
                long effect = weapon->effect;
                const char* start_name = weapon->starts[i];
                const char* end_name = weapon->ends[i];
                V start_pos, end_pos;
                if (!weapon->GetHelperPoint(start_name, start_pos) && !weapon->GetDummyObject(start_name, start_pos))
                    return;
                if (!weapon->GetHelperPoint(end_name, end_pos) && !weapon->GetDummyObject(end_name, end_pos))
                    return;
                if (effect > 0) {
                    FlourishParticles[i] = Create(effect);
                    Thing* blend_thing = Create(blend_effect);
                    if (IsThingAlive(FlourishParticles[i]) && IsThingAlive(blend_thing)) {
                        Emitter* particles = &FlourishParticles[i]->emitter;
                        Emitter* blend_particles = &blend_thing->emitter;
                        blend_particles->sleeping = true;
                        float dx = start_pos.X - end_pos.X, dy = start_pos.Y - end_pos.Y, dz = start_pos.Z - end_pos.Z;
                        float distance = (float)std::sqrt(dx * dx + dy * dy + dz * dz);
                        float blend_length = blend_particles->length;
                        float length = particles->length;
                        float range = blend_length - length;
                        particles->blendedTo = blend_particles;
                        particles->blendAmount = (distance - length) / (range < DELTA_NOTIONAL_ZERO ? DELTA_NOTIONAL_ZERO : range) * 2.0f;
                        particles->alwaysUpdate = true;
                        particles->attachSlot = 10; particles->attachPoint = start_name;
                        blend_thing->killed = true;
                    }
                }
            }
        }
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f * (1.0f + std::fabs(b)); }
static Point P(const char* n, float x, float y, float z) { Point p; p.name = n; p.pos.X = x; p.pos.Y = y; p.pos.Z = z; return p; }
static void reset() { created.clear(); createdEffects.clear(); for (int i = 0; i < 8; ++i) { nextLength[i] = 0; nextDead[i] = false; } }
static Weapon MakeWeapon() {
    Weapon w; w.hasWeaponDef = true; w.period = 1.5f; w.effect = 7; w.blendEffect = 9; w.helperCalls = w.dummyCalls = 0;
    w.starts.push_back("S0"); w.ends.push_back("E0"); w.starts.push_back("S1"); w.ends.push_back("E1");
    w.helpers.push_back(P("S0", 0, 0, 0)); w.helpers.push_back(P("E0", 3, 4, 0));      // distance 5
    w.dummies.push_back(P("S1", 1, 1, 1)); w.dummies.push_back(P("E1", 1, 1, 3));      // distance 2, dummy fallback
    return w;
}
int main() {
    // no weapon sub-def: nothing changes
    { reset(); Weapon w = MakeWeapon(); w.hasWeaponDef = false; Combat c; c.CurFlourishTimer = -5; c.AttemptToStartParticle(&w, 30.0f);
      check(c.CurFlourishTimer == -5 && c.FlourishParticles.empty() && created.empty() && w.helperCalls == 0, "no weapon def"); }
    // two pairs, the second through the dummy-object fallback
    { reset(); Weapon w = MakeWeapon(); Combat c; c.CurFlourishTimer = 0;
      nextLength[0] = 1.0f; nextLength[1] = 3.0f; nextLength[2] = 4.0f; nextLength[3] = 4.0f;
      c.AttemptToStartParticle(&w, 30.0f);
      check(c.CurFlourishTimer == 45, "timer = fps * period");
      check(c.FlourishParticles.size() == 2 && created.size() == 4, "one effect and one blend effect per pair");
      check(createdEffects[0] == 7 && createdEffects[1] == 9 && createdEffects[2] == 7 && createdEffects[3] == 9, "effect then blend effect");
      check(c.FlourishParticles[0] == created[0] && c.FlourishParticles[1] == created[2], "the effect thing is kept, not the blend thing");
      check(created[1]->emitter.sleeping && created[1]->killed && !created[0]->killed && !created[0]->emitter.sleeping, "blend thing asleep and killed");
      check(created[0]->emitter.blendedTo == &created[1]->emitter && near(created[0]->emitter.blendAmount, (5.0f - 1.0f) / 2.0f * 2.0f), "blend amount pair 0");
      check(created[0]->emitter.alwaysUpdate && created[0]->emitter.attachSlot == 10 && !std::strcmp(created[0]->emitter.attachPoint, "S0"), "attached at the start point");
      check(near(created[2]->emitter.blendAmount, (2.0f - 4.0f) / DELTA_NOTIONAL_ZERO * 2.0f), "zero range clamps to DELTA_NOTIONAL_ZERO");
      check(w.helperCalls == 4 && w.dummyCalls == 2, "dummy object only after a failed helper point"); }
    // a pair with a missing end point ends the function: the timer and the resize stay, later pairs are not started
    { reset(); Weapon w = MakeWeapon(); w.helpers.pop_back(); Combat c; c.CurFlourishTimer = 0; c.AttemptToStartParticle(&w, 20.0f);
      check(c.CurFlourishTimer == 30 && c.FlourishParticles.size() == 2 && created.empty(), "missing end point returns");
      check(c.FlourishParticles[0] == 0 && c.FlourishParticles[1] == 0, "nothing stored"); }
    // missing start point of the second pair: the first pair is complete, the second untouched
    { reset(); Weapon w = MakeWeapon(); w.dummies.erase(w.dummies.begin()); Combat c; c.AttemptToStartParticle(&w, 20.0f);
      check(created.size() == 2 && c.FlourishParticles[0] == created[0] && c.FlourishParticles[1] == 0 && created[1]->killed, "missing start point of pair 1"); }
    // effect id <= 0: nothing is created but the points are still looked up for every pair
    { reset(); Weapon w = MakeWeapon(); w.effect = 0; Combat c; c.AttemptToStartParticle(&w, 20.0f);
      check(created.empty() && w.helperCalls == 4 && c.FlourishParticles.size() == 2, "effect 0 creates nothing"); }
    // a dead blend thing (or a dead effect thing): no blending, no attachment, the blend thing is not killed again
    { reset(); Weapon w = MakeWeapon(); nextDead[1] = true; nextDead[2] = true; Combat c; c.AttemptToStartParticle(&w, 20.0f);
      check(created.size() == 4, "all four created");
      check(created[0]->emitter.blendedTo == 0 && !created[0]->emitter.alwaysUpdate && !created[1]->killed && !created[1]->emitter.sleeping, "dead blend thing");
      check(created[2]->emitter.blendedTo == 0 && !created[3]->killed && !created[3]->emitter.sleeping && c.FlourishParticles[1] == created[2], "dead effect thing"); }
    if (fails == 0) std::printf("00735fd0_TEST PASS\n");
    return fails;
}
