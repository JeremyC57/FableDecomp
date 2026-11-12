// Behaviour test for CAIStateGroup_HoverWander::Update (0x00959BF0): a copy of the model with scripted callees.
#include <cstdio>
#include <cmath>
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V operator+(const V& o) const { return V(X + o.X, Y + o.Y, Z + o.Z); }
    V operator-(const V& o) const { return V(X - o.X, Y - o.Y, Z - o.Z); }
    V operator*(float s) const { return V(X * s, Y * s, Z * s); }
    V CrossWithUp() const { return V(Y, -X, 0.0f); }
    void AccurateNormalise() { float m = (float)std::sqrt(X * X + Y * Y + Z * Z); if (m > 0) { X /= m; Y /= m; Z /= m; } } };
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{ *seed *= 0x24A1; *seed += 0x24DF; *seed = ror13(*seed); return range ? *seed % range : 0; }
static double GFSin(float a) { return std::sin(a * (double)6.2831855f); }
static double GFCos(float a) { return std::cos(a * (double)6.2831855f); }
struct Model {
    unsigned long seed; V creature; int wanderFails, lookResult, moveResults[8], moveIdx, terminated, looks, moves; V lastLook; long lastMove;
    bool Wander(V& out) { if (wanderFails > 0) { --wanderFails; return false; } out = creature + V(4, 0, 1); return true; }
    int LookAtPos(V p) { ++looks; lastLook = p; return lookResult; }
    int Move(const V&, const V&, bool&, long m) { ++moves; lastMove = m; return moveResults[moveIdx++ & 7]; }
    void Update() {
        unsigned long* s = &seed;
        for (long i = 0; i != 3; ++i) {
            V target;
            if (!Wander(target)) continue;
            float angle = (float)GFRandom(100, s) * 0.003f + 0.1f;
            if (GFRandom(2, s) == 0) angle *= -1.0f;
            float dist = (float)GFRandom(100, s) * 0.02f + 3.0f;
            V dir = target - creature; dir.Z = 0.0f; dir.AccurateNormalise();
            V perp = dir.CrossWithUp();
            V look = target + perp * (float)GFSin(angle) * dist + dir * (float)GFCos(angle) * dist;
            if (LookAtPos(look) != AI_ACTION_FINISHED) { ++terminated; return; }
            long movement = (long)GFRandom(3, &seed) + AI_MOVEMENT_HOVER_SLOW;
            bool reached; int r;
            do { r = Move(target, look, reached, movement); } while (r == AI_ACTION_IN_PROGRESS);
            if (r != AI_ACTION_FINISHED) { ++terminated; return; }
        }
    }
};
int main()
{
    Model m = {}; m.seed = 12345; m.creature = V(10, 20, 0); m.lookResult = AI_ACTION_FINISHED;
    for (int k = 0; k < 8; ++k) m.moveResults[k] = AI_ACTION_FINISHED;
    m.moveResults[0] = AI_ACTION_IN_PROGRESS;                         // first move keeps running once
    m.Update();
    bool ok = m.terminated == 0 && m.looks == 3 && m.moves == 4 && m.lastMove >= AI_MOVEMENT_HOVER_SLOW && m.lastMove <= AI_MOVEMENT_HOVER_FAST;
    V t = m.creature + V(4, 0, 1); V d = m.lastLook - t;
    float r = (float)std::sqrt(d.X * d.X + d.Y * d.Y);
    ok = ok && r >= 2.99f && r <= 5.0f && std::fabs(d.Z) < 1e-5f && d.X > 0.0f;   // in front of the target, 3..5m away, level
    Model f = m; f.seed = 1; f.looks = f.moves = f.terminated = 0; f.lookResult = AI_ACTION_FAILED; f.Update();
    ok = ok && f.terminated == 1 && f.looks == 1 && f.moves == 0;
    Model w = m; w.looks = w.moves = w.terminated = 0; w.wanderFails = 3; w.Update();
    ok = ok && w.looks == 0 && w.terminated == 0;
    Model x = m; x.looks = x.moves = x.terminated = 0; x.moveIdx = 0; x.moveResults[0] = AI_ACTION_FAILED; x.Update();
    ok = ok && x.terminated == 1 && x.moves == 1;
    if (!ok) { std::printf("00959bf0_TEST FAIL\n"); return 1; }
    std::printf("00959bf0_TEST PASS\n");
    return 0;
}