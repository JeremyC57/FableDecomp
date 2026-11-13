// Behaviour test for 0x00725500 CTCPhysicsControlled::PerformImpulseBasedMovement: a host model of the row's stepping loop with
// stand-in collision callees (the row's callees are out-of-line physics functions), checking the step count, the distance covered,
// the landscape stop, the thing collision (cylinder / impassable / slope responses), the special-move nudge and the early exits.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
static Vec V(float x, float y, float z) { Vec v; v.X = x; v.Y = y; v.Z = z; return v; }
static float Mag(const Vec& v) { return (float)std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
struct Response { Vec CollidePos, CollideNormal; float Friction; };
struct Other { int kind; float wallX; int interactions; };   // kind 0 cylinder, 1 impassable, 2 sloped; a wall at X = wallX

struct Physics {
    Vec ImpulseVelocity; bool canCollideLandscape; float groundZ; bool special;
    int impassabilityCalls, groundInteractions, steps_taken;
    Other* things[4]; int thingCount;
    void UpdateVelocityForImpassability(const Vec&, Vec&) { ++impassabilityCalls; }
    bool HitThing(const Other& o, const Vec& from, const Vec& to, Response& r)
    {
        if (!(from.X < o.wallX && to.X >= o.wallX)) return false;
        r.CollidePos = V(o.wallX, to.Y, to.Z); r.CollideNormal = V(-1, 0, 0); return true;
    }
    bool HitLandscape(const Vec& p, Response& r)
    {
        if (p.Z >= groundZ) return false;
        r.CollidePos = V(p.X, p.Y, groundZ); r.CollideNormal = V(0, 0, 1); return true;
    }
    void Respond(const Response& r, Vec& v) { float d = v.X * r.CollideNormal.X + v.Y * r.CollideNormal.Y + v.Z * r.CollideNormal.Z; if (d < 0) { v.X -= d * r.CollideNormal.X; v.Y -= d * r.CollideNormal.Y; v.Z -= d * r.CollideNormal.Z; } }

    void Perform(Vec& pos)
    {
        bool landscape_collision = false;
        if (thingCount == 0 && !canCollideLandscape) return;
        Vec moved = V(0, 0, 0), old_pos;
        float speed = Mag(ImpulseVelocity), distance = Mag(ImpulseVelocity);
        Vec step = ImpulseVelocity;
        Response response; response.Friction = 1.0f;
        long i = 0;
        long steps = (long)(speed * 14.0f);
        long max_steps = steps * 2;
        if (steps < 1) steps = 1;
        float r = 1.0f / (float)steps; step.X *= r; step.Y *= r; step.Z *= r;
        bool moving = speed > 1e-4f, can_continue = true, special_move = special;
        if (!moving) return;
        do {
            UpdateVelocityForImpassability(pos, step);
            Vec new_pos = V(pos.X + step.X, pos.Y + step.Y, pos.Z + step.Z);
            bool collided = false;
            for (int t = 0; t < thingCount; ++t) {
                Other& o = *things[t];
                if (HitThing(o, pos, new_pos, response)) {
                    if (o.kind == 0) { Respond(response, step); collided = true; }
                    else if (o.kind == 1) { collided = true; step = V(0, 0, 0); }
                    else { collided = true; Respond(response, step); }
                    ++o.interactions;
                }
            }
            if (!collided) {
                if (canCollideLandscape) {
                    if (HitLandscape(new_pos, response)) { new_pos = response.CollidePos; Respond(response, step); landscape_collision = true; }
                    old_pos = pos; pos = new_pos;
                    moved.X += pos.X - old_pos.X; moved.Y += pos.Y - old_pos.Y; moved.Z += pos.Z - old_pos.Z;
                    ++steps_taken;
                }
            } else if (0.0f - response.CollideNormal.Z < 0.125f)
                can_continue = false;
            if (!collided && special_move) step.Z -= 0.001f;
            if (++i >= steps) {
                float moved_distance = Mag(moved), step_length = Mag(step);
                if (!(moved_distance < distance) || !(step_length > 0.01f)) break;
                float remaining = distance - moved_distance;
                if (can_continue) {
                    if (remaining < step_length) { float s = (remaining + 0.01f) / step_length; step.X *= s; step.Y *= s; step.Z *= s; }
                    ++steps;
                } else moving = false;
                if (steps > max_steps) break;
            }
        } while (moving);
        if (landscape_collision) ++groundInteractions;
    }
};

static Physics Make(float vx, float vy, float vz)
{
    Physics p; p.ImpulseVelocity = V(vx, vy, vz); p.canCollideLandscape = true; p.groundZ = -1000.0f; p.special = false;
    p.impassabilityCalls = 0; p.groundInteractions = 0; p.steps_taken = 0; p.thingCount = 0; return p;
}
static bool Near(float a, float b, float e) { return std::fabs(a - b) < e; }

int main()
{
    int fail = 0;
    // free flight: 2 units at 14 steps per unit = 28 steps, the full distance covered
    Physics p = Make(2.0f, 0.0f, 0.0f); Vec pos = V(0, 0, 0);
    p.Perform(pos);
    if (p.steps_taken < 28 || p.steps_taken > 29 || !Near(pos.X, 2.0f, 0.02f) || p.groundInteractions != 0) fail |= 1;
    // no things and no landscape collision: nothing moves
    p = Make(2.0f, 0.0f, 0.0f); p.canCollideLandscape = false; pos = V(0, 0, 0); p.Perform(pos);
    if (p.impassabilityCalls != 0 || pos.X != 0.0f) fail |= 2;
    // a tiny impulse: one step, then the near-zero check stops before any step
    p = Make(0.00001f, 0.0f, 0.0f); pos = V(0, 0, 0); p.Perform(pos);
    if (p.impassabilityCalls != 0) fail |= 4;
    // falling into the ground: the landscape response stops the drop and calls the ground interaction once
    p = Make(0.0f, 0.0f, -2.0f); p.groundZ = -0.5f; pos = V(0, 0, 0); p.Perform(pos);
    if (!Near(pos.Z, -0.5f, 1e-4f) || p.groundInteractions != 1) fail |= 8;
    // an impassable thing ahead: the move stops at it (normal points along -X, so stepping on is not allowed)
    Other wall = { 1, 0.5f, 0 }; p = Make(2.0f, 0.0f, 0.0f); p.things[0] = &wall; p.thingCount = 1; pos = V(0, 0, 0); p.Perform(pos);
    if (pos.X >= 0.5f || wall.interactions == 0) fail |= 16;
    // the special move lowers each free step by 0.001
    p = Make(1.0f, 0.0f, 0.0f); p.special = true; pos = V(0, 0, 0); p.Perform(pos);
    if (!(pos.Z < -0.01f)) fail |= 32;
    if (fail) { std::printf("00725500_TEST FAIL %d\n", fail); return 1; }
    std::printf("00725500_TEST PASS\n");
    return 0;
}