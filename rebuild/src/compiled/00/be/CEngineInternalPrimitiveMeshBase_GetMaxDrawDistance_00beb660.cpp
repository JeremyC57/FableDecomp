// CEngineInternalPrimitiveMeshBase::GetMaxDrawDistance() const -- the primitive's maximum draw distance: with OverrideFadeDistance set it is scaled by the engine fade scale and clamped into the renderer's [min, max] draw range, otherwise the stored FadeDistance.MaxDrawDistance
// Leaf: ?GetMaxDrawDistance@CEngineInternalPrimitiveMeshBase@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEB660, 83 bytes (+ int3 padding), `ret` (float in st0). Ego_r.pdb:
// ?GetMaxDrawDistance@CEngineInternalPrimitiveMeshBase@@QBEMXZ (the manifest label "CEnginePrimitive_ComputeFadeDistance" is
// descriptive). Callers: CEngineInternalPrimitiveStaticMesh::IsInsideFrustum 0x00B9DC60 and the animated-mesh one 0x00BCF040.
// Layout: as the landed GetFadeDistance 0x00BEB5C0 of the same class: FadeDistance (CEngineFadeDistance: FadeBeginDistance,
// MaxDrawDistance) at +0x14, OverrideFadeDistance = bit 2 of the flag byte +0x34; the renderer settings behind the pointer at
// 0x01436E78 (fade scale +0x7c, min / max draw distance +0x80 / +0x84; names descriptive). The engine header is opaque, so the
// class is modelled locally like the landed sibling.
// Shape: the stored distance is read into a float local BEFORE the flag test (`fld [ecx+18h]` above the `test al,4`, shared
// by both returns, and the member then multiplies first: `member * scale` written inline loads the global first, and
// `distance *= scale` keeps the product on the FPU stack); the scaled distance is a separate float frame local,
// the max limit a float local copied as an integer (`mov eax,[eax+84h]`), the min limit stays on the FPU stack and is the
// value returned by the `<` exit (`test ah,5 ; jnp`), the `>` exit is `test ah,41h ; jne`. Grades: 4.
struct SFadeSettings {                             // engine settings record reached through the pointer at 0x01436E78
    unsigned char _pad[0x7c];
    float         FadeDistanceScale;               // +0x7c
    float         MinDrawDistance;                 // +0x80
    float         MaxDrawDistance;                 // +0x84
};
extern SFadeSettings* g_pFadeSettings;             // 0x01436E78

struct CEngineFadeDistance {
    float FadeBeginDistance;
    float MaxDrawDistance;
    CEngineFadeDistance() { }
};

class CEngineInternalPrimitiveMeshBase {
public:
    virtual void v000();
    unsigned char       _pad04[0x10];
    CEngineFadeDistance FadeDistance;              // +0x14
    unsigned char       _pad1c[0x18];
    unsigned char       Enable : 1;                // +0x34
    unsigned char       NonPersistent : 1;
    unsigned char       OverrideFadeDistance : 1;
    unsigned char       PostRenderEnable : 1;
    float GetMaxDrawDistance() const;
};

float CEngineInternalPrimitiveMeshBase::GetMaxDrawDistance() const
{
    float distance = FadeDistance.MaxDrawDistance;
    if (OverrideFadeDistance) {
        float maxDraw = distance * g_pFadeSettings->FadeDistanceScale;
        float minLimit = g_pFadeSettings->MinDrawDistance;
        float maxLimit = g_pFadeSettings->MaxDrawDistance;
        if (maxDraw < minLimit)
            return minLimit;
        if (maxDraw > maxLimit)
            return maxLimit;
        return maxDraw;
    }
    return distance;
}