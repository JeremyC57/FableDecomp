// Behaviour test for CModeScorpionKingTailPlunge::FrameUpdate (0x008E0D20): countdown phases, target lock and dust slide.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
struct Mode {
    int RecoveryCount, DustEffectCounter, SpikeTime, SpikeDecisionTime;
    V TargetPos, TailPos, Dust;
    bool targetAlive, dustAlive, animDone;
    V owner, target, forward;
    int spikes, kills, recovers;
};
static void Frame(Mode& m)
{
    if (m.RecoveryCount)
        --m.RecoveryCount;
    if (m.RecoveryCount == m.SpikeDecisionTime) {
        if (m.targetAlive) {
            m.TargetPos = m.target;
            V d = { m.TargetPos.X - m.owner.X, m.TargetPos.Y - m.owner.Y, m.TargetPos.Z - m.owner.Z };
            if (std::sqrt(d.X * d.X + d.Y * d.Y) < 6.0f) {
                float inv = 1.0f / std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
                m.TargetPos.X = m.owner.X + d.X * inv * 6.0f;
                m.TargetPos.Y = m.owner.Y + d.Y * inv * 6.0f;
                m.TargetPos.Z = m.owner.Z + d.Z * inv * 6.0f;
            }
        } else {
            m.TargetPos.X = m.owner.X + m.forward.X * 8.0f;
            m.TargetPos.Y = m.owner.Y + m.forward.Y * 8.0f;
            m.TargetPos.Z = m.owner.Z + m.forward.Z * 8.0f;
        }
        m.DustEffectCounter = 0;
    } else if (m.RecoveryCount == m.SpikeTime) {
        ++m.spikes;
        if (m.dustAlive) { ++m.kills; m.dustAlive = false; }
    } else if (m.RecoveryCount <= 0) {
        if (m.animDone) { ++m.recovers; return; }
    }
    if (m.RecoveryCount < m.SpikeDecisionTime && m.RecoveryCount > m.SpikeTime && m.dustAlive) {
        float inv = 1.0f / (float)(m.SpikeDecisionTime - m.SpikeTime);
        m.Dust.X += (m.TargetPos.X - m.TailPos.X) * inv;
        m.Dust.Y += (m.TargetPos.Y - m.TailPos.Y) * inv;
        m.Dust.Z += (m.TargetPos.Z - m.TailPos.Z) * inv;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    Mode m;
    m.RecoveryCount = 11; m.DustEffectCounter = 7; m.SpikeTime = 4; m.SpikeDecisionTime = 10;
    V zero = { 0, 0, 0 };
    m.TailPos = zero; m.Dust = zero; m.TargetPos = zero;
    m.targetAlive = true; m.dustAlive = true; m.animDone = false;
    m.owner = zero; V t = { 3.0f, 0.0f, 0.0f }; m.target = t; V f = { 0, 1, 0 }; m.forward = f;
    m.spikes = m.kills = m.recovers = 0;
    Frame(m);   // 10: decision -> target pulled out to 6 units
    int ok = m.RecoveryCount == 10 && Near(m.TargetPos.X, 6.0f) && m.DustEffectCounter == 0;
    for (int i = 0; i < 5; ++i) Frame(m);   // 9..5: five dust steps of 1 unit
    ok = ok && Near(m.Dust.X, 5.0f);
    Frame(m);   // 4: spike, dust killed
    ok = ok && m.spikes == 1 && m.kills == 1;
    for (int i = 0; i < 4; ++i) Frame(m);   // 3..0
    ok = ok && m.recovers == 0;
    m.animDone = true; Frame(m);
    ok = ok && m.recovers == 1;
    // dead target: 8 units ahead
    Mode n = m; n.RecoveryCount = 11; n.targetAlive = false; Frame(n);
    ok = ok && Near(n.TargetPos.Y, 8.0f) && Near(n.TargetPos.X, 0.0f);
    std::printf(ok ? "008e0d20_TEST PASS\n" : "008e0d20_TEST FAIL\n");
    return ok ? 0 : 1;
}