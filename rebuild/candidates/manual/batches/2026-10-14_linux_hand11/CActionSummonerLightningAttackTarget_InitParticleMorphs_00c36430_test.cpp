#include <cstdio>

static float g_scale = 0.0f; static long g_type = -1; static void* g_table = 0; static int g_activated = 0;
struct CParticleMorphDef { unsigned char _pad[0x70]; float m_rate; };
struct CThingDef { void* _unused; CParticleMorphDef* m_pMorphDef; };
struct CThing { CThingDef* m_pDef; unsigned char _pad[0xc8 - 4]; void* m_pMorphTable; };
struct CScriptOwner { CThing* m_pThing; };
struct CParticleMorphs { unsigned char _body[4]; void SetScale(float scale) { g_scale = scale; } void Init(long morphType, void* morphTable) { g_type = morphType; g_table = morphTable; } };
struct CActionSummonerLightningAttackTarget { CScriptOwner* m_pOwner; unsigned char _pad[0x4c - 4]; CParticleMorphs m_particleMorphs;
    void OnActivate() { ++g_activated; }
    void InitParticleMorphs(long morphType)
    {
        m_particleMorphs.SetScale(1000.0f / m_pOwner->m_pThing->m_pDef->m_pMorphDef->m_rate);
        m_particleMorphs.Init(morphType, m_pOwner->m_pThing->m_pMorphTable);
        OnActivate();
    } };
int main()
{
    CParticleMorphDef md; md.m_rate = 250.0f; CThingDef td; td.m_pMorphDef = &md; CThing t; t.m_pDef = &td; int table; t.m_pMorphTable = &table;
    CScriptOwner o; o.m_pThing = &t; CActionSummonerLightningAttackTarget a; a.m_pOwner = &o;
    a.InitParticleMorphs(5);
    if (g_scale != 4.0f || g_type != 5 || g_table != &table || g_activated != 1) { std::printf("00c36430_TEST FAIL\n"); return 1; }
    std::printf("00c36430_TEST PASS\n"); return 0;
}
