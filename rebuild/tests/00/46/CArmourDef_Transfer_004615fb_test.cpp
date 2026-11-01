// Behaviour test for CArmourDef::Transfer (0x004615FB): every member is handed to the context once, in declaration order,
// maps through TransferMap and scalars through Transfer<T>.
#include <cstdio>
static const void* g_seen[16];
static int g_kind[16];
static int g_count = 0;
struct MapEOA { int root, first, size; };
struct MapEDA { int root, first, size; };
class CPersistContext {
public:
    void TransferMap(MapEOA& m) { g_seen[g_count] = &m; g_kind[g_count++] = 1; }
    void TransferMap(MapEDA& m) { g_seen[g_count] = &m; g_kind[g_count++] = 2; }
    template <class T> void Transfer(T& v) { g_seen[g_count] = &v; g_kind[g_count++] = (int)sizeof(T) == 1 ? 3 : 4; }
};
class CArmourDef {
public:
    MapEOA AugmentationResponse;
    MapEDA DamageTypeResponse;
    MapEDA ArmourThresholds;
    bool AllHitsKnockdown;
    bool AllHitsNegated;
    bool AllHitsCauseRecoil;
    long DamageMaterial;
    long BlockMaterial;
    void Transfer(CPersistContext& context)
    {
        context.TransferMap(AugmentationResponse);
        context.TransferMap(DamageTypeResponse);
        context.TransferMap(ArmourThresholds);
        context.Transfer(AllHitsKnockdown);
        context.Transfer(AllHitsNegated);
        context.Transfer(AllHitsCauseRecoil);
        context.Transfer(DamageMaterial);
        context.Transfer(BlockMaterial);
    }
};
int main()
{
    CArmourDef def;
    CPersistContext ctx;
    def.Transfer(ctx);
    const void* want[8] = { &def.AugmentationResponse, &def.DamageTypeResponse, &def.ArmourThresholds, &def.AllHitsKnockdown,
                            &def.AllHitsNegated, &def.AllHitsCauseRecoil, &def.DamageMaterial, &def.BlockMaterial };
    int kinds[8] = { 1, 2, 2, 3, 3, 3, 4, 4 };
    if (g_count != 8) { std::printf("004615fb_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        if (g_seen[i] != want[i] || g_kind[i] != kinds[i]) { std::printf("004615fb_TEST FAIL\n"); return 1; }
    std::printf("004615fb_TEST PASS\n");
    return 0;
}