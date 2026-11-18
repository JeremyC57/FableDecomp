// Behaviour test for CTCPhysicsStandard::Move (0x008E5D40): a resting thing does nothing; a moving one applies forces, gathers
// things with its speed, copies its position, runs the impassability check only with flag 0x200, steps, and -- only if still
// moving -- rescales the velocity to the old speed when flag 0x80 (preserve KE) is set, settles on the surface, updates the
// moving flag and commits the stepped position.
#include <cstdio>
#include <cmath>
#include <vector>
struct V { float X, Y, Z; };
struct Phys {
    bool moving, stop_in_step; long flags; V pos, vel; float speed_seen; int forces, impass, steps, surface, upd, setpos; V committed; size_t things;
    void PerformForceUpdates() { ++forces; vel.X *= 2.0f; vel.Y *= 2.0f; vel.Z *= 2.0f; }
    void GetPossibleInteractedWithList(std::vector<int>& l, float speed) { speed_seen = speed; l.push_back(1); l.push_back(2); }
    void UpdateVelocityForImpassability(const V&, V& v) { ++impass; v.Z = 0.0f; }
    void PerformStepBasedMovement(std::vector<int>& l, V& p) { ++steps; things = l.size(); p.X += vel.X; p.Y += vel.Y; p.Z += vel.Z; vel.X += 1.0f; if (stop_in_step) moving = false; }
    void Move()
    {
        if (!moving) return;
        PerformForceUpdates();
        float speed = std::sqrt(vel.X * vel.X + vel.Y * vel.Y + vel.Z * vel.Z);
        bool preserve_ke = (flags & 0x80) != 0;
        std::vector<int> thing_list;
        GetPossibleInteractedWithList(thing_list, speed);
        V current_pos = pos;
        if (flags & 0x200) UpdateVelocityForImpassability(current_pos, vel);
        PerformStepBasedMovement(thing_list, current_pos);
        if (!moving) return;
        if (preserve_ke) {
            float m = std::sqrt(vel.X * vel.X + vel.Y * vel.Y + vel.Z * vel.Z);
            vel.X = vel.X / m * speed; vel.Y = vel.Y / m * speed; vel.Z = vel.Z / m * speed;
        }
        ++surface; ++upd; ++setpos; committed = current_pos;
    }
};
static Phys make(bool moving, long flags) { Phys p; p.moving = moving; p.stop_in_step = false; p.flags = flags; V a = { 1, 2, 3 }; V v = { 3, 0, 4 };
    p.pos = a; p.vel = v; p.speed_seen = -1; p.forces = p.impass = p.steps = p.surface = p.upd = p.setpos = 0; V z = { 0, 0, 0 }; p.committed = z; p.things = 0; return p; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static int fail(int n) { std::printf("008e5d40_TEST FAIL %d\n", n); return 1; }
int main()
{
    Phys p = make(false, 0x280); p.Move();
    if (p.forces || p.steps || p.setpos) return fail(1);                                      // not moving: nothing
    p = make(true, 0); p.Move();
    if (p.forces != 1 || !near(p.speed_seen, 10.0f) || p.impass || p.steps != 1 || p.things != 2) return fail(2);
    if (p.setpos != 1 || !near(p.committed.X, 7.0f) || !near(p.committed.Z, 11.0f) || p.surface != 1 || p.upd != 1) return fail(3);
    if (!near(p.vel.X, 7.0f)) return fail(4);                                                // no preserve: velocity left as stepped
    p = make(true, 0x200); p.Move();
    if (p.impass != 1 || !near(p.committed.Z, 3.0f)) return fail(5);                         // impassability before the step
    p = make(true, 0x80); p.Move();
    if (!near(std::sqrt(p.vel.X * p.vel.X + p.vel.Y * p.vel.Y + p.vel.Z * p.vel.Z), 10.0f)) return fail(6);   // speed restored
    p = make(true, 0x80); p.stop_in_step = true; p.Move();
    if (p.setpos || p.surface || p.upd || !near(p.vel.X, 7.0f)) return fail(7);              // stopped in the step: no tail
    std::printf("008e5d40_TEST PASS\n");
    return 0;
}