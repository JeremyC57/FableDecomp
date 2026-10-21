// Behaviour test for CRegionDef::operator= @ 0x0045CE48: base part, sound map and every member are copied; returns *this.
#include <cstdio>
#include <cstring>

static int g_baseCopies = 0, g_soundCopies = 0;
struct CParentDefClassBase {
    int Instantiation;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { Instantiation = o.Instantiation; ++g_baseCopies; return *this; }
};
struct CDefString { long m_index; };
struct C2DVector { float X, Y; };
struct CSoundMap {
    int Default;
    CSoundMap& operator=(const CSoundMap& o) { Default = o.Default; ++g_soundCopies; return *this; }
};
struct CRegionDef : CParentDefClassBase {
    long EnvironmentTheme, InternalEnvironmentTheme, InternalLightingChannelCount, CameraManagerSetIndex, CameraManagerSetBuildingIndex;
    CSoundMap SoundMap;
    bool PreventHeroWeaponUse, PreventHeroWillUse;
    CDefString WanderingPopulationScriptDefName;
    long Music, ReverbEnvironmentType;
    float ReverbEnvironmentLevel, InBuildingOcclusionLevel;
    bool UseVillageTime, AllowHenchmen;
    long AttachToCameraFX;
    bool EnableSpotlightShadowMode, CheckAllThingsOnMapForCombatCollision;
    long HeroBreathParticle, MaxNumAttackersOnHero;
    bool WeaponOutCrime, UseCrimes, DisplayCrimes, PermitCrimes;
    unsigned long RegionDescription;
    bool HasSea;
    long LockToFrameRate;
    C2DVector WorldMapOffset, NameGraphicOffset;
    long NameGraphic;
    float FishWeightMult;
    CRegionDef& operator=(const CRegionDef& other);
};

CRegionDef& CRegionDef::operator=(const CRegionDef& other)
{
    CParentDefClassBase::operator=(other);
    EnvironmentTheme = other.EnvironmentTheme;
    InternalEnvironmentTheme = other.InternalEnvironmentTheme;
    InternalLightingChannelCount = other.InternalLightingChannelCount;
    CameraManagerSetIndex = other.CameraManagerSetIndex;
    CameraManagerSetBuildingIndex = other.CameraManagerSetBuildingIndex;
    SoundMap = other.SoundMap;
    PreventHeroWeaponUse = other.PreventHeroWeaponUse;
    PreventHeroWillUse = other.PreventHeroWillUse;
    WanderingPopulationScriptDefName = other.WanderingPopulationScriptDefName;
    Music = other.Music;
    ReverbEnvironmentType = other.ReverbEnvironmentType;
    ReverbEnvironmentLevel = other.ReverbEnvironmentLevel;
    InBuildingOcclusionLevel = other.InBuildingOcclusionLevel;
    UseVillageTime = other.UseVillageTime;
    AllowHenchmen = other.AllowHenchmen;
    AttachToCameraFX = other.AttachToCameraFX;
    EnableSpotlightShadowMode = other.EnableSpotlightShadowMode;
    CheckAllThingsOnMapForCombatCollision = other.CheckAllThingsOnMapForCombatCollision;
    HeroBreathParticle = other.HeroBreathParticle;
    MaxNumAttackersOnHero = other.MaxNumAttackersOnHero;
    WeaponOutCrime = other.WeaponOutCrime;
    UseCrimes = other.UseCrimes;
    DisplayCrimes = other.DisplayCrimes;
    PermitCrimes = other.PermitCrimes;
    RegionDescription = other.RegionDescription;
    HasSea = other.HasSea;
    LockToFrameRate = other.LockToFrameRate;
    WorldMapOffset = other.WorldMapOffset;
    NameGraphicOffset = other.NameGraphicOffset;
    NameGraphic = other.NameGraphic;
    FishWeightMult = other.FishWeightMult;
    return *this;
}

int main()
{
    CRegionDef a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    b.Instantiation = 7; b.EnvironmentTheme = 1; b.CameraManagerSetBuildingIndex = 5; b.SoundMap.Default = 9;
    b.PreventHeroWillUse = true; b.Music = 12; b.ReverbEnvironmentLevel = 0.5f; b.AllowHenchmen = true;
    b.PermitCrimes = true; b.RegionDescription = 77; b.HasSea = true; b.WorldMapOffset.Y = 3.0f;
    b.NameGraphicOffset.X = -2.0f; b.NameGraphic = 4; b.FishWeightMult = 1.5f;
    CRegionDef& r = (a = b);
    int ok = &r == &a && g_baseCopies == 1 && g_soundCopies == 1 && a.Instantiation == 7 && a.EnvironmentTheme == 1
          && a.CameraManagerSetBuildingIndex == 5 && a.SoundMap.Default == 9 && a.PreventHeroWillUse && a.Music == 12
          && a.ReverbEnvironmentLevel == 0.5f && a.AllowHenchmen && a.PermitCrimes && a.RegionDescription == 77
          && a.HasSea && a.WorldMapOffset.Y == 3.0f && a.NameGraphicOffset.X == -2.0f && a.NameGraphic == 4
          && a.FishWeightMult == 1.5f;
    if (!ok) { std::printf("0045ce48_TEST FAIL\n"); return 1; }
    std::printf("0045ce48_TEST PASS\n");
    return 0;
}
