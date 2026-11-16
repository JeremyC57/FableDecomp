// CEngineInternalPrimitiveMeshBase::GetFadeDistance() const -- the primitive's fade distances: with OverrideFadeDistance set they are scaled by the engine fade scale and the draw distance is clamped into [min, max] (fade start scaled by the same ratio); otherwise the stored pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEB5C0, length 159 bytes (`ret 4`, CEngineFadeDistance by hidden pointer).
// Globals: the engine settings pointer @ 0x01436E78 (fade-distance scale +0x7C, min / max draw distance +0x80 / +0x84; record named
// by use). Layout: FadeDistance (PDB CEngineFadeDistance: FadeBeginDistance, MaxDrawDistance) at +0x14, the flag byte +0x34 with
// OverrideFadeDistance = bit 2 (PDB CEngineInternalPrimitiveBase; the header keeps the bitfield opaque, so modelled locally).
// Shape: an 8-byte result with a user constructor returns through the hidden pointer; the result is ONE local written field by field
// (a constructed temporary assigned to it adds a frame copy); `fcomp ; test ah,5 ; jp` is the `<` clamp, `test ah,41h ; jne` the `>`
// one; the max distance is a float local (frame word, copied as an integer into the result).
// NF46 (round 7): the 0x01436E78 global is NEngineInternal::GPPrimitiveRendererSharedData (Ego_r.pdb / FableWin.pdb
// ?GPPrimitiveRendererSharedData@NEngineInternal@@3PAVCEnginePrimitiveRendererSharedData@@A); its fields are the Ego_r.pdb members of
// engine/CEnginePrimitiveRendererSharedData.h (DrawDistanceFactor +0x7C, DrawDistanceMaximum +0x84, DrawDistanceMinimum +0x80); was a local SFadeSettings / g_pFadeSettings stand-in.
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#include "engine/CEnginePrimitiveRendererSharedData.h"
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78

class CEngineFadeDistance { public:
    float FadeBeginDistance;
    float MaxDrawDistance;
    CEngineFadeDistance() { }
    CEngineFadeDistance(float begin, float maxDraw) : FadeBeginDistance(begin), MaxDrawDistance(maxDraw) { }
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
    CEngineFadeDistance GetFadeDistance() const;
};

CEngineFadeDistance CEngineInternalPrimitiveMeshBase::GetFadeDistance() const
{
    CEngineFadeDistance result;
    if (OverrideFadeDistance) {
        float scale = NEngineInternal::GPPrimitiveRendererSharedData->DrawDistanceFactor;
        result.FadeBeginDistance = scale * FadeDistance.FadeBeginDistance;
        result.MaxDrawDistance = scale * FadeDistance.MaxDrawDistance;
        float minDraw = NEngineInternal::GPPrimitiveRendererSharedData->DrawDistanceMinimum;
        float maxDraw = NEngineInternal::GPPrimitiveRendererSharedData->DrawDistanceMaximum;
        if (result.MaxDrawDistance < minDraw) {
            result.FadeBeginDistance *= minDraw / result.MaxDrawDistance;
            result.MaxDrawDistance = minDraw;
        } else if (result.MaxDrawDistance > maxDraw) {
            result.FadeBeginDistance *= maxDraw / result.MaxDrawDistance;
            result.MaxDrawDistance = maxDraw;
        }
    } else {
        result = FadeDistance;
    }
    return result;
}