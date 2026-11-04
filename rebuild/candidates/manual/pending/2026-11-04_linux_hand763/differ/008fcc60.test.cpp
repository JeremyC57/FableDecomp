// Behaviour test for 0x008FCC60 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <cmath>
struct V { float X, Y, Z; };
struct State { V Position, Velocity, Acceleration; bool CullPhysics; };
static V Avg(const V& a, const V& b, float w) { V r = { (a.X + b.X * w) / (1 + w), (a.Y + b.Y * w) / (1 + w), (a.Z + b.Z * w) / (1 + w) }; return r; }
static void FrameUpdate(State& s) {
    V target = s.Acceleration;
    if (!s.CullPhysics) { V d = { target.X - s.Position.X, target.Y - s.Position.Y, target.Z - s.Position.Z };
        float c = (float)std::tanh((d.X*d.X + d.Y*d.Y + d.Z*d.Z) * 0.075f); target = Avg(s.Position, target, c * c * 5.0f); }
    V step = { target.X - s.Position.X, target.Y - s.Position.Y, target.Z - s.Position.Z };
    s.CullPhysics = false; s.Position = target; s.Velocity = step;
}
int main() {
    State s = { { 0, 0, 0 }, { 0, 0, 0 }, { 2, 0, 0 }, true }; FrameUpdate(s);
    bool ok = s.Position.X == 2 && s.Velocity.X == 2 && !s.CullPhysics;
    s.Acceleration.X = 4; FrameUpdate(s); ok = ok && s.Position.X > 2 && s.Position.X < 4 && s.Velocity.X > 0;
    std::printf(ok ? "008fcc60_TEST PASS\n" : "008fcc60_TEST FAIL\n");
    return ok ? 0 : 1;
}
