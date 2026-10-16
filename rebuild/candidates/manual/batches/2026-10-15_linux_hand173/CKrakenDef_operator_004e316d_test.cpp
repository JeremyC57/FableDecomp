// behaviour test for CKrakenDef::operator= (0x004E316D): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
struct CLongVector { unsigned long m_words[3]; CLongVector& operator=(const CLongVector& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } };
struct CKrakenDef : CDefClassBase {
    float HeightOffset;
    float PoolRadius;
    float HeadRiseDelay;
    float HeadIdleTime;
    long LowStrikesBeforeHigh;
    long LowStrikeChargeLoops;
    long LowStrikeAttackLoops;
    long HighStrikeAttackLoops;
    long RoarLoops;
    float DelayBeforeLowBeam;
    float DelayBeforeHighBeam;
    float BeamExplosionRadius;
    float BeamTrackSpeed;
    float TargetingTrackSpeed;
    float BeamMouthDistLow;
    float BeamMouthDistHigh;
    long TicksBetweenBeamExplosions;
    CLongVector TentacleDefIndex;
    float TentacleMaxRiseDelay;
    CKrakenDef& operator=(const CKrakenDef& other);
};
CKrakenDef& CKrakenDef::operator=(const CKrakenDef& other)
{
    CDefClassBase::operator=(other);
    HeightOffset = other.HeightOffset;
    PoolRadius = other.PoolRadius;
    HeadRiseDelay = other.HeadRiseDelay;
    HeadIdleTime = other.HeadIdleTime;
    LowStrikesBeforeHigh = other.LowStrikesBeforeHigh;
    LowStrikeChargeLoops = other.LowStrikeChargeLoops;
    LowStrikeAttackLoops = other.LowStrikeAttackLoops;
    HighStrikeAttackLoops = other.HighStrikeAttackLoops;
    RoarLoops = other.RoarLoops;
    DelayBeforeLowBeam = other.DelayBeforeLowBeam;
    DelayBeforeHighBeam = other.DelayBeforeHighBeam;
    BeamExplosionRadius = other.BeamExplosionRadius;
    BeamTrackSpeed = other.BeamTrackSpeed;
    TargetingTrackSpeed = other.TargetingTrackSpeed;
    BeamMouthDistLow = other.BeamMouthDistLow;
    BeamMouthDistHigh = other.BeamMouthDistHigh;
    TicksBetweenBeamExplosions = other.TicksBetweenBeamExplosions;
    TentacleDefIndex = other.TentacleDefIndex;
    TentacleMaxRiseDelay = other.TentacleMaxRiseDelay;
    return *this;
}
int main()
{
    CKrakenDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    CKrakenDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.HeightOffset; const unsigned char* y = (const unsigned char*)&b.HeightOffset; for (unsigned int i = 0; i < sizeof(a.HeightOffset); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.PoolRadius; const unsigned char* y = (const unsigned char*)&b.PoolRadius; for (unsigned int i = 0; i < sizeof(a.PoolRadius); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HeadRiseDelay; const unsigned char* y = (const unsigned char*)&b.HeadRiseDelay; for (unsigned int i = 0; i < sizeof(a.HeadRiseDelay); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HeadIdleTime; const unsigned char* y = (const unsigned char*)&b.HeadIdleTime; for (unsigned int i = 0; i < sizeof(a.HeadIdleTime); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.LowStrikesBeforeHigh; const unsigned char* y = (const unsigned char*)&b.LowStrikesBeforeHigh; for (unsigned int i = 0; i < sizeof(a.LowStrikesBeforeHigh); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.LowStrikeChargeLoops; const unsigned char* y = (const unsigned char*)&b.LowStrikeChargeLoops; for (unsigned int i = 0; i < sizeof(a.LowStrikeChargeLoops); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.LowStrikeAttackLoops; const unsigned char* y = (const unsigned char*)&b.LowStrikeAttackLoops; for (unsigned int i = 0; i < sizeof(a.LowStrikeAttackLoops); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.HighStrikeAttackLoops; const unsigned char* y = (const unsigned char*)&b.HighStrikeAttackLoops; for (unsigned int i = 0; i < sizeof(a.HighStrikeAttackLoops); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RoarLoops; const unsigned char* y = (const unsigned char*)&b.RoarLoops; for (unsigned int i = 0; i < sizeof(a.RoarLoops); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.DelayBeforeLowBeam; const unsigned char* y = (const unsigned char*)&b.DelayBeforeLowBeam; for (unsigned int i = 0; i < sizeof(a.DelayBeforeLowBeam); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.DelayBeforeHighBeam; const unsigned char* y = (const unsigned char*)&b.DelayBeforeHighBeam; for (unsigned int i = 0; i < sizeof(a.DelayBeforeHighBeam); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.BeamExplosionRadius; const unsigned char* y = (const unsigned char*)&b.BeamExplosionRadius; for (unsigned int i = 0; i < sizeof(a.BeamExplosionRadius); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.BeamTrackSpeed; const unsigned char* y = (const unsigned char*)&b.BeamTrackSpeed; for (unsigned int i = 0; i < sizeof(a.BeamTrackSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TargetingTrackSpeed; const unsigned char* y = (const unsigned char*)&b.TargetingTrackSpeed; for (unsigned int i = 0; i < sizeof(a.TargetingTrackSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.BeamMouthDistLow; const unsigned char* y = (const unsigned char*)&b.BeamMouthDistLow; for (unsigned int i = 0; i < sizeof(a.BeamMouthDistLow); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.BeamMouthDistHigh; const unsigned char* y = (const unsigned char*)&b.BeamMouthDistHigh; for (unsigned int i = 0; i < sizeof(a.BeamMouthDistHigh); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TicksBetweenBeamExplosions; const unsigned char* y = (const unsigned char*)&b.TicksBetweenBeamExplosions; for (unsigned int i = 0; i < sizeof(a.TicksBetweenBeamExplosions); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleDefIndex; const unsigned char* y = (const unsigned char*)&b.TentacleDefIndex; for (unsigned int i = 0; i < sizeof(a.TentacleDefIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleMaxRiseDelay; const unsigned char* y = (const unsigned char*)&b.TentacleMaxRiseDelay; for (unsigned int i = 0; i < sizeof(a.TentacleMaxRiseDelay); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e316d_TEST FAIL\n"); return 1; }
    std::printf("004e316d_TEST PASS\n");
    return 0;
}
