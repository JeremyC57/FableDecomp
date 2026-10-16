// behaviour test for CTrapDef::operator= (0x004E5C89): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
struct CSoundNameMap { unsigned long m_words[3]; CSoundNameMap& operator=(const CSoundNameMap& o) { ++g_ops; for (int i = 0; i < 3; ++i) m_words[i] = o.m_words[i]; return *this; } }; struct CDefString { long TablePos; };
struct CTrapDef : CDefClassBase {
    CSoundNameMap Sound;
    long TriggerType;
    long TrapType;
    float TriggerRadius;
    CDefString TriggerHelperName;
    float TrapDamage;
    long TriggeredCollisionMesh;
    long UntriggeredCollisionMesh;
    long ExplosionDefIndex;
    long PhysicalObstructionDefIndex;
    long NumShotsToFireBeforeIdle;
    float SecondsIdle;
    float SecondsBetweenShots;
    CTrapDef& operator=(const CTrapDef& other);
};
CTrapDef& CTrapDef::operator=(const CTrapDef& other)
{
    CDefClassBase::operator=(other);
    Sound = other.Sound;
    TriggerType = other.TriggerType;
    TrapType = other.TrapType;
    TriggerRadius = other.TriggerRadius;
    TriggerHelperName = other.TriggerHelperName;
    TrapDamage = other.TrapDamage;
    TriggeredCollisionMesh = other.TriggeredCollisionMesh;
    UntriggeredCollisionMesh = other.UntriggeredCollisionMesh;
    ExplosionDefIndex = other.ExplosionDefIndex;
    PhysicalObstructionDefIndex = other.PhysicalObstructionDefIndex;
    NumShotsToFireBeforeIdle = other.NumShotsToFireBeforeIdle;
    SecondsIdle = other.SecondsIdle;
    SecondsBetweenShots = other.SecondsBetweenShots;
    return *this;
}
int main()
{
    CTrapDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    CTrapDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.Sound; const unsigned char* y = (const unsigned char*)&b.Sound; for (unsigned int i = 0; i < sizeof(a.Sound); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TriggerType; const unsigned char* y = (const unsigned char*)&b.TriggerType; for (unsigned int i = 0; i < sizeof(a.TriggerType); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TrapType; const unsigned char* y = (const unsigned char*)&b.TrapType; for (unsigned int i = 0; i < sizeof(a.TrapType); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TriggerRadius; const unsigned char* y = (const unsigned char*)&b.TriggerRadius; for (unsigned int i = 0; i < sizeof(a.TriggerRadius); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TriggerHelperName; const unsigned char* y = (const unsigned char*)&b.TriggerHelperName; for (unsigned int i = 0; i < sizeof(a.TriggerHelperName); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TrapDamage; const unsigned char* y = (const unsigned char*)&b.TrapDamage; for (unsigned int i = 0; i < sizeof(a.TrapDamage); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TriggeredCollisionMesh; const unsigned char* y = (const unsigned char*)&b.TriggeredCollisionMesh; for (unsigned int i = 0; i < sizeof(a.TriggeredCollisionMesh); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.UntriggeredCollisionMesh; const unsigned char* y = (const unsigned char*)&b.UntriggeredCollisionMesh; for (unsigned int i = 0; i < sizeof(a.UntriggeredCollisionMesh); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ExplosionDefIndex; const unsigned char* y = (const unsigned char*)&b.ExplosionDefIndex; for (unsigned int i = 0; i < sizeof(a.ExplosionDefIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.PhysicalObstructionDefIndex; const unsigned char* y = (const unsigned char*)&b.PhysicalObstructionDefIndex; for (unsigned int i = 0; i < sizeof(a.PhysicalObstructionDefIndex); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.NumShotsToFireBeforeIdle; const unsigned char* y = (const unsigned char*)&b.NumShotsToFireBeforeIdle; for (unsigned int i = 0; i < sizeof(a.NumShotsToFireBeforeIdle); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsIdle; const unsigned char* y = (const unsigned char*)&b.SecondsIdle; for (unsigned int i = 0; i < sizeof(a.SecondsIdle); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecondsBetweenShots; const unsigned char* y = (const unsigned char*)&b.SecondsBetweenShots; for (unsigned int i = 0; i < sizeof(a.SecondsBetweenShots); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e5c89_TEST FAIL\n"); return 1; }
    std::printf("004e5c89_TEST PASS\n");
    return 0;
}