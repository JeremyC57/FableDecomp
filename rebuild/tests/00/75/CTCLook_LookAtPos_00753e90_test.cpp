// Behaviour test for CTCLook::LookAtPos (0x00753E90): nothing happens for a target (nearly) at the parent's position; otherwise
// the look timer is frame + 90, the body flag (1) is dropped when the parent overrides look turning, flags / target / direction
// are stored, the head turn state is 2 with flag 2 else 1 (OneFrameDelay only on a change, TimeToStopLooking = -1), with flag 1
// the body turn state is 2 and CanTurnBody set and a turning animation starts when able, not navigating, not already turning and
// turning the body; without flag 1 the body turn state is 1; the recovery angles come from the physics position and the target.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct World { V parent_pos; bool overridden; bool has_nav; bool navigating; bool able; bool turning_anim; bool turning_body; long frame; };
static float Wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
static bool nearf(float a, float b) { return std::fabs(a - b) < 1e-4f; }
struct Look {
    World* w; long LookTimer, LookFlags, HeadTurnState, BodyTurnState, TimeToStopLooking; bool UsingOldStyleTurning, CanTurnBody, OneFrameDelay;
    V PosToLookAt, DirectionToLookIn; float RecXY, RecYZ; int anim_started; long anim_flags;
    void SetHeadTurnState(long s) { if (HeadTurnState != s) { HeadTurnState = s; OneFrameDelay = true; } TimeToStopLooking = -1; }
    void SetBodyTurnState(long s) { if (BodyTurnState != s) BodyTurnState = s; }
    void LookAtPos(const V& p, long flags)
    {
        V d = { p.X - w->parent_pos.X, p.Y - w->parent_pos.Y, p.Z - w->parent_pos.Z };
        const float eps = 1e-4f;
        float sq = d.X * d.X + d.Y * d.Y + d.Z * d.Z;
        if (sq > eps * eps) {
            LookTimer = w->frame + 90;
            if (w->overridden && (flags & 1)) flags ^= 1;
            LookFlags = flags; PosToLookAt = p; DirectionToLookIn = d; UsingOldStyleTurning = true;
            if (flags & 2) SetHeadTurnState(2); else SetHeadTurnState(1);
            if (flags & 1) {
                SetBodyTurnState(2); CanTurnBody = true;
                if (w->able && sq > eps * eps) {
                    bool naving = w->has_nav && w->navigating;
                    if (!naving && !w->turning_anim && w->turning_body) { ++anim_started; anim_flags = flags; }
                }
            } else SetBodyTurnState(1);
            const V& from = w->parent_pos;
            RecXY = Wrap((float)std::atan2(p.X - from.X, p.Y - from.Y) * 0.15915494f);
            float dx = from.X - p.X, dy = from.Y - p.Y, dz = p.Z - from.Z;
            RecYZ = Wrap((float)std::atan2(dz, std::sqrt(dy * dy + dx * dx)) * 0.15915494f);
        }
    }
};
static void reset(Look& l, World* w) { l.w = w; l.LookTimer = 5; l.LookFlags = 0; l.HeadTurnState = 1; l.BodyTurnState = 1; l.TimeToStopLooking = 77;
    l.UsingOldStyleTurning = false; l.CanTurnBody = false; l.OneFrameDelay = false; l.anim_started = 0; l.anim_flags = -1; l.RecXY = l.RecYZ = 9.0f; }
static int fail(int n) { std::printf("00753e90_TEST FAIL %d\n", n); return 1; }
int main()
{
    World w = { { 1.0f, 2.0f, 0.0f }, false, false, false, true, false, true, 100 };
    Look l; reset(l, &w);
    V same = { 1.0f, 2.0f, 0.0f };
    l.LookAtPos(same, 3);
    if (l.LookTimer != 5 || l.TimeToStopLooking != 77 || l.UsingOldStyleTurning) return fail(1);          // target at the parent: no-op
    V p = { 1.0f, 5.0f, 0.0f };
    l.LookAtPos(p, 3);                                                                                      // head 2 + body
    if (l.LookTimer != 190 || l.LookFlags != 3 || l.HeadTurnState != 2 || !l.OneFrameDelay || l.TimeToStopLooking != -1) return fail(2);
    if (l.BodyTurnState != 2 || !l.CanTurnBody || l.anim_started != 1 || l.anim_flags != 3) return fail(3);
    if (l.DirectionToLookIn.Y != 3.0f || l.PosToLookAt.Y != 5.0f || !l.UsingOldStyleTurning || l.RecXY == 9.0f) return fail(4);
    reset(l, &w); l.HeadTurnState = 1; l.LookAtPos(p, 0);                                                    // head 1 unchanged, body 1
    if (l.HeadTurnState != 1 || l.OneFrameDelay || l.TimeToStopLooking != -1 || l.BodyTurnState != 1 || l.CanTurnBody) return fail(5);
    reset(l, &w); l.BodyTurnState = 2; l.LookAtPos(p, 0);
    if (l.BodyTurnState != 1) return fail(6);
    reset(l, &w); w.overridden = true; l.LookAtPos(p, 3);                                                    // override drops flag 1
    if (l.LookFlags != 2 || l.BodyTurnState != 1 || l.anim_started) return fail(7);
    reset(l, &w); w.overridden = true; l.LookAtPos(p, 2);                                                    // no flag 1: kept
    if (l.LookFlags != 2) return fail(8);
    w.overridden = false;
    reset(l, &w); w.able = false; l.LookAtPos(p, 1);                                                         // not able
    if (l.anim_started || l.BodyTurnState != 2) return fail(9);
    w.able = true;
    reset(l, &w); w.has_nav = true; w.navigating = true; l.LookAtPos(p, 1);                                 // navigating
    if (l.anim_started) return fail(10);
    reset(l, &w); w.has_nav = true; w.navigating = false; l.LookAtPos(p, 1);                                // nav TC, not navigating
    if (l.anim_started != 1) return fail(11);
    reset(l, &w); w.has_nav = false; w.navigating = true; l.LookAtPos(p, 1);                                // no nav TC
    if (l.anim_started != 1) return fail(12);
    reset(l, &w); w.turning_anim = true; l.LookAtPos(p, 1);                                                  // already turning
    if (l.anim_started) return fail(13);
    w.turning_anim = false;
    reset(l, &w); w.turning_body = false; l.LookAtPos(p, 1);                                                 // not turning the body
    if (l.anim_started) return fail(14);
    V up = { 1.0f, 5.0f, 3.0f }, down = { 1.0f, 5.0f, -3.0f }, west = { -2.0f, 2.0f, 0.0f };   // recovery angles (XY / YZ, wrapped)
    reset(l, &w); l.LookAtPos(up, 0);
    if (!nearf(l.RecXY, 0.0f) || !nearf(l.RecYZ, 0.125f)) return fail(15);
    reset(l, &w); l.LookAtPos(down, 0);
    if (!nearf(l.RecYZ, 0.875f)) return fail(16);                                                         // below: wraps to 1 - 1/8
    reset(l, &w); l.LookAtPos(west, 0);
    if (!nearf(l.RecXY, 0.75f) || !nearf(l.RecYZ, 0.0f)) return fail(17);
    std::printf("00753e90_TEST PASS\n");
    return 0;
}