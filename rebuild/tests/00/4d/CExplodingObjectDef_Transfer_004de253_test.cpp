// Behaviour test for 0x004DE253 CExplodingObjectDef::Transfer(CPersistContext&): the context sees the four members in
// declaration order, each through the transfer of its own type (bool, float, long, long), and can change them in place.
#include <cstdio>

static int g_order[8]; static int g_count = 0;

class CPersistContext {
public:
    int Loading;
    void Transfer(bool& v)  { g_order[g_count++] = 1; if (Loading) v = true; }
    void Transfer(float& v) { g_order[g_count++] = 2; if (Loading) v = 12.5f; }
    void Transfer(long& v)  { g_order[g_count++] = 3; if (Loading) v += 100; }
};

class CExplodingObjectDef {
public:
    virtual ~CExplodingObjectDef() { }
    virtual void Transfer(CPersistContext& context);
    bool  TriggeredOnCreatureProximity;
    float TriggerRadius;
    long  ExplosionDef;
    long  ProximityParticleEffect;
};

void CExplodingObjectDef::Transfer(CPersistContext& context)
{
    context.Transfer(TriggeredOnCreatureProximity);
    context.Transfer(TriggerRadius);
    context.Transfer(ExplosionDef);
    context.Transfer(ProximityParticleEffect);
}

int main()
{
    CExplodingObjectDef d;
    d.TriggeredOnCreatureProximity = false; d.TriggerRadius = 0.0f; d.ExplosionDef = 1; d.ProximityParticleEffect = 2;
    CPersistContext c; c.Loading = 1;
    d.Transfer(c);
    if (g_count != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 3) { std::printf("004de253_TEST FAIL\n"); return 1; }
    if (!d.TriggeredOnCreatureProximity || d.TriggerRadius != 12.5f || d.ExplosionDef != 101 || d.ProximityParticleEffect != 102) { std::printf("004de253_TEST FAIL\n"); return 1; }
    std::printf("004de253_TEST PASS\n");
    return 0;
}