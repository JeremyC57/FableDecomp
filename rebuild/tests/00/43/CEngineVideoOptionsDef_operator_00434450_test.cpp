// Behaviour test for retail 0x00434450: the candidate source below is embedded verbatim (from src/00434450.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
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
struct CDefinitionManager;
struct CDefClassBase : CDefRootBase {
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
template<class K, class V> struct CVectorMap {          // retail: 14 bytes
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
struct CDefString { long TablePos; };
struct CSubDefInfo;

#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {
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

struct CEngineVideoOptionsDef : CParentDefClassBase {
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

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_root, g_vec;
void CDefRootBase::v00() {}
CDefRootBase& CDefRootBase::operator=(const CDefRootBase& o) { ++g_root; for (int i = 0; i < 6; ++i) m_words[i] = o.m_words[i]; return *this; }
template<class T> CRetailVector<T>& CRetailVector<T>::operator=(const CRetailVector<T>& o) { ++g_vec; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; }
template struct CRetailVector<CSubDefInfo>;
int main() {
    CEngineVideoOptionsDef a, b; unsigned char* pb = (unsigned char*)&b;
    memset(&a, 0, sizeof a); memset(&b, 0, sizeof b);
    int ok = sizeof(CEngineVideoOptionsDef) == 0x78 && (unsigned char*)&b.HiresTextureMemory - pb == 0x3c
          && (unsigned char*)&b.SubDefInfoMap - pb == 0x29 && (unsigned char*)&b.WeatherDensity - pb == 0x74;
    for (int i = 0; i < 6; ++i) a.m_words[i] = 10 + i;
    a.PDefManager = (CDefinitionManager*)0x1234; a.GlobalIndex = 77; a.Setup = 1; a.Template = 0; a.DefaultValsApplied = 1;
    pb[0x24] = 0xfa;                                    /* dst: Template=1, other 5 bits set: must keep bits 3..7 */
    a.InstantiationName.TablePos = 5; a.SubDefInfoMap.m_vector.m_last = (CSubDefInfo*)0x99; a.SubDefInfoMap.m_sorted = true;
    a.HiresTextureMemory = 64; a.DrawDistanceMaximum = 2.5f; a.ShadowBufferSize = 1024; a.ShadowDistanceScale = 0.5f;
    a.EnableGlow = true; a.EnableRepeatedMeshes = true; a.WeatherDensity = 0.25f;
    CEngineVideoOptionsDef* r = &(b = a);
    ok = ok && r == &b && g_root == 1 && g_vec == 1 && b.m_words[5] == 15 && b.GlobalIndex == 77 && b.PDefManager == a.PDefManager;
    ok = ok && pb[0x24] == 0xfd;                        /* bits 0/1/2 = 1/0/1, bits 3..7 kept */
    ok = ok && b.InstantiationName.TablePos == 5 && b.SubDefInfoMap.m_vector.m_last == (CSubDefInfo*)0x99 && b.SubDefInfoMap.m_sorted;
    ok = ok && b.HiresTextureMemory == 64 && b.DrawDistanceMaximum == 2.5f && b.ShadowBufferSize == 1024 && b.ShadowDistanceScale == 0.5f;
    ok = ok && b.EnableGlow && !b.EnableRadialBlur && b.EnableRepeatedMeshes && b.WeatherDensity == 0.25f;
    printf(ok ? "00434450_TEST PASS\n" : "00434450_TEST FAIL\n"); return ok ? 0 : 1;
}