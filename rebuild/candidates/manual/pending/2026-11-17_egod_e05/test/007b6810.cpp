// Behaviour test for CTCCrateStack::FillStack (0x007B6810): height 0 creates nothing; otherwise the first crate is created from the
// stack's def at the marker for the main player, faces wrap(angle of marker_dir) + (random(0.125) - 0.0625) + 0.5 and is remembered as
// FirstCrate; only height 2 also creates a second crate (at the parent's position) moved to marker + (0, 0, 0.6) with its own twist,
// stopped (SetAsMoving(false)) and remembered as SecondCrate. Each twist draws one random number.
#include <cstdio>
#include <cmath>
#include <cstring>
struct V3 { float X, Y, Z; };
struct Crate { long def; V3 created_at; long player; V3 moved_to; int moves; float facing; int stopped; };
struct World {
    long main_player; V3 parent_pos; float randoms[2]; int draws; Crate crates[3]; int created;
    Crate* Create(long def, const V3& at, long player) { Crate& c = crates[created++]; std::memset(&c, 0, sizeof(c)); c.def = def; c.created_at = at; c.player = player; return &c; }
    float Random(float range) { float r = randoms[draws++]; return r * range; }
};
static float wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
static float angle_xy(const V3& v) { return wrap((float)std::atan2(v.X, v.Y) * 0.15915494f); }
struct Stack {
    World* w; long CrateDefIndex; Crate* FirstCrate; Crate* SecondCrate;
    void FillStack(long height, const V3& marker_pos, const V3& marker_dir)
    {
        if (height) {
            long player_number = w->main_player;
            Crate* crate = w->Create(CrateDefIndex, marker_pos, player_number);
            const float RANGE = 0.125f;
            float adder = w->Random(RANGE); adder -= RANGE / 2.0f;
            crate->facing = angle_xy(marker_dir) + adder + 0.5f;
            FirstCrate = crate;
            if (height == 2) {
                crate = w->Create(CrateDefIndex, w->parent_pos, player_number);
                static float f = 0.6f;                                       // retail 0x0138366C (Ego_d block static f)
                float crate_height = f;
                V3 t_pos = { marker_pos.X, marker_pos.Y, marker_pos.Z + crate_height };
                crate->moved_to = t_pos; ++crate->moves;
                float adder2 = w->Random(RANGE); adder2 -= RANGE / 2.0f;
                crate->facing = angle_xy(marker_dir) + adder2 + 0.5f;
                ++crate->stopped;
                SecondCrate = crate;
            }
        }
    }
};
static int fail(int n) { std::printf("007b6810_TEST FAIL %d\n", n); return 1; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    World w; std::memset(&w, 0, sizeof(w)); w.main_player = 3; w.parent_pos.X = 9; w.randoms[0] = 1.0f; w.randoms[1] = 0.0f;
    V3 pos = { 1, 2, 3 }, dir = { 0, 1, 0 };                          // angle 0
    Stack s = { &w, 42, 0, 0 };
    s.FillStack(0, pos, dir);
    if (w.created || s.FirstCrate || w.draws) return fail(1);
    s.FillStack(1, pos, dir);
    if (w.created != 1 || s.FirstCrate != &w.crates[0] || s.SecondCrate || w.draws != 1) return fail(2);
    if (w.crates[0].def != 42 || w.crates[0].player != 3 || !near(w.crates[0].created_at.Z, 3)) return fail(3);
    if (!near(w.crates[0].facing, 0.0f + 0.0625f + 0.5f)) return fail(4);            // random 1.0 -> +1/16
    std::memset(&w.crates, 0, sizeof(w.crates)); w.created = 0; w.draws = 0;
    V3 dir2 = { -1, 0, 0 };                                              // angle 0.75
    s.FillStack(2, pos, dir2);
    if (w.created != 2 || s.FirstCrate != &w.crates[0] || s.SecondCrate != &w.crates[1] || w.draws != 2) return fail(5);
    if (!near(w.crates[0].facing, 0.75f + 0.0625f + 0.5f)) return fail(6);
    const Crate& top = w.crates[1];
    if (!near(top.created_at.X, 9) || top.moves != 1 || !near(top.moved_to.X, 1) || !near(top.moved_to.Y, 2) || !near(top.moved_to.Z, 3.6f)) return fail(7);
    if (!near(top.facing, 0.75f - 0.0625f + 0.5f) || top.stopped != 1 || w.crates[0].stopped) return fail(8);   // random 0 -> -1/16
    std::memset(&w.crates, 0, sizeof(w.crates)); w.created = 0; w.draws = 0; s.SecondCrate = 0;
    s.FillStack(3, pos, dir);                                            // only exactly 2 makes a second crate
    if (w.created != 1 || s.SecondCrate) return fail(9);
    // audit addition: retail tests the height for zero only (test eax,eax ; je), so a negative height still makes the first crate
    std::memset(&w.crates, 0, sizeof(w.crates)); w.created = 0; w.draws = 0; s.FirstCrate = 0; s.SecondCrate = 0;
    s.FillStack(-1, pos, dir);
    if (w.created != 1 || s.FirstCrate != &w.crates[0] || s.SecondCrate || w.draws != 1) return fail(10);
    std::printf("007b6810_TEST PASS\n");
    return 0;
}
