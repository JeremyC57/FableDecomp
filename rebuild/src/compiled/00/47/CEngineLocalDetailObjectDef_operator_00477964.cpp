// CEngineLocalDetailObjectDef::operator=(CEngineLocalDetailObjectDef&) -- assign the native parent state, rendering values, and the eight PDB-named flags
// Retail: Fable.exe function 0x00477964, 294 bytes.
// Callee: CParentDefClassBase::operator= @ 0x00431ED0.
// The real PDB headers are included below under aliases so their donor evidence and assertions stay visible. Their flattened donor sizes (0x3d parent, 0x8c detail object) conflict with retail: native derived members begin at +0x3c and the native object ends at +0x88. This assignment-only native projection therefore uses an explicit vptr data member and a 0x3c parent extent; no synthetic virtual slot is declared. The PDB lists the packed flags in declaration order as CastShadows, ReceiveShadows, IsRepeatedMesh, IsZSprite, AlphaIsBoolean, HasLandscapeNormalLighting, TiltToSlope, HasWindSkew.
#pragma optimize("s", on)
#include "rebuild_abi.h"
#ifdef _MSC_VER
#undef FABLE_STATIC_ASSERT
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_parent_donor_assert_, __LINE__)[(expression) ? 1 : -1]
#endif
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#ifdef _MSC_VER
#undef FABLE_STATIC_ASSERT
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_detail_donor_assert_, __LINE__)[(expression) ? 1 : -1]
#endif
#define CEngineLocalDetailObjectDef CEngineLocalDetailObjectDefDonor
#include "engine/_quarantine/CEngineLocalDetailObjectDef.h"
#undef CEngineLocalDetailObjectDef
#ifdef _MSC_VER
#undef FABLE_STATIC_ASSERT
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_native_projection_assert_, __LINE__)[(expression) ? 1 : -1]
#endif

struct CParentDefClassBase {
    void* __nativeVftable;
    unsigned char _nativeBaseTail[0x38];
    CParentDefClassBase* operator=(CParentDefClassBase& other); // 0x00431ED0
};

struct CEngineLocalDetailObjectDef : CParentDefClassBase {
    unsigned long Mesh;
    unsigned long ShadowMesh;
    float Probability;
    float ThemeBlendThreshold;
    float Scale;
    float ScaleRandomElement;
    float FadeStart;
    float FadeEnd;
    unsigned long ZSpriteMesh;
    float ZSpriteFadeStart;
    float ZSpriteFadeEnd;
    float SlopeFadeStart;
    float SlopeFadeEnd;
    unsigned long AlphaRef;
    float AlphaMipBias;
    float WindSkewConstantFactor;
    float WindSkewRandomFactor;
    float WindSkewSpeedFactor;
    unsigned char CastShadows : 1;
    unsigned char ReceiveShadows : 1;
    unsigned char IsRepeatedMesh : 1;
    unsigned char IsZSprite : 1;
    unsigned char AlphaIsBoolean : 1;
    unsigned char HasLandscapeNormalLighting : 1;
    unsigned char TiltToSlope : 1;
    unsigned char HasWindSkew : 1;
    CEngineLocalDetailObjectDef* operator=(CEngineLocalDetailObjectDef& other);
};

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, _nativeBaseTail) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLocalDetailObjectDef, Mesh) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineLocalDetailObjectDef, WindSkewSpeedFactor) == 0x80);
FABLE_STATIC_ASSERT(sizeof(CEngineLocalDetailObjectDef) == 0x88);
#endif

CEngineLocalDetailObjectDef* CEngineLocalDetailObjectDef::operator=(CEngineLocalDetailObjectDef& other)
{
    CParentDefClassBase::operator=(other);
    Mesh = other.Mesh;
    ShadowMesh = other.ShadowMesh;
    Probability = other.Probability;
    ThemeBlendThreshold = other.ThemeBlendThreshold;
    Scale = other.Scale;
    ScaleRandomElement = other.ScaleRandomElement;
    FadeStart = other.FadeStart;
    FadeEnd = other.FadeEnd;
    ZSpriteMesh = other.ZSpriteMesh;
    ZSpriteFadeStart = other.ZSpriteFadeStart;
    ZSpriteFadeEnd = other.ZSpriteFadeEnd;
    SlopeFadeStart = other.SlopeFadeStart;
    SlopeFadeEnd = other.SlopeFadeEnd;
    AlphaRef = other.AlphaRef;
    AlphaMipBias = other.AlphaMipBias;
    WindSkewConstantFactor = other.WindSkewConstantFactor;
    WindSkewRandomFactor = other.WindSkewRandomFactor;
    WindSkewSpeedFactor = other.WindSkewSpeedFactor;
    CastShadows = other.CastShadows;
    ReceiveShadows = other.ReceiveShadows;
    IsRepeatedMesh = other.IsRepeatedMesh;
    IsZSprite = other.IsZSprite;
    AlphaIsBoolean = other.AlphaIsBoolean;
    HasLandscapeNormalLighting = other.HasLandscapeNormalLighting;
    TiltToSlope = other.TiltToSlope;
    HasWindSkew = other.HasWindSkew;
    return this;
}