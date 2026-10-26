// CRegionDef::operator=(const CRegionDef&) -- memberwise copy: CParentDefClassBase part, environment/camera indices, sound map, hero-use/crime/village flags, reverb, world-map offsets and fish weight; returns *this
#pragma optimize("s", on)
// Leaf: operator=
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CE48, length 357 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, CSoundMap::operator= @ 0x0045CFAD.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CRegionDef's
// vtable 0x01233AAC is the 5-byte `jmp 0x0045CE48` right before this body.
// Layout: PDB CRegionDef (0xD0) members +0x40.. sit at retail +0x3c.. (retail CParentDefClassBase 0x3c bytes);
// CSoundMap is 0x28 in the PDB but 0x20 in retail (+0x50..+0x70), so every later member is PDB-8.
// Shape: memberwise assignment in declaration order; CDefString is a 4-byte handle copied as a dword, bools as
// byte moves, the two C2DVector offsets as two dwords each, the sound map through its out-of-line operator=.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x38];      // +0x04 CDefClassBase part, InstantiationName, SubDefInfoMap
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

class CDefString { public:                         // 4-byte def-string handle (trivially copyable)
    long m_index;
};

class C2DVector { public:
    float X;
    float Y;
};

class CSoundMap { public:                          // retail 0x20 bytes (a sound-name map plus its default)
    unsigned char _body[0x20];
    CSoundMap& operator=(const CSoundMap& other);   // 0x0045CFAD
};

class CRegionDef : public CParentDefClassBase { public:   // vtable 0x01233AAC
    long       EnvironmentTheme;                       // +0x3c
    long       InternalEnvironmentTheme;               // +0x40
    long       InternalLightingChannelCount;           // +0x44
    long       CameraManagerSetIndex;                  // +0x48
    long       CameraManagerSetBuildingIndex;          // +0x4c
    CSoundMap  SoundMap;                               // +0x50
    bool       PreventHeroWeaponUse;                   // +0x70
    bool       PreventHeroWillUse;                     // +0x71
    CDefString WanderingPopulationScriptDefName;       // +0x74
    long       Music;                                  // +0x78
    long       ReverbEnvironmentType;                  // +0x7c EReverbEnvironmentType
    float      ReverbEnvironmentLevel;                 // +0x80
    float      InBuildingOcclusionLevel;               // +0x84
    bool       UseVillageTime;                         // +0x88
    bool       AllowHenchmen;                          // +0x89
    long       AttachToCameraFX;                       // +0x8c
    bool       EnableSpotlightShadowMode;              // +0x90
    bool       CheckAllThingsOnMapForCombatCollision;  // +0x91
    long       HeroBreathParticle;                     // +0x94
    long       MaxNumAttackersOnHero;                  // +0x98
    bool       WeaponOutCrime;                         // +0x9c
    bool       UseCrimes;                              // +0x9d
    bool       DisplayCrimes;                          // +0x9e
    bool       PermitCrimes;                           // +0x9f
    unsigned long RegionDescription;                   // +0xa0
    bool       HasSea;                                 // +0xa4
    long       LockToFrameRate;                        // +0xa8
    C2DVector  WorldMapOffset;                         // +0xac
    C2DVector  NameGraphicOffset;                      // +0xb4
    long       NameGraphic;                            // +0xbc EWorldMapNameGraphic
    float      FishWeightMult;                         // +0xc0
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