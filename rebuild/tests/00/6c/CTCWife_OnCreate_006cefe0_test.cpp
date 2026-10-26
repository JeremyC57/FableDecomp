#include <cstdio>
static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = (seed * 0x24A1 + 0x24DF) & 0xffffffffUL;
    seed = GFROR13(seed) & 0xffffffffUL;
    if (range != 0) return seed % range;
    return 0;
}
class CHeroMarriageDef { public: long FramesBetweenCullingOldGifts; };
static CHeroMarriageDef g_def;
struct CDefPointer_CHeroMarriageDef { const CHeroMarriageDef* Object; };
class CDefClassBase { public: bool GetSubDef(CDefPointer_CHeroMarriageDef* out) { out->Object = &g_def; return true; } };
struct CDefinitions { CDefClassBase* m_pMarriageDefs; };
static CDefClassBase g_defs; static CDefinitions g_definitions; static unsigned long g_seed;
class CTCWife { public:
    CDefPointer_CHeroMarriageDef PHeroMarriageDef; unsigned long FrameLastCulledGiftsReceived;
    CDefinitions* GetDefinitions() { return &g_definitions; }
    unsigned long* GetPRandomSeed() { return &g_seed; }
    void OnCreate()
    {
        CDefinitions* definitions = GetDefinitions();
        definitions->m_pMarriageDefs->GetSubDef(&PHeroMarriageDef);
        const CHeroMarriageDef* def = PHeroMarriageDef.Object;
        unsigned long* seed = GetPRandomSeed();
        FrameLastCulledGiftsReceived = WorldRandomRange(*seed, def->FramesBetweenCullingOldGifts);
    }
};
int main()
{
    g_definitions.m_pMarriageDefs = &g_defs; g_def.FramesBetweenCullingOldGifts = 100; g_seed = 12345;
    CTCWife w; w.PHeroMarriageDef.Object = 0; w.FrameLastCulledGiftsReceived = 999;
    unsigned long expectSeed = GFROR13((12345UL * 0x24A1 + 0x24DF) & 0xffffffffUL) & 0xffffffffUL;
    w.OnCreate();
    if (w.PHeroMarriageDef.Object != &g_def || g_seed != expectSeed || w.FrameLastCulledGiftsReceived != expectSeed % 100) { std::printf("006cefe0_TEST FAIL\n"); return 1; }
    g_def.FramesBetweenCullingOldGifts = 0; w.OnCreate();
    if (w.FrameLastCulledGiftsReceived != 0) { std::printf("006cefe0_TEST FAIL\n"); return 1; }
    std::printf("006cefe0_TEST PASS\n");
    return 0;
}