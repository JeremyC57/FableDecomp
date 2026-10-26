#include <cstdio>
#include <cstring>
static long g_lookedUp = -1;
static int g_live = 0;
static long g_added = -1;
class CCharString { public:
    long Id;
    CCharString(long id) : Id(id) { ++g_live; }
    CCharString(const CCharString& o) : Id(o.Id) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CDefStringTable { CCharString GetString(long id) { g_lookedUp = id; return CCharString(id * 10); } };
static CDefStringTable g_DefStringTable;
class CCoopSpiritDef { public: long OnHitParticle; long OnStrikeParticle; long DefaultParticle; long AttackTargetParticle; float MeleeTargetRange; };
class CTCCoopSpirit { public:
    const CCoopSpiritDef* PCoopSpiritDef;
    void AddAppearanceParticle(const CCharString& particleName) { g_added = particleName.Id; }
    void OnStrike()
    {
        CCharString particle = g_DefStringTable.GetString(PCoopSpiritDef->OnStrikeParticle);
        AddAppearanceParticle(particle);
    }
};
int main()
{
    CCoopSpiritDef def = { 11, 22, 33, 44, 1.0f };
    CTCCoopSpirit spirit; spirit.PCoopSpiritDef = &def;
    spirit.OnStrike();
    if (g_lookedUp != 22 || g_added != 22 * 10 || g_live != 0) { std::printf("006707e0_TEST FAIL\n"); return 1; }
    std::printf("006707e0_TEST PASS\n");
    return 0;
}