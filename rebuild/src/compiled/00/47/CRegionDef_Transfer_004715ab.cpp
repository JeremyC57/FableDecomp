// CRegionDef::Transfer(CPersistContext&) -- serialise every CRegionDef field through the persist context
// Retail 0x004715AB, 442 bytes, `ret 4`. A straight run of CPersistContext::Transfer overloads, one per
// member, in the def's declared order except that the two camera-manager-set indices are transferred late
// (after MaxNumAttackersOnHero) and RegionDescription is transferred before the crime flags.
// Callees: Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A, Transfer(float&) 0x00431061,
// Transfer(unsigned long&) 0x00431020, Transfer(CDefString&) 0x0045228F,
// Transfer(EReverbEnvironmentType&) 0x00454CFA, Transfer(CSoundMap&) 0x00470301,
// Transfer(EWorldMapNameGraphic&) 0x00471765.
// Layout: Ego_r.pdb puts EnvironmentTheme at +0x40; retail reads it at +0x3C (the CDefClassBase prefix is
// 4 bytes smaller in retail) and the CSoundMap member is 0x20 bytes (PDB 0x28: STLport maps are 12 bytes
// in retail), so every later member sits 4 / 12 bytes below its PDB offset. No engine header exists.
// Size-optimised TU: `#pragma optimize("s", on)` (every address goes through eax, as in retail).
#pragma optimize("s", on)
enum EReverbEnvironmentType { REVERB_ENVIRONMENT_DEFAULT };
enum EWorldMapNameGraphic { WORLD_MAP_NAME_GRAPHIC_NONE };
class CDefString { public: long Index; };
class CSoundMap { public: unsigned long _words[8]; };
class C2DVector { public: float X; float Y; };

class CPersistContext { public:
    template <class T> void Transfer(T& value);                     // 0x00431102
    void Transfer(EReverbEnvironmentType& value);   // 0x00454CFA
    void Transfer(CSoundMap& value);                // 0x00470301
    void Transfer(EWorldMapNameGraphic& value);     // 0x00471765
};

class CRegionDef { public:
    unsigned char _defBase[0x38];                   // +0x04 after the vptr
    long EnvironmentTheme;                          // +0x3C
    long InternalEnvironmentTheme;                  // +0x40
    long InternalLightingChannelCount;              // +0x44
    long CameraManagerSetIndex;                     // +0x48
    long CameraManagerSetBuildingIndex;             // +0x4C
    CSoundMap SoundMap;                             // +0x50
    bool PreventHeroWeaponUse;                      // +0x70
    bool PreventHeroWillUse;                        // +0x71
    CDefString WanderingPopulationScriptDefName;    // +0x74
    long Music;                                     // +0x78
    EReverbEnvironmentType ReverbEnvironmentType;   // +0x7C
    float ReverbEnvironmentLevel;                   // +0x80
    float InBuildingOcclusionLevel;                 // +0x84
    bool UseVillageTime;                            // +0x88
    bool AllowHenchmen;                             // +0x89
    long AttachToCameraFX;                          // +0x8C
    bool EnableSpotlightShadowMode;                 // +0x90
    bool CheckAllThingsOnMapForCombatCollision;     // +0x91
    long HeroBreathParticle;                        // +0x94
    long MaxNumAttackersOnHero;                     // +0x98
    bool WeaponOutCrime;                            // +0x9C
    bool UseCrimes;                                 // +0x9D
    bool DisplayCrimes;                             // +0x9E
    bool PermitCrimes;                              // +0x9F
    unsigned long RegionDescription;                // +0xA0
    bool HasSea;                                    // +0xA4
    long LockToFrameRate;                           // +0xA8
    C2DVector WorldMapOffset;                       // +0xAC
    C2DVector NameGraphicOffset;                    // +0xB4
    EWorldMapNameGraphic NameGraphic;               // +0xBC
    float FishWeightMult;                           // +0xC0
    virtual void Transfer(CPersistContext& context);
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