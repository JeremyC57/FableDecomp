#include <cstdio>
// behaviour test for CThingCreatureBase::MakeNoise (0x00662110)
class CCharString { public: const char* text; };
struct C3DVector { float X, Y, Z; };
static long g_lookup = 0;
static int g_creates = 0, g_passes = 0;
static float g_radius = 0.0f;
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) { return name.text[0] == 'N' ? g_lookup : 0; }
};
static CDefinitionManager g_defs;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
struct CThing {
    unsigned char m_dead : 1;
    CDefinitionManager* GetDefinitionManager() { return &g_defs; }
    const C3DVector& GetPosition() { return g_pos; }
};
struct CThingNoise : CThing {
    CThing* passed;
    static CThingNoise* Create(long defIndex, const C3DVector& pos, float radius);
    void PassToNoiseComponent(CThing* thing) { ++g_passes; passed = thing; }
};
static CThingNoise g_noise;
static bool g_createFails = false;
CThingNoise* CThingNoise::Create(long defIndex, const C3DVector& pos, float radius)
{
    ++g_creates; g_radius = radius;
    if (g_createFails || defIndex != 42 || pos.Y != 2.0f) return 0;
    return &g_noise;
}
struct CThingCreatureBase : CThing {
    void MakeNoise(const CCharString& noiseDefName);
};
void CThingCreatureBase::MakeNoise(const CCharString& noiseDefName)
{
    long defIndex = GetDefinitionManager()->GetDefGlobalIndexFromName(noiseDefName);
    if (defIndex) {
        CThingNoise* noise = CThingNoise::Create(defIndex, GetPosition(), -1.0f);
        if (noise && !noise->m_dead)
            noise->PassToNoiseComponent(this);
    }
}
int main()
{
    CThingCreatureBase c; c.m_dead = 0;
    CCharString bad; bad.text = "X";
    CCharString good; good.text = "NOISE";
    g_lookup = 42; g_noise.m_dead = 0; g_noise.passed = 0;
    c.MakeNoise(bad);
    if (g_creates != 0) { std::printf("00662110_TEST FAIL\n"); return 1; }
    c.MakeNoise(good);
    if (g_creates != 1 || g_passes != 1 || g_noise.passed != &c || g_radius != -1.0f) { std::printf("00662110_TEST FAIL\n"); return 1; }
    g_noise.m_dead = 1;
    c.MakeNoise(good);
    if (g_creates != 2 || g_passes != 1) { std::printf("00662110_TEST FAIL\n"); return 1; }
    g_createFails = true;
    c.MakeNoise(good);
    if (g_creates != 3 || g_passes != 1) { std::printf("00662110_TEST FAIL\n"); return 1; }
    std::printf("00662110_TEST PASS\n");
    return 0;
}
