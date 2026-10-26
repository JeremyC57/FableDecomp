#include <cstdio>
#include <cstring>
// behaviour test: CEngineThemeDef::Transfer visits the members in the persisted order and round-trips the Passable bit
enum EWaterType { WATER_TYPE_NONE, WATER_TYPE_SEA };
enum EMinimapThemeType { MINIMAP_THEME_NONE, MINIMAP_THEME_ROAD };
class CPersistContext { public:
    const void* order[32];
    int count;
    bool loading;
    bool loadValue;
    const char* lastName;
    bool lastDefault;
    void Transfer(long& value) { order[count++] = &value; }
    void Transfer(float& value) { order[count++] = &value; }
    void Transfer(EMinimapThemeType& value) { order[count++] = &value; }
    void Transfer(EWaterType& value) { order[count++] = &value; }
    void Transfer(const char* name, bool& value, const bool& defaultValue)
    {
        lastName = name; lastDefault = defaultValue;
        if (loading) value = loadValue;
    }
};
class CDefClassBase { public:
    virtual ~CDefClassBase() {}
    virtual void Transfer(CPersistContext& context) {}
};
class CEngineThemeDef : public CDefClassBase { public:
    long BaseTexture, BackgroundTexture, CliffBaseTexture, CliffBackgroundTexture, BaseBumpMap, CliffBumpMap;
    long BaseTextureMaxSize, CliffTextureMaxSize, BaseBumpMapMaxSize, CliffBumpMapMaxSize;
    float BaseTextureSelfIllumination, CliffTextureSelfIllumination, WaterHeight;
    EWaterType WaterType;
    long NoWaterThemeDef, LocalDetailGeneratorDef, GroupDef;
    float Height, MinCameraHeight, CoverValue;
    long MaterialDef;
    EMinimapThemeType MinimapTheme;
    long DestructionEffectDef, DestructionThemeDef;
    float Friction;
    bool Passable : 1;
    bool Other : 1;
    virtual void Transfer(CPersistContext& context);
};
void CEngineThemeDef::Transfer(CPersistContext& context)
{
    context.Transfer(BaseTexture);
    context.Transfer(BackgroundTexture);
    context.Transfer(CliffBaseTexture);
    context.Transfer(CliffBackgroundTexture);
    context.Transfer(BaseBumpMap);
    context.Transfer(CliffBumpMap);
    context.Transfer(BaseTextureMaxSize);
    context.Transfer(CliffTextureMaxSize);
    context.Transfer(BaseBumpMapMaxSize);
    context.Transfer(CliffBumpMapMaxSize);
    context.Transfer(BaseTextureSelfIllumination);
    context.Transfer(CliffTextureSelfIllumination);
    context.Transfer(GroupDef);
    context.Transfer(Height);
    context.Transfer(MinCameraHeight);
    context.Transfer(CoverValue);
    context.Transfer(MaterialDef);
    context.Transfer(MinimapTheme);
    context.Transfer(LocalDetailGeneratorDef);
    context.Transfer(DestructionEffectDef);
    context.Transfer(DestructionThemeDef);
    context.Transfer(Friction);
    context.Transfer(WaterHeight);
    context.Transfer(WaterType);
    context.Transfer(NoWaterThemeDef);
    bool passable = Passable;
    context.Transfer("Passable", passable, true);
    Passable = passable;
}
int main()
{
    CEngineThemeDef def;
    def.Passable = true; def.Other = true;
    CPersistContext ctx;
    ctx.count = 0; ctx.loading = true; ctx.loadValue = false; ctx.lastName = 0; ctx.lastDefault = false;
    def.Transfer(ctx);
    const void* expect[25] = { &def.BaseTexture, &def.BackgroundTexture, &def.CliffBaseTexture, &def.CliffBackgroundTexture,
        &def.BaseBumpMap, &def.CliffBumpMap, &def.BaseTextureMaxSize, &def.CliffTextureMaxSize, &def.BaseBumpMapMaxSize,
        &def.CliffBumpMapMaxSize, &def.BaseTextureSelfIllumination, &def.CliffTextureSelfIllumination, &def.GroupDef,
        &def.Height, &def.MinCameraHeight, &def.CoverValue, &def.MaterialDef, &def.MinimapTheme, &def.LocalDetailGeneratorDef,
        &def.DestructionEffectDef, &def.DestructionThemeDef, &def.Friction, &def.WaterHeight, &def.WaterType, &def.NoWaterThemeDef };
    bool ok = ctx.count == 25;
    for (int i = 0; ok && i < 25; ++i)
        ok = ctx.order[i] == expect[i];
    ok = ok && def.Passable == false && def.Other == true && ctx.lastDefault == true
         && ctx.lastName && std::strcmp(ctx.lastName, "Passable") == 0;
    ctx.count = 0; ctx.loadValue = true;
    def.Transfer(ctx);
    ok = ok && def.Passable == true && def.Other == true;
    if (!ok) { std::printf("0045979b_TEST FAIL\n"); return 1; }
    std::printf("0045979b_TEST PASS\n");
    return 0;
}