// behaviour test for 005d96a0: CTCInventoryItem::GetEngineGraphic(long) -- a cached def yields a copy of its 14-byte
// CEngineGraphic (taken from def+0x25, unaligned), a failed cache yields the all-zero default graphic
#include <cstdio>
#pragma pack(push, 1)
struct CEngineGraphic {
    unsigned int  BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) {}
};
struct CInventoryItemDef {
    unsigned char _pad[0x25];
    CEngineGraphic Graphic;               /* +0x25 */
    unsigned char _pad33;
    unsigned int ItemDescription;         /* +0x34 */
};
#pragma pack(pop)
static CInventoryItemDef g_defs[2];
static const CInventoryItemDef* s_CachedItemDef = 0;
static bool CacheDef(long defIndex)
{
    if (defIndex < 0 || defIndex >= 2) return false;
    s_CachedItemDef = &g_defs[defIndex];
    return true;
}
static CEngineGraphic GetEngineGraphic(long defIndex)
{
    if (CacheDef(defIndex))
        return s_CachedItemDef->Graphic;
    return CEngineGraphic();
}
int main()
{
    int bad = 0;
    if (sizeof(CEngineGraphic) != 14 || sizeof(CInventoryItemDef) != 0x38) ++bad;
    g_defs[1].Graphic.BankIndex = 0x1234; g_defs[1].Graphic.AnimStep = 0.5f; g_defs[1].Graphic.RenderSizeX = 2.25f;
    g_defs[1].Graphic.AdditiveAlpha = true; g_defs[1].Graphic.Type = 3;
    CEngineGraphic g = GetEngineGraphic(1);
    if (g.BankIndex != 0x1234 || g.AnimStep != 0.5f || g.RenderSizeX != 2.25f || !g.AdditiveAlpha || g.Type != 3) ++bad;
    CEngineGraphic z;
    z.BankIndex = 9; z.AnimStep = 1.0f; z.RenderSizeX = 1.0f; z.AdditiveAlpha = true; z.Type = 7;
    z = GetEngineGraphic(5);              /* no def: the default graphic */
    if (z.BankIndex != 0 || z.AnimStep != 0.0f || z.RenderSizeX != 0.0f || z.AdditiveAlpha || z.Type != 0) ++bad;
    if (bad) { std::printf("005d96a0_TEST FAIL\n"); return 1; }
    std::printf("005d96a0_TEST PASS\n");
    return 0;
}