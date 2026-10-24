// CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade(float distance) -- the primitive's alpha faded out linearly across its fade range: 0 when the alpha is 0 or the distance is beyond the draw distance, the full alpha before the fade start, else alpha * (255 - 255 * t) >> 8
// Grade: DIFFER(153v172) -- retail_asm candidate: retail converts the fade fraction with a bare `fld [float] ; mov edx,[int*] ; fistp dword ptr [edx]` (an inline-asm float-to-int helper taking an int* and a float, the int homed in the dead distance argument slot); the C++ `(int)` cast calls _ftol2 (everything before the conversion matches; frame offsets move by the helper's two locals). Next: land as grade retail_asm with a minimal inline-asm helper and a `// Retail-asm:` line (owner exception 2026-10-24), not in this lane.
// Leaf: ?GetAlphaIncludingDistanceFade@CEngineInternalPrimitiveBase@@QAEEM
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA7B30, 172 bytes, thiscall, `ret 4`, unsigned char in al.
// Identity: Ego_r.pdb ?GetAlphaIncludingDistanceFade@CEngineInternalPrimitiveBase@@QAEEM@Z (the only uchar(float) member
//   of the class; non-virtual). The body reads Alpha (+0x27), FadeDistance (+0x14) and the flag byte +0x34 bit 2
//   (OverrideFadeDistance, as the landed CEngineInternalPrimitiveMeshBase::GetFadeDistance 0x00BEB5C0 reads it), and calls
//   vtable slot 13 (+0x34) = GetFadeDistance (slot list as the landed 0x00B92660 models it). Catalog
//   "CFrustum_ComputeAlphaBlend" is wrong.
// Layout: engine/CEngineInternalPrimitiveBase.h models the vptr as a data member and keeps the bitfield byte opaque, so
//   the virtual GetFadeDistance call needs the polymorphic local model below (same offsets).
// Shape: one shared `xor al,al` return for Alpha == 0 and distance > MaxDrawDistance (`test ah,41h ; jne`); `<` begin is
//   `test ah,5 ; jp`; the fade pair is one local copied from either the virtual call's hidden-pointer result or the member
//   (CEngineFadeDistance needs a user constructor for the hidden-pointer return); 255.0f is the literal at 0x01230014.

class CEngineFadeDistance {                        // Ego_r.pdb, 8 bytes
public:
    float FadeBeginDistance;                       // +0x00
    float MaxDrawDistance;                         // +0x04
    CEngineFadeDistance(float begin, float maxDraw) : FadeBeginDistance(begin), MaxDrawDistance(maxDraw) { }
};

class CEngineInternalPrimitiveBase {               // retail layout of engine/CEngineInternalPrimitiveBase.h, polymorphic
public:
    virtual ~CEngineInternalPrimitiveBase();       // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual CEngineFadeDistance GetFadeDistance() const;   // slot 13 (+0x34)

    unsigned long       IVRefCount;                // +0x04 (CIVCountedPointeeBase)
    long                Type;                      // +0x08 (CEnginePersistentPrimitiveBase)
    unsigned long       EnginePrimitiveFlags;      // +0x0c
    float               AlphaMipBias;              // +0x10
    CEngineFadeDistance FadeDistance;              // +0x14
    unsigned long       OcclusionLayerBitMask;     // +0x1c
    void*               GridCell;                  // +0x20
    signed char         GridLevel;                 // +0x24
    unsigned char       GridX, GridY;              // +0x25
    unsigned char       Alpha;                     // +0x27
    unsigned char       AlphaRef;                  // +0x28
    unsigned char       PreloadStat;               // +0x29
    int                 InterpolationSet;          // +0x2c
    unsigned long       RenderLayerMask;           // +0x30
    unsigned char       Enable : 1;                // +0x34
    unsigned char       NonPersistent : 1;
    unsigned char       OverrideFadeDistance : 1;
    unsigned char       PostRenderEnable : 1;

    unsigned char GetAlphaIncludingDistanceFade(float distance);
};

unsigned char CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade(float distance)
{
    if (Alpha == 0)
        return 0;
    CEngineFadeDistance fade = OverrideFadeDistance ? GetFadeDistance() : FadeDistance;
    if (distance > fade.MaxDrawDistance)
        return 0;
    if (distance < fade.FadeBeginDistance)
        return Alpha;
    int fadeAmount = (int)((distance - fade.FadeBeginDistance) * 255.0f / (fade.MaxDrawDistance - fade.FadeBeginDistance));
    return (unsigned char)(((255 - fadeAmount) * Alpha) >> 8);
}
