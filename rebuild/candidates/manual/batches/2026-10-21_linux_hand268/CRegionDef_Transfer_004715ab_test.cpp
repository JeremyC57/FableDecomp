// Behaviour test for CRegionDef::Transfer (0x004715AB): the persist context must see every member once,
// in retail's order, through the overload of the member's type.
#include <cstdio>
enum EReverbEnvironmentType { REVERB_ENVIRONMENT_DEFAULT };
enum EWorldMapNameGraphic { WORLD_MAP_NAME_GRAPHIC_NONE };
struct CDefString { long Index; };
struct CSoundMap { unsigned long _words[8]; };
struct C2DVector { float X; float Y; };

static const void* g_seen[64];
static char g_kind[64];
static int g_count = 0;
static void Note(const void* p, char k) { if (g_count < 64) { g_seen[g_count] = p; g_kind[g_count] = k; } ++g_count; }

struct CPersistContext {
    void Transfer(long& value) { Note(&value, 'l'); }
    void Transfer(bool& value) { Note(&value, 'b'); }
    void Transfer(float& value) { Note(&value, 'f'); }
    void Transfer(unsigned long& value) { Note(&value, 'u'); }
    void Transfer(CDefString& value) { Note(&value, 's'); }
    void Transfer(EReverbEnvironmentType& value) { Note(&value, 'r'); }
    void Transfer(CSoundMap& value) { Note(&value, 'm'); }
    void Transfer(EWorldMapNameGraphic& value) { Note(&value, 'n'); }
};

struct CRegionDef {
    long EnvironmentTheme; long InternalEnvironmentTheme; long InternalLightingChannelCount;
    long CameraManagerSetIndex; long CameraManagerSetBuildingIndex; CSoundMap SoundMap;
    bool PreventHeroWeaponUse; bool PreventHeroWillUse; CDefString WanderingPopulationScriptDefName;
    long Music; EReverbEnvironmentType ReverbEnvironmentType; float ReverbEnvironmentLevel;
    float InBuildingOcclusionLevel; bool UseVillageTime; bool AllowHenchmen; long AttachToCameraFX;
    bool EnableSpotlightShadowMode; bool CheckAllThingsOnMapForCombatCollision; long HeroBreathParticle;
    long MaxNumAttackersOnHero; bool WeaponOutCrime; bool UseCrimes; bool DisplayCrimes; bool PermitCrimes;
    unsigned long RegionDescription; bool HasSea; long LockToFrameRate; C2DVector WorldMapOffset;
    C2DVector NameGraphicOffset; EWorldMapNameGraphic NameGraphic; float FishWeightMult;
    void Transfer(CPersistContext& context);
};

void CRegionDef::Transfer(CPersistContext& context)
{
    context.Transfer(EnvironmentTheme);
    context.Transfer(InternalEnvironmentTheme);
    context.Transfer(InternalLightingChannelCount);
    context.Transfer(SoundMap);
    context.Transfer(PreventHeroWeaponUse);
    context.Transfer(PreventHeroWillUse);
    context.Transfer(WanderingPopulationScriptDefName);
    context.Transfer(Music);
    context.Transfer(ReverbEnvironmentType);
    context.Transfer(ReverbEnvironmentLevel);
    context.Transfer(InBuildingOcclusionLevel);
    context.Transfer(UseVillageTime);
    context.Transfer(AllowHenchmen);
    context.Transfer(AttachToCameraFX);
    context.Transfer(EnableSpotlightShadowMode);
    context.Transfer(CheckAllThingsOnMapForCombatCollision);
    context.Transfer(HeroBreathParticle);
    context.Transfer(MaxNumAttackersOnHero);
    context.Transfer(CameraManagerSetIndex);
    context.Transfer(CameraManagerSetBuildingIndex);
    context.Transfer(RegionDescription);
    context.Transfer(WeaponOutCrime);
    context.Transfer(UseCrimes);
    context.Transfer(DisplayCrimes);
    context.Transfer(PermitCrimes);
    context.Transfer(HasSea);
    context.Transfer(LockToFrameRate);
    context.Transfer(WorldMapOffset.X);
    context.Transfer(WorldMapOffset.Y);
    context.Transfer(NameGraphicOffset.X);
    context.Transfer(NameGraphicOffset.Y);
    context.Transfer(NameGraphic);
    context.Transfer(FishWeightMult);
}

int main()
{
    CRegionDef def;
    CPersistContext context;
    def.Transfer(context);
    const void* want[33] = {
        &def.EnvironmentTheme, &def.InternalEnvironmentTheme, &def.InternalLightingChannelCount, &def.SoundMap,
        &def.PreventHeroWeaponUse, &def.PreventHeroWillUse, &def.WanderingPopulationScriptDefName, &def.Music,
        &def.ReverbEnvironmentType, &def.ReverbEnvironmentLevel, &def.InBuildingOcclusionLevel, &def.UseVillageTime,
        &def.AllowHenchmen, &def.AttachToCameraFX, &def.EnableSpotlightShadowMode,
        &def.CheckAllThingsOnMapForCombatCollision, &def.HeroBreathParticle, &def.MaxNumAttackersOnHero,
        &def.CameraManagerSetIndex, &def.CameraManagerSetBuildingIndex, &def.RegionDescription,
        &def.WeaponOutCrime, &def.UseCrimes, &def.DisplayCrimes, &def.PermitCrimes, &def.HasSea,
        &def.LockToFrameRate, &def.WorldMapOffset.X, &def.WorldMapOffset.Y, &def.NameGraphicOffset.X,
        &def.NameGraphicOffset.Y, &def.NameGraphic, &def.FishWeightMult };
    const char* kinds = "lllmbbslrffbblbbllllubbbbblffffnf";
    if (g_count != 33) { std::printf("004715ab_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 33; ++i)
        if (g_seen[i] != want[i] || g_kind[i] != kinds[i]) { std::printf("004715ab_TEST FAIL\n"); return 1; }
    std::printf("004715ab_TEST PASS\n");
    return 0;
}
