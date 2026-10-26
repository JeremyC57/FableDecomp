// behaviour test for CThingObject::OnDie (0x007038D0): spawn the on-death object with a random spin, base OnDie always runs
#include <cstdio>
#include <cstring>
class C3DVector { public: float X, Y, Z; };
class CCharString { public: const char* m_text; CCharString(const char* t, long = -1) : m_text(t) {} ~CCharString() {} };
static float g_randoms[3]; static int g_rand = 0; static float g_ranges[3];
static float GFFloatRandom(float range, unsigned long* seed) { ++*seed; g_ranges[g_rand] = range; return g_randoms[g_rand++]; }
class CThingObject;
static long g_createdDef = 0; static const C3DVector* g_createdAt = 0; static long g_player = 0; static const char* g_name = 0;
static CThingObject* g_spawn = 0;
static CThingObject* CreateObject(long d, C3DVector* p, long player, float*, unsigned long, const CCharString& n)
{ g_createdDef = d; g_createdAt = p; g_player = player; g_name = n.m_text; return g_spawn; }
class CThingObjectDef { public: long OnDeathObject; };
static unsigned long g_seed = 0; static int g_baseDie = 0;
class CThingPhysical { public:
    C3DVector m_pos; C3DVector m_spin;
    unsigned long* GetRandomSeed_004c7ae0() { return &g_seed; }
    const C3DVector& GetPosition() const { return m_pos; }
    void SetSpin_006a4dc0(const C3DVector* s) { m_spin = *s; }
    void OnDie() { ++g_baseDie; } };
class CThingObject : public CThingPhysical { public:
    const CThingObjectDef* PDef;
    void OnDie()
    {
        const CThingObjectDef* def = PDef;
        if (def->OnDeathObject) {
            C3DVector spin;
            spin.X = 0.1f - GFFloatRandom(0.2f, GetRandomSeed_004c7ae0());
            spin.Y = 0.1f - GFFloatRandom(0.2f, GetRandomSeed_004c7ae0());
            spin.Z = GFFloatRandom(0.3f, GetRandomSeed_004c7ae0()) + 0.2f;
            CThingObject* created = CreateObject(def->OnDeathObject, const_cast<C3DVector*>(&GetPosition()), -1, 0, 0, "");
            if (created) created->SetSpin_006a4dc0(&spin);
        }
        CThingPhysical::OnDie();
    } };
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    CThingObjectDef def; def.OnDeathObject = 0;
    CThingObject o, spawned; o.PDef = &def; spawned.m_spin.X = spawned.m_spin.Y = spawned.m_spin.Z = 9.0f;
    int bad = 0;
    o.OnDie(); if (g_baseDie != 1 || g_rand != 0) bad = 1;                   // nothing to spawn
    def.OnDeathObject = 33; g_spawn = &spawned; g_randoms[0] = 0.05f; g_randoms[1] = 0.15f; g_randoms[2] = 0.1f;
    o.OnDie();
    if (g_baseDie != 2 || g_createdDef != 33 || g_createdAt != &o.m_pos || g_player != -1 || !g_name || std::strcmp(g_name, "") != 0) bad = 1;
    if (!Near(spawned.m_spin.X, 0.05f) || !Near(spawned.m_spin.Y, -0.05f) || !Near(spawned.m_spin.Z, 0.3f) || g_seed != 3) bad = 1;
    if (g_ranges[0] != 0.2f || g_ranges[1] != 0.2f || g_ranges[2] != 0.3f) bad = 1;
    g_spawn = 0; g_rand = 0; o.OnDie(); if (g_baseDie != 3) bad = 1;          // creation failed: no spin
    if (bad) { std::printf("007038d0_TEST FAIL\n"); return 1; }
    std::printf("007038d0_TEST PASS\n");
    return 0;
}