// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CEngineVideoOptionsDef::operator=(const CEngineVideoOptionsDef&) -- def copy with the CDefClassBase/CParentDefClassBase chain inlined, then the video-option fields
// Retail: Fable.exe (TLC, VC7.1) @ 0x00434450, 240 bytes.
// Callees: CDefClassBase's polymorphic root operator= @ 0x00431F60 (copies +0x04..+0x18; manifest name
//   "CTCPhysicsBase::ConstructFromParams" is propagated) and SubDefInfoMap's 12-byte vector operator= @ 0x00431F90.
// Layout: same def base chain as the landed CFrontEndDef::operator= 0x00432E60 -- the retail CParentDefClassBase is
//   byte-packed (dword at +0x25, vector at +0x29, sorted flag at +0x36 are all unaligned in the retail copy), so the
//   #pragma pack(1) reproduces a proven native layout. No engine/ header exists for these classes.
//   CEngineVideoOptionsDef fields use the PDB names/types, 4 bytes earlier than the Ego_r layout (base is 0x3c in TLC).
#pragma pack(push, 1)
struct CDefRootBase {                                   // 28-byte polymorphic root of CDefClassBase
    virtual void v00();
    unsigned long m_words[6];                           // +0x04..+0x18
    CDefRootBase& operator=(const CDefRootBase& other); // 0x00431F60
};
class CDefinitionManager;
class CDefClassBase : public CDefRootBase { public:
    CDefinitionManager* PDefManager;                    // +0x1c
    unsigned long GlobalIndex;                          // +0x20
    unsigned char Setup : 1;                            // +0x24 bit 0
    unsigned char Template : 1;                         // +0x24 bit 1
    unsigned char DefaultValsApplied : 1;               // +0x24 bit 2
    __forceinline CDefClassBase& operator=(const CDefClassBase& other)
    {
        CDefRootBase::operator=(other);
        PDefManager = other.PDefManager;
        GlobalIndex = other.GlobalIndex;
        Setup = other.Setup;
        Template = other.Template;
        DefaultValsApplied = other.DefaultValsApplied;
        return *this;
    }
};
#pragma pack(pop)

template<class T> struct CRetailVector {                // begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector& operator=(const CRetailVector& other); // CSubDefInfo instance: 0x00431F90
};
struct CKeyPairCompareLess {};
template<class K, class V> class CVectorMap { public:          // retail: 14 bytes
    CRetailVector<V>    m_vector;                       // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0c (empty)
    bool                m_sorted;                       // +0x0d
    __forceinline CVectorMap& operator=(const CVectorMap& other)
    {
        m_vector = other.m_vector;
        m_compare = other.m_compare;
        m_sorted = other.m_sorted;
        return *this;
    }
};
class CDefString { public: long TablePos; };
struct CSubDefInfo;

#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase { public:
    CDefString                             InstantiationName;   // +0x25
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;       // +0x29
    __forceinline CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        CDefClassBase::operator=(other);
        InstantiationName = other.InstantiationName;
        SubDefInfoMap = other.SubDefInfoMap;
        return *this;
    }
};
#pragma pack(pop)

class CEngineVideoOptionsDef : public CParentDefClassBase { public:
    long  HiresTextureMemory;                           // +0x3c
    float LODErrorTolerance;                            // +0x40
    float CharacterLODErrorTolerance;                   // +0x44
    float DrawDistanceMultiplier;                       // +0x48
    float DrawDistanceMinimum;                          // +0x4c
    float DrawDistanceMaximum;                          // +0x50
    float RepeatedMeshDrawDistanceFactor;               // +0x54
    float MinimumZSpriteAsMeshDistance;                 // +0x58
    float MaximumZSpriteAsMeshDistance;                 // +0x5c
    float ZSpriteDrawDistanceMultiplier;                // +0x60
    long  ShadowBufferSize;                             // +0x64
    float ShadowDistanceScale;                          // +0x68
    bool  Enable2DDisplacement;                         // +0x6c
    bool  Enable3DDisplacement;                         // +0x6d
    bool  EnableGlow;                                   // +0x6e
    bool  EnableRadialBlur;                             // +0x6f
    bool  EnableWaterReflection;                        // +0x70
    bool  EnableWeatherEffects;                         // +0x71
    bool  EnableColourFilter;                           // +0x72
    bool  EnableRepeatedMeshes;                         // +0x73
    float WeatherDensity;                               // +0x74
    CEngineVideoOptionsDef& operator=(const CEngineVideoOptionsDef& other);
};

CEngineVideoOptionsDef& CEngineVideoOptionsDef::operator=(const CEngineVideoOptionsDef& other)
{
    CParentDefClassBase::operator=(other);
    HiresTextureMemory = other.HiresTextureMemory;
    LODErrorTolerance = other.LODErrorTolerance;
    CharacterLODErrorTolerance = other.CharacterLODErrorTolerance;
    DrawDistanceMultiplier = other.DrawDistanceMultiplier;
    DrawDistanceMinimum = other.DrawDistanceMinimum;
    DrawDistanceMaximum = other.DrawDistanceMaximum;
    RepeatedMeshDrawDistanceFactor = other.RepeatedMeshDrawDistanceFactor;
    MinimumZSpriteAsMeshDistance = other.MinimumZSpriteAsMeshDistance;
    MaximumZSpriteAsMeshDistance = other.MaximumZSpriteAsMeshDistance;
    ZSpriteDrawDistanceMultiplier = other.ZSpriteDrawDistanceMultiplier;
    ShadowBufferSize = other.ShadowBufferSize;
    ShadowDistanceScale = other.ShadowDistanceScale;
    Enable2DDisplacement = other.Enable2DDisplacement;
    Enable3DDisplacement = other.Enable3DDisplacement;
    EnableGlow = other.EnableGlow;
    EnableRadialBlur = other.EnableRadialBlur;
    EnableWaterReflection = other.EnableWaterReflection;
    EnableWeatherEffects = other.EnableWeatherEffects;
    EnableColourFilter = other.EnableColourFilter;
    EnableRepeatedMeshes = other.EnableRepeatedMeshes;
    WeatherDensity = other.WeatherDensity;
    return *this;
}