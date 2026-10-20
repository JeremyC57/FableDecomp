// CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask) -- base primitive constructor: ref count 1, type, all-layer occlusion mask, empty grid position, opaque alpha, enabled, unlinked, alpha-ref from the render settings
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the four zeroed/-1 words in the initialiser list (OcclusionLayerBitMask's -1 lands before the vtable store) and the bit-field writes spread: Enable=1 first (the byte load), the three clears after InterpolationSet (the merged read-modify-write store lands after the +0x2c store)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8FF00, length 102 bytes (`ret 8`). Globals: render settings @ 0x01436E20 (DefaultAlphaRef byte +0x64).
// Shape: +4 RefCount = 1 and +8 Type belong to the polymorphic persistent base (inline constructor, vtable store dead); the derived
// vtable 0x012A3D7C (rtti-vtables.tsv: CEngineInternalPrimitiveBase). GridPos (CEngineSceneGridPos) has an inline constructor setting
// Level = -1 only; FadeDistance is left uninitialised; the flag byte +0x34 is `and 0F1h ; or 1` = Enable 1, NonPersistent /
// OverrideFadeDistance / PostRenderEnable 0. Members are the PDB names (engine/CEngineInternalPrimitiveBase.h: opaque bitfield group
// there, so the class is modelled locally with real types).
// Tried: (a) everything in the initialiser list, (c) everything as body assignments, (d) OcclusionLayerBitMask initialiser + body.
struct SRenderSettings {
    unsigned char _pad[0x64];
    unsigned char DefaultAlphaRef;                 // +0x64
};
extern SRenderSettings* g_pRenderSettings;         // 0x01436E20

struct CEngineSceneGridCell;

struct CEngineSceneGridPos {
    signed char   Level;
    unsigned char X;
    unsigned char Y;
    CEngineSceneGridPos() : Level(-1) { }
};

struct CEngineFadeDistance {
    float FadeBeginDistance;
    float MaxDrawDistance;
    CEngineFadeDistance() { }
};

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    long RefCount;                                 // +0x04
    long Type;                                     // +0x08
    CEnginePersistentPrimitiveBase(long type) : RefCount(1), Type(type) { }
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    unsigned long        EnginePrimitiveFlags;     // +0x0c
    float                AlphaMipBias;             // +0x10
    CEngineFadeDistance  FadeDistance;             // +0x14
    unsigned long        OcclusionLayerBitMask;    // +0x1c
    CEngineSceneGridCell* GridCell;                // +0x20
    CEngineSceneGridPos  GridPos;                  // +0x24
    unsigned char        Alpha;                    // +0x27
    unsigned char        AlphaRef;                 // +0x28
    unsigned char        PreloadStat;              // +0x29
    long                 InterpolationSet;         // +0x2c
    unsigned long        RenderLayerMask;          // +0x30
    unsigned char        Enable : 1;               // +0x34
    unsigned char        NonPersistent : 1;
    unsigned char        OverrideFadeDistance : 1;
    unsigned char        PostRenderEnable : 1;
    unsigned short       LastFrameRendered;        // +0x36
    CEngineInternalPrimitiveBase*  NextPrimitive;  // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;   // +0x3c
    CEngineInternalPrimitiveBase*  NextLayerMask;  // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;   // +0x44
    virtual void v001();
    CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask);
};

CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask)
    : CEnginePersistentPrimitiveBase(type), EnginePrimitiveFlags(0), AlphaMipBias(0.0f), OcclusionLayerBitMask(0xffffffff), GridCell(0)
{
    RenderLayerMask = renderLayerMask;
    Enable = 1;
    Alpha = 0xff;
    PreloadStat = 0;
    InterpolationSet = 0;
    NonPersistent = 0;
    OverrideFadeDistance = 0;
    PostRenderEnable = 0;
    LastFrameRendered = 0;
    NextPrimitive = 0;
    RefPrimitive = 0;
    NextLayerMask = 0;
    RefLayerMask = 0;
    AlphaRef = g_pRenderSettings->DefaultAlphaRef;
}