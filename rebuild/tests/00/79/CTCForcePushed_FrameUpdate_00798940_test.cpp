// Behaviour test for CTCForcePushed::FrameUpdate (0x00798940): the creature is put in the being-pushed action every frame; inside the
// push distance (squared, strict) it is moved to its position + the push direction with the facing set to the wrapped angle of the
// reversed direction and StartedMoving set -- unless it already started moving and the physics' last position + direction is more
// than 0.2 x the per-frame distance (2D, strict) from where it is: then, for a physical thing (type flag 2), the remaining distance
// (push distance - travelled) is passed on and a HIT_EXPLOSION effect is created when the def has a special-effects sub-def that knows
// it; FinishBeingForcePushed(true, remaining) ends it. Outside the push distance FinishBeingForcePushed(false, 0).
#include <cstdio>
#include <cmath>
#include <cstring>
struct V3 { float X, Y, Z; };
struct Env {
    V3 pos, last; unsigned long type_flags; bool has_subdef; bool knows_effect; long effect_id;
    int actions; int finishes; bool fin_reversed; float fin_damage; int moves; V3 move_to; bool move_flag; float facing;
    int effects; long effect_made; V3 effect_pos; int lookups; char looked_up[32];
};
static float wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
struct Pushed {
    Env* e; V3 start; float distance; V3 dir; float per_frame; bool started;
    void Finish(bool r, float d) { ++e->finishes; e->fin_reversed = r; e->fin_damage = d; }
    void FrameUpdate()
    {
        ++e->actions;
        V3 cur = e->pos;
        float dx = cur.X - start.X, dy = cur.Y - start.Y, dz = cur.Z - start.Z;
        float dsq = dx * dx + dy * dy + dz * dz;
        if (dsq < distance * distance) {
            if (started) {
                float ex = e->last.X + dir.X - cur.X, ey = e->last.Y + dir.Y - cur.Y;
                float lim = per_frame * 0.2f;
                if (ex * ex + ey * ey > lim * lim) {
                    float left = 0.0f;
                    if (e->type_flags & 2) {
                        left = distance - (float)std::sqrt(dsq);
                        long effect = 0;
                        bool ok = false;
                        if (e->has_subdef) {
                            ++e->lookups; std::strcpy(e->looked_up, "HIT_EXPLOSION");
                            if (e->knows_effect) { effect = e->effect_id; ok = true; }
                        }
                        if (ok) { ++e->effects; e->effect_made = effect; e->effect_pos = cur; }
                    }
                    Finish(true, left);
                    return;
                }
            }
            ++e->moves; e->move_to.X = cur.X + dir.X; e->move_to.Y = cur.Y + dir.Y; e->move_to.Z = cur.Z + dir.Z; e->move_flag = true;
            e->facing = wrap((float)std::atan2(-dir.X, -dir.Y) * 0.15915494f);
            started = true;
        } else
            Finish(false, 0.0f);
    }
};
static int fail(int n) { std::printf("00798940_TEST FAIL %d\n", n); return 1; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static Env env() { Env e; std::memset(&e, 0, sizeof(e)); e.type_flags = 3; e.has_subdef = true; e.knows_effect = true; e.effect_id = 7; return e; }
static Pushed pushed(Env* e) { Pushed p = { e, { 0, 0, 0 }, 10.0f, { 1, 0, 0 }, 1.0f, false }; return p; }
int main()
{
    // first frame inside the distance: move along +X, face back (-X: angle 0.75), start moving
    Env e = env(); e.pos.X = 2; Pushed p = pushed(&e); p.FrameUpdate();
    if (e.actions != 1 || e.moves != 1 || e.finishes || !p.started) return fail(1);
    if (!near(e.move_to.X, 3) || !near(e.move_to.Y, 0) || !e.move_flag || !near(e.facing, 0.75f)) return fail(2);
    // a direction along +Y faces 0.5; one along -Y faces 0.0 (wrap of exactly 0)
    e = env(); p = pushed(&e); p.dir.X = 0; p.dir.Y = 1; p.FrameUpdate(); if (!near(e.facing, 0.5f)) return fail(3);
    // outside (or exactly at) the push distance: finish(false, 0)
    e = env(); e.pos.X = 10; p = pushed(&e); p.FrameUpdate();
    if (e.finishes != 1 || e.fin_reversed || e.fin_damage != 0.0f || e.moves) return fail(4);
    // moving and on track (last + dir == pos): keep moving
    e = env(); e.pos.X = 2; e.last.X = 1; p = pushed(&e); p.started = true; p.FrameUpdate();
    if (e.moves != 1 || e.finishes) return fail(5);
    // moving, off track by exactly 0.2 x per-frame: still fine (strict); just past it: blocked
    e = env(); e.pos.X = 2; e.last.X = 2.0f; p = pushed(&e); p.started = true; p.per_frame = 5.0f; p.FrameUpdate();
    if (e.moves != 1 || e.finishes) return fail(6);
    e = env(); e.pos.X = 2; e.last.X = 1.5f; p = pushed(&e); p.started = true; p.FrameUpdate();
    if (e.finishes != 1 || !e.fin_reversed || !near(e.fin_damage, 8.0f) || e.moves) return fail(7);
    if (e.effects != 1 || e.effect_made != 7 || std::strcmp(e.looked_up, "HIT_EXPLOSION") || !near(e.effect_pos.X, 2)) return fail(8);
    // blocked but not yet started: no check, keeps moving
    e = env(); e.pos.X = 2; e.last.X = 5; p = pushed(&e); p.FrameUpdate(); if (e.moves != 1 || e.finishes) return fail(9);
    // blocked, not a physical thing: no remaining distance, no effect
    e = env(); e.type_flags = 1; e.pos.X = 2; e.last.X = 5; p = pushed(&e); p.started = true; p.FrameUpdate();
    if (e.finishes != 1 || e.fin_damage != 0.0f || e.effects || e.lookups) return fail(10);
    // blocked, no special-effects sub-def / effect unknown: remaining distance, no effect
    e = env(); e.has_subdef = false; e.pos.X = 2; e.last.X = 5; p = pushed(&e); p.started = true; p.FrameUpdate();
    if (e.effects || e.lookups || !near(e.fin_damage, 8.0f)) return fail(11);
    e = env(); e.knows_effect = false; e.pos.X = 2; e.last.X = 5; p = pushed(&e); p.started = true; p.FrameUpdate();
    if (e.effects || e.lookups != 1 || !near(e.fin_damage, 8.0f)) return fail(12);
    // the off-track test is 2D: a Z difference alone does not block
    e = env(); e.pos.X = 2; e.pos.Z = 3; e.last.X = 1; e.last.Z = 0; p = pushed(&e); p.started = true; p.FrameUpdate();
    if (e.moves != 1 || e.finishes) return fail(13);
    // audit addition (constant pin, optional): off track by 1.125 with a per-frame distance of 5 is blocked (1.125 > 0.2 x 5)
    e = env(); e.pos.X = 2; e.last.X = 2.125f; p = pushed(&e); p.started = true; p.per_frame = 5.0f; p.FrameUpdate();
    if (e.finishes != 1 || e.moves) return fail(14);
    std::printf("00798940_TEST PASS\n");
    return 0;
}