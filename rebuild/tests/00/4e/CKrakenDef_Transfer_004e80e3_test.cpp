#include <cstdio>
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
static void* g_seq[32]; static char g_kind[32]; static int g_n = 0;
class CPersistContext { public:
    void Transfer(float& v) { g_seq[g_n] = &v; g_kind[g_n++] = 'f'; v += 1.0f; }
    void Transfer(long& v) { g_seq[g_n] = &v; g_kind[g_n++] = 'l'; v += 1; }
    void Transfer(CLongVector& v) { g_seq[g_n] = &v; g_kind[g_n++] = 'v'; }
};
class CKrakenDef { public:
    float HeightOffset, PoolRadius, HeadRiseDelay, HeadIdleTime;
    long LowStrikesBeforeHigh, LowStrikeChargeLoops, LowStrikeAttackLoops, HighStrikeAttackLoops, RoarLoops;
    float DelayBeforeLowBeam, DelayBeforeHighBeam, BeamExplosionRadius, BeamTrackSpeed, TargetingTrackSpeed, BeamMouthDistLow, BeamMouthDistHigh;
    long TicksBetweenBeamExplosions;
    CLongVector TentacleDefIndex;
    float TentacleMaxRiseDelay;
    void Transfer(CPersistContext& context);
};
void CKrakenDef::Transfer(CPersistContext& context)
{
    context.Transfer(HeightOffset);
    context.Transfer(PoolRadius);
    context.Transfer(HeadRiseDelay);
    context.Transfer(HeadIdleTime);
    context.Transfer(LowStrikesBeforeHigh);
    context.Transfer(LowStrikeChargeLoops);
    context.Transfer(LowStrikeAttackLoops);
    context.Transfer(HighStrikeAttackLoops);
    context.Transfer(RoarLoops);
    context.Transfer(DelayBeforeLowBeam);
    context.Transfer(DelayBeforeHighBeam);
    context.Transfer(BeamExplosionRadius);
    context.Transfer(BeamTrackSpeed);
    context.Transfer(TargetingTrackSpeed);
    context.Transfer(BeamMouthDistLow);
    context.Transfer(BeamMouthDistHigh);
    context.Transfer(TicksBetweenBeamExplosions);
    context.Transfer(TentacleDefIndex);
    context.Transfer(TentacleMaxRiseDelay);
}
int main()
{
    CKrakenDef d = CKrakenDef(); CPersistContext c;
    d.Transfer(c);
    void* want[19] = { &d.HeightOffset, &d.PoolRadius, &d.HeadRiseDelay, &d.HeadIdleTime, &d.LowStrikesBeforeHigh, &d.LowStrikeChargeLoops,
        &d.LowStrikeAttackLoops, &d.HighStrikeAttackLoops, &d.RoarLoops, &d.DelayBeforeLowBeam, &d.DelayBeforeHighBeam, &d.BeamExplosionRadius,
        &d.BeamTrackSpeed, &d.TargetingTrackSpeed, &d.BeamMouthDistLow, &d.BeamMouthDistHigh, &d.TicksBetweenBeamExplosions, &d.TentacleDefIndex, &d.TentacleMaxRiseDelay };
    const char* expect = "fffflllllfffffffl" "vf";
    if (g_n != 19) { std::printf("004e80e3_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 19; ++i)
        if (g_seq[i] != want[i] || g_kind[i] != expect[i]) { std::printf("004e80e3_TEST FAIL\n"); return 1; }
    if (d.RoarLoops != 1 || d.TentacleMaxRiseDelay != 1.0f) { std::printf("004e80e3_TEST FAIL\n"); return 1; }
    std::printf("004e80e3_TEST PASS\n");
    return 0;
}