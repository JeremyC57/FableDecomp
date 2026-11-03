// NParticleEngine::CPSCSingleSprite::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle) -- vtable slot 13: set the sprite child primitives (one, or NoCrossedSprites rotated copies) and the trail child primitive for this frame
// Grade: DIFFER(2951v2967)
// Leaf: ?PreparePrimitives@CPSCSingleSprite@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B087C0, 2967 bytes, thiscall, ebp frame, `ret 14h`.
// Identity: FableWin.pdb ?PreparePrimitives@CPSCSingleSprite@NParticleEngine@@...; slot 13 of the CPSCSingleSprite vtable 0x0129FB84.
// Callees: SetTrailPositionsFromSpline 0x00B08250 (FableWin IAE), GetTickRate 0x00AEA9F0, _CIfmod, CPSCSplineBase::GetPointAtIndex
//   0x00B219A0, CVectorMap::LowerBound 0x00AF2C50 (same-TU body), CParticleUpdateParamCallback slot 1 GetUpdateParam,
//   GetSystemTransform 0x00AF2250, NParticleEngine::GetGraphicBank 0x00AEAAA0 (YIABVCGraphicDataBank@@XZ), CGraphicDataBank slot 20
//   GetNoFramesInGraphic 0x00A013E0, CPSCSingleSprite::SmoothFadeValue 0x00B05E10 (IBEMM), ScaleParticleDimensions C2DVector& 0x00AF8F00 /
//   unsigned long& 0x00B05B80 / float& 0x00AF8F80, GetUnscaledWorldTransform 0x00B05840, CEnginePrimitiveSprite ctor 0x006558CE,
//   CIEngine slot 28 SetChildPrimitive(handle, id, prim, flags) / slot 30 ClearChildPrimitive, __ftol2.
// Residue (b8, 10 graded variants b1..b10, LOG.tsv): frame 0xE0 vs 0xDC (the [ebp-10h] colour temp sits 4 bytes lower and
//   sizeScale above it); the mid-colour alpha * FadeAlpha ordering; the size.X tail scheduled before the frame-info loads; the trail
//   GetRGBColour stores each byte as it converts (retail holds R/G/B, then converts A); trail blend mode/op register choice; the
//   Z row of the inline TransformPoint (retail E33*zz, E13*xx, E23*yy).
// Retail-asm: 0x00B08EEB `fistp dword ptr [ebp+54h]` (GFFloatToLongFloor, shared engine/GFFloatToLongFloor.h) and 0x00B08E08
//   `fistp dword ptr [ebp+50h]` (GFFloatToLongNear inside GetRGBColourClamped); VC7.1 emits no bare dword fistp from C++.
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include <math.h>
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CParticleSystem.h"
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
class CGraphicInfo;
class CGraphicInfoFrame {                               // Ego_r.pdb, 16 bytes
public:
    unsigned char _pad00[0xc];
    CGraphicInfo* GraphicInfo;                          // +0x0c
};
class CGraphicInfo {                                    // Ego_r.pdb, 0x2c bytes
public:
    unsigned short Width, Height, Depth;                // Header +0x00
    unsigned short FrameWidth;                          // +0x06
    unsigned short FrameHeight;                         // +0x08
    unsigned short FrameCount;                          // +0x0a
    unsigned char _pad0c[0x28 - 0xc];
    CGraphicInfoFrame* Frames;                          // +0x28
};
class CGraphicDataBank {                                // retail vtable 0x0129C8CC
public:
    virtual void _vslot00(); virtual void _vslot01(); virtual void _vslot02(); virtual void _vslot03(); virtual void _vslot04();
    virtual void _vslot05(); virtual void _vslot06(); virtual void _vslot07(); virtual void _vslot08(); virtual void _vslot09();
    virtual void _vslot10(); virtual void _vslot11(); virtual void _vslot12(); virtual void _vslot13(); virtual void _vslot14();
    virtual void _vslot15(); virtual void _vslot16(); virtual void _vslot17(); virtual void _vslot18(); virtual void _vslot19();
    virtual unsigned long GetNoFramesInGraphic(unsigned long index) const;   // slot 20, 0x00A013E0
    const CGraphicInfo* GetGraphicInfo(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }
    unsigned char _pad04[0x1e0 - 4];
    CArray<CGraphicInfo> GraphicList;                   // +0x1e0
};
class CEnginePrimitiveSprite : public CEnginePrimitive {
public:
    CEnginePrimitiveSprite(const C3DVector& pos, unsigned long bank_index, unsigned long frame, unsigned short flags, float angle,
                           float size_x, float size_y, EEnginePrimitiveBlendMode blend_mode, unsigned char a, unsigned char r,
                           unsigned char g, unsigned char b, float fade_start, EEngineInterpolationSet interpolation_set,
                           EEnginePrimitiveBlendOp blend_op);   // 0x006558CE
    unsigned char _body[0x48 - 0xc];
};
class CEnginePrimitiveSpriteTrail : public CEnginePrimitive {
public:
    void* Trail;                                        // +0x0c STLport list
    bool UpdateExisting;                                // +0x10
    unsigned long MaxLength;                            // +0x14
    unsigned long GraphicIndex;                         // +0x18
    unsigned long GraphicFrame;                         // +0x1c
    float SkipPoints;                                   // +0x20
    float Width;                                        // +0x24
    CRGBColour HeadColour;                              // +0x28
    CRGBColour MidColour;                               // +0x2c
    CRGBColour TailColour;                              // +0x30
    float FadeStart;                                    // +0x34
    C3DVector OffsetVector;                             // +0x38
    unsigned short Flags;                               // +0x44
    EEnginePrimitiveBlendMode BlendMode;                // +0x48
    EEnginePrimitiveBlendOp BlendOp;                    // +0x4c
    CMatrix3x4 WorldTransform;                          // +0x50
};
namespace NParticleEngine {
long __fastcall GetTickRate();                          // 0x00AEA9F0
const CGraphicDataBank& __fastcall GetGraphicBank();    // 0x00AEAAA0
template <class K, class V> class CParticleUpdateParameters;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector& out, V data, float time, bool local, const CParticleUpdateParameters<K, V>& params, unsigned long flags);
};
template <class K, class V> class CParticleUpdateParamDesc {
public:
    virtual void _vslot00();
    CParticleUpdateParamCallback<K, V>* Callback;
    V Data;
};
template <class K> class CParticleUpdateCCompareParamDesc {
public:
    template <class P> bool operator()(const K& key, const P& p) const { return key < p.first; }
};
template <class K, class V, class TCompare> class CVectorMapP {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    CArray<value_type> Data;
    TCompare Compare;
    bool Dirty;
    const_iterator end() const { return Data.end(); }
    const_iterator LowerBound(const K& key) const;      // 0x00AF2C50
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        const_iterator last = end();
        if (it == last || Compare(key, *it)) return last;
        return it;
    }
};
#pragma auto_inline(off)
// Same-TU body of the binary search (out-of-line copy @ 0x00AF2C50, as the landed GetUpdateParamDesc 0x00B15B50 holds it): it only
// reads memory, which lets VC7.1 keep the caller's key in ebx across the call as retail does.
template <class K, class V, class TCompare>
__declspec(noinline) typename CVectorMapP<K, V, TCompare>::const_iterator CVectorMapP<K, V, TCompare>::LowerBound(const K& key) const
{
    const_iterator first = Data.begin();
    int len = Data.end() - first;
    while (len > 0) {
        int half = len / 2;
        const_iterator middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}
#pragma auto_inline(on)
template <class K, class V> class CParticleUpdateParameters {
public:
    virtual void _vslot00();
    unsigned char VectorParams[0x10];                   // +0x04
    CVectorMapP<K, CParticleUpdateParamDesc<K, V>, CParticleUpdateCCompareParamDesc<K> > UpdateParams;   // +0x14
    __forceinline bool GetUpdateParam(C3DVector& out, const K& key, float time, bool local, unsigned long flags) const
    {
        if (key) {
            typename CVectorMapP<K, CParticleUpdateParamDesc<K, V>, CParticleUpdateCCompareParamDesc<K> >::const_iterator it = UpdateParams.Find(key);
            if (it != UpdateParams.end())
                return it->second.Callback->GetUpdateParam(out, it->second.Data, time, local, *this, flags);
        }
        return false;
    }
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) = 0;
};
class CPSCSplineBase : public CPSCBase {
public:
    class CSplineRandomOffsetData;
    class CSplinePosParamData;
    float SplineAnimSpeed;                              // +0x0c
    CArray<C3DVector> SplinePoints;                     // +0x10 CCardinalSpline (points first)
    unsigned char _spline1c[8];
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData> SplinePosParamData;           // +0x2c
    long GetIndexCount() { return SplinePoints.size(); }
    void GetPointAtIndex(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& point, long index, float t);   // 0x00B219A0
};
class CPSCSingleSprite : public CPSCSplineBase {
public:
    long SpriteBankIndex;                               // +0x34
    CRGBColour StartColour;                             // +0x38
    CRGBColour MidColour;                               // +0x3c
    CRGBColour EndColour;                               // +0x40
    float AnimationTimeSecs;                            // +0x44
    float StartRenderSize;                              // +0x48
    float EndRenderSize;                                // +0x4c
    long SpriteAlignment;                               // +0x50
    long NoCrossedSprites;                              // +0x54
    float InitialAngle;                                 // +0x58
    long FadeInEndInteger;                              // +0x5c
    long FadeOutBeginInteger;                           // +0x60
    float AlphaFadeMinimum;                             // +0x64
    float SizeFadeMinimum;                              // +0x68
    unsigned long PositionParam;                        // +0x6c
    long BlendMode;                                     // +0x70
    long BlendOp;                                       // +0x74
    long TrailBlendMode;                                // +0x78
    long TrailBlendOp;                                  // +0x7c
    long TrailBankIndex;                                // +0x80
    float TrailWidth;                                   // +0x84
    long MaxTrailLength;                                // +0x88
    long StayWithEmitterIntFactor;                      // +0x8c
    bool UseStartColour;                                // +0x90
    bool UseMidColour;                                  // +0x91
    bool UseEndColour;                                  // +0x92
    bool SelfIlluminating : 1;                          // +0x93 bit 0
    bool AlphaFadeEnable : 1;
    bool RotateAroundCentre : 1;
    bool FaceMe2D : 1;
    bool FaceMe3D : 1;
    bool CrossedSprites : 1;
    bool SizeFadeEnable : 1;
    bool ForceAnimationTime : 1;
    bool StayWithEmitter : 1;                           // +0x94 bit 0
    bool UsePosition;                                   // +0x95
    bool UseSplinePoints;                               // +0x96
    bool EnteringTimelineEnd;                           // +0x97
    unsigned long LastUpdateFrame;                      // +0x98
    long Lifetimer;                                     // +0x9c
    C3DVector Offset;                                   // +0xa0
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;   // +0xac
    CArray<long> PrimIDList;                            // +0x12c
    virtual void PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
    void SetTrailPositionsFromSpline(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                     float width, float time);   // 0x00B08250
    float SmoothFadeValue(float value) const;           // 0x00B05E10
    long GetPrimID(long index) const { return PrimIDList.size() > (unsigned long)index ? PrimIDList[index] : -1; }
};
void CPSCSingleSprite::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                         const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle)
{
    if (!SpriteBankIndex && !TrailBankIndex) return;
    if (!SplineRandomOffsetData.IsNull())
        SetTrailPositionsFromSpline(system, params, TrailWidth, time);
    float tickRate = (float)GetTickRate();
    float fadeInEnd = FadeInEndInteger * 0.001f;
    float fadeOutBegin = FadeOutBeginInteger * 0.001f;
    float lifeTime = Lifetimer + time;
    float lifeFraction = system.GetNormalisedTime();
    if (lifeFraction < 0.0f) lifeFraction = 0.0f;
    else if (lifeFraction > 1.0f) lifeFraction = 1.0f;
    float animFraction;
    if (!ForceAnimationTime) {
        animFraction = lifeFraction;
        if (animFraction < 0.0f) animFraction = 0.0f;
        else {
            float limit = 1.0f - DELTA_NOTIONAL_ZERO;
            if (animFraction > limit) animFraction = limit;
        }
    } else
        animFraction = (float)fmod((double)(lifeTime * (1.0f / (tickRate * AnimationTimeSecs))), 1.0);
    CRGBFloatColour startColour(UseStartColour ? StartColour : UseMidColour ? MidColour
                                : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255));
    startColour.A = StartColour.A * (1.0f / 255.0f);
    CRGBFloatColour endColour(UseEndColour ? EndColour : UseMidColour ? MidColour
                              : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255));
    endColour.A = EndColour.A * (1.0f / 255.0f);
    CRGBFloatColour midColour(UseMidColour ? MidColour : (startColour * 0.5f + endColour * 0.5f).GetRGBColour());
    float fadeAlpha = system.GetFadeAlpha();
    startColour.A *= fadeAlpha;
    endColour.A *= fadeAlpha;
    midColour.A = MidColour.A * (1.0f / 255.0f) * fadeAlpha;
    unsigned short flags = (unsigned short)SpriteAlignment;
    if (!SelfIlluminating) flags |= 0x80;
    if (CrossedSprites) flags |= 0x40;
    else {
        if (FaceMe3D) flags |= 0x01;
        if (RotateAroundCentre) flags |= 0x20;
    }
    C3DVector position;
    if (!UsePosition || !params.GetUpdateParam(position, PositionParam, time, false, 1))
        GetPointAtIndex(params, position, 0, 0.0f);
    {
        CMatrix3x4 transform;
        system.GetSystemTransform(transform);
        transform.TransformPoint(position);
    }
    const CGraphicDataBank& bank = GetGraphicBank();
    float angle = InitialAngle;
    CRGBFloatColour colour;
    if (lifeFraction < 0.5f)
        colour = startColour.Interpolate(midColour, lifeFraction * 2.0f);
    else
        colour = midColour.Interpolate(endColour, (lifeFraction - 0.5f) * 2.0f);
    float alpha = system.GetGlobalAlpha();
    float sizeScale = 1.0f;
    if (lifeFraction < fadeInEnd)
        alpha *= SmoothFadeValue(lifeFraction / fadeInEnd);
    if (lifeFraction > fadeOutBegin)
        alpha *= SmoothFadeValue((1.0f - lifeFraction) / (1.0f - fadeOutBegin));
    if (alpha < 1.0f) {
        if (AlphaFadeEnable)
            colour.A *= (1.0f - AlphaFadeMinimum) * alpha + AlphaFadeMinimum;
        if (SizeFadeEnable)
            sizeScale = (1.0f - SizeFadeMinimum) * alpha + SizeFadeMinimum;
    }
    CRGBColour spriteColour = colour.GetRGBColourClamped();
    long primIndex = 0;
    if (SpriteBankIndex) {
        long blendMode = BlendMode;
        long blendOp = BlendOp;
        long noFrames = bank.GetNoFramesInGraphic(SpriteBankIndex);
        long frame = GFFloatToLongFloor(noFrames * animFraction);
        if (frame < 0) frame = 0;
        else if (frame > noFrames - 1) frame = noFrames - 1;
        const CGraphicInfo* graphic = bank.GetGraphicInfo(SpriteBankIndex);
        C2DVector size;
        float startSize = StartRenderSize;
        size.X = ((EndRenderSize - startSize) * animFraction + startSize) * sizeScale;
        const CGraphicInfo* frameGraphic = graphic->Frames[frame].GraphicInfo;
        size.Y = (float)frameGraphic->FrameHeight / frameGraphic->FrameWidth * size.X;
        system.ScaleParticleDimensions(size);
        if (CrossedSprites) {
            float angleScale = 0.5f;
            if (SpriteAlignment == 2 || SpriteAlignment == 4) angleScale = 1.0f;
            for (long i = 0; i < NoCrossedSprites; ++i) {
                float spriteAngle = (i + 0.5f) * angleScale / NoCrossedSprites + angle;
                info.Engine->SetChildPrimitive(handle, GetPrimID(i),
                    CEnginePrimitiveSprite(position, SpriteBankIndex, frame, flags, spriteAngle, size.X, size.Y,
                                           (EEnginePrimitiveBlendMode)blendMode, spriteColour.A, spriteColour.R, spriteColour.G,
                                           spriteColour.B, 0.0f, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)blendOp), 0);
            }
            primIndex = NoCrossedSprites;
        } else {
            long primID = GetPrimID(0);
            if (size.X > 0.0f && spriteColour.A)
                info.Engine->SetChildPrimitive(handle, primID,
                    CEnginePrimitiveSprite(position, SpriteBankIndex, frame, flags, angle, size.X, size.Y,
                                           (EEnginePrimitiveBlendMode)blendMode, spriteColour.A, spriteColour.R, spriteColour.G,
                                           spriteColour.B, 0.0f, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)blendOp), 0);
            else
                info.Engine->ClearChildPrimitive(handle, primID);
            primIndex = 1;
        }
    }
    if (TrailBankIndex) {
        long blendMode = TrailBlendMode;
        long blendOp = TrailBlendOp;
        long noFrames = bank.GetNoFramesInGraphic(TrailBankIndex);
        long frame = GFFloatToLongFloor(noFrames * animFraction);
        if (frame < 0) frame = 0;
        else if (frame > noFrames - 1) frame = noFrames - 1;
        TrailUpdatePrimitive.GraphicIndex = TrailBankIndex;
        TrailUpdatePrimitive.GraphicFrame = frame;
        float skip;
        if (UsePosition) {
            skip = 1.0f - time;
            float limit = 1.0f - DELTA_NOTIONAL_ZERO;
            if (!(skip < limit)) skip = limit;
        } else
            skip = 0.0f;
        TrailUpdatePrimitive.SkipPoints = skip;
        if (!UsePosition && UseSplinePoints)
            TrailUpdatePrimitive.MaxLength = GetIndexCount();
        else {
            TrailUpdatePrimitive.MaxLength = MaxTrailLength;
            system.ScaleParticleDimensions(TrailUpdatePrimitive.MaxLength);
        }
        TrailUpdatePrimitive.Width = TrailWidth;
        system.ScaleParticleDimensions(TrailUpdatePrimitive.Width);
        CRGBColour trailColour = (colour * colour.A).GetRGBColour();
        TrailUpdatePrimitive.BlendMode = (EEnginePrimitiveBlendMode)blendMode;
        TrailUpdatePrimitive.HeadColour = trailColour;
        TrailUpdatePrimitive.MidColour = trailColour;
        TrailUpdatePrimitive.TailColour = trailColour;
        TrailUpdatePrimitive.BlendOp = (EEnginePrimitiveBlendOp)blendOp;
        TrailUpdatePrimitive.Flags = flags;
        system.GetUnscaledWorldTransform(TrailUpdatePrimitive.WorldTransform);
        TrailUpdatePrimitive.UpdateExisting = false;
        info.Engine->SetChildPrimitive(handle, GetPrimID(primIndex), TrailUpdatePrimitive, 0);
    }
}
}
