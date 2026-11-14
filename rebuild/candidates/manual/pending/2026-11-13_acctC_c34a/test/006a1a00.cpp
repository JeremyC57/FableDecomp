// Behaviour test for 0x006A1A00 CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation: fails when the thing is in no carry slot,
// the slot has no def, the carrier has no graphic appearance, or the slot dummy is missing and the carrier has no physics; the slot
// dummy is asked with flags 0x10 (interpolated) / 0x31; a missing dummy falls back to the physics position and (up (0,0,1), facing);
// a creature's primary slot turns the set half a turn about its up when the global is set; a two-handed hold (creature, mode
// manager, both-hands mode, no action) with a secondary dummy turns the set 0.2 about (pos - secondary) ^ up when that is non-zero.
#include <cstdio>
#include <cstring>
#include <vector>
struct V { float X, Y, Z; V() : X(0), Y(0), Z(0) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Rot { V axis; float turns; };
struct Set { V Up, Forward; std::vector<Rot> rots; void Init(const V& u, const V& f) { Up = u; Forward = f; }
             void Rotate(const V& a, float t) { Rot r; r.axis = a; r.turns = t; rots.push_back(r); } };
struct Def { const char* name; const char* secondary; };
struct Carrier {
    bool appearance, physics, dummy, secondaryDummy, hasModes, bothHands, acting; int type; V dummyPos, secondaryPos, physPos, facing;
    unsigned long seenFlags[2]; const char* seenName[2]; int dummyCalls;
    Carrier() : appearance(true), physics(true), dummy(true), secondaryDummy(true), hasModes(true), bothHands(true), acting(false), type(2),
                dummyCalls(0) {}
    bool GetDummy(const char* name, unsigned long flags, V& pos, Set& o, const Def& d) {
        seenFlags[dummyCalls & 1] = flags; seenName[dummyCalls & 1] = name; ++dummyCalls;
        if (!std::strcmp(name, d.name)) { if (!dummy) return false; pos = dummyPos; o.Init(V(0, 1, 0), V(1, 0, 0)); return true; }
        if (!secondaryDummy) return false; pos = secondaryPos; return true;
    }
};
static bool g_turn = true;
struct Carrying {
    Carrier* parent; int slots[3]; const void* things[3]; int count; const Def* defs[8]; int primarySlot;
    bool Get(const void* thing, V& pos, Set& o, bool interpolate) {
        int i = 0; for (; i < count; ++i) if (things[i] == thing) break;
        if (i == count) return false;
        const Def* def = defs[slots[i]]; if (!def) return false;
        if (!parent->appearance) return false;
        unsigned long flags = 0x10; if (!interpolate) flags = 0x31;
        if (!parent->GetDummy(def->name, flags, pos, o, *def)) {
            if (!parent->physics) return false;
            pos = parent->physPos; o.Init(V(0, 0, 1), parent->facing);
        }
        if (parent->type == 2 && slots[i] == primarySlot && g_turn) o.Rotate(o.Up, 0.5f);
        if (std::strcmp(def->secondary, "") != 0) {
            if (parent->type == 2 && parent->hasModes && parent->bothHands && !parent->acting) {
                V spos; Set so;
                if (parent->GetDummy(def->secondary, flags, spos, so, *def)) {
                    V d(pos.X - spos.X, pos.Y - spos.Y, pos.Z - spos.Z);
                    if (d.X != 0.0f || d.Y != 0.0f || d.Z != 0.0f) {
                        float m = 1.0f; { float q = d.X * d.X + d.Y * d.Y + d.Z * d.Z, r = q; for (int k = 0; k < 30; ++k) r = 0.5f * (r + q / r); m = r; }
                        d = V(d.X / m, d.Y / m, d.Z / m);
                        o.Rotate(V(d.Y * o.Up.Z - d.Z * o.Up.Y, d.Z * o.Up.X - d.X * o.Up.Z, d.X * o.Up.Y - d.Y * o.Up.X), 0.2f);
                    }
                }
            }
        }
        return true;
    }
};
static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { ++fails; std::printf("FAIL %s\n", what); } }
static bool eq(const V& a, float x, float y, float z) { float e = 1e-4f; return a.X - x < e && x - a.X < e && a.Y - y < e && y - a.Y < e && a.Z - z < e && z - a.Z < e; }
int main()
{
    Def one = { "HAND_R", "" }, two = { "HAND_R", "HAND_L" };
    int a, b, other;
    Carrier c; c.dummyPos = V(1, 2, 3); c.secondaryPos = V(1, 2, 1); c.physPos = V(7, 8, 9); c.facing = V(0, 1, 0);
    Carrying t; t.parent = &c; t.count = 2; t.slots[0] = 4; t.things[0] = &a; t.slots[1] = 5; t.things[1] = &b; t.primarySlot = 4;
    for (int i = 0; i < 8; ++i) t.defs[i] = 0; t.defs[4] = &one;
    V pos; Set o;
    check(!t.Get(&other, pos, o, true) && c.dummyCalls == 0, "not carried");
    check(!t.Get(&b, pos, o, true) && c.dummyCalls == 0, "slot without def");
    c.appearance = false; check(!t.Get(&a, pos, o, true) && c.dummyCalls == 0, "no appearance"); c.appearance = true;
    check(t.Get(&a, pos, o, true) && eq(pos, 1, 2, 3) && c.seenFlags[0] == 0x10 && c.dummyCalls == 1 && o.rots.size() == 1 &&
          eq(o.rots[0].axis, 0, 1, 0) && o.rots[0].turns == 0.5f, "dummy, primary turn");
    c = Carrier(); c.dummyPos = V(1, 2, 3); c.physPos = V(7, 8, 9); c.facing = V(0, 1, 0); o = Set(); g_turn = false;
    check(t.Get(&a, pos, o, false) && c.seenFlags[0] == 0x31 && o.rots.empty(), "flags 0x31, global off"); g_turn = true;
    c.type = 1; o = Set(); check(t.Get(&a, pos, o, true) && o.rots.empty(), "not a creature");
    c.type = 2; t.primarySlot = 6; o = Set(); check(t.Get(&a, pos, o, true) && o.rots.empty(), "not the primary slot"); t.primarySlot = 4;
    c.dummy = false; c.physics = false; check(!t.Get(&a, pos, o, true), "no dummy, no physics");
    c.physics = true; o = Set(); check(t.Get(&a, pos, o, true) && eq(pos, 7, 8, 9) && eq(o.Up, 0, 0, 1) && eq(o.Forward, 0, 1, 0) &&
                                       o.rots.size() == 1 && eq(o.rots[0].axis, 0, 0, 1), "physics fallback");
    c.dummy = true; t.defs[4] = &two; c.secondaryPos = V(1, 2, 1); c.dummyCalls = 0; o = Set();
    check(t.Get(&a, pos, o, true) && c.dummyCalls == 2 && !std::strcmp(c.seenName[1], "HAND_L") && c.seenFlags[1] == 0x10 &&
          o.rots.size() == 2 && o.rots[1].turns == 0.2f && eq(o.rots[1].axis, -1, 0, 0), "two-handed turn");   // (0,0,1) ^ (0,1,0)
    c.secondaryPos = V(1, 2, 3); o = Set(); check(t.Get(&a, pos, o, true) && o.rots.size() == 1, "zero direction");
    c.secondaryPos = V(1, 2, 1); c.secondaryDummy = false; o = Set(); check(t.Get(&a, pos, o, true) && o.rots.size() == 1, "no secondary dummy");
    c.secondaryDummy = true; c.acting = true; c.dummyCalls = 0; o = Set(); check(t.Get(&a, pos, o, true) && c.dummyCalls == 1, "performing an action");
    c.acting = false; c.bothHands = false; c.dummyCalls = 0; check(t.Get(&a, pos, o, true) && c.dummyCalls == 1, "one-handed mode");
    c.bothHands = true; c.hasModes = false; c.dummyCalls = 0; check(t.Get(&a, pos, o, true) && c.dummyCalls == 1, "no mode manager");
    c.hasModes = true; c.type = 1; c.dummyCalls = 0; check(t.Get(&a, pos, o, true) && c.dummyCalls == 1, "object carrier");
    if (fails == 0) std::printf("006a1a00_TEST PASS\n");
    return fails;
}
