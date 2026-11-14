// NParticleEngine::CPSCSingleSprite::PreparePrimitives(const float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle) -- vtable slot 13: set the sprite child primitives (one, or NoCrossedSprites rotated copies) and the trail child primitive for this frame
// Grade: RELOCATION_MATCH
// Flags: /d2inlT86
// Inline-budget: /d2inlT86 -- CParticleUpdateParameters::GetUpdateParam (and its Find) is expanded in retail at 0x00B08B50..0x00B08B9B; at the
//   default budget VC7.1 calls it; T86 is the smallest that expands it (T85 calls it); nothing else changes (twin 0x00AFCC30 carries /d2inlT1537 /d2inlS15).
// Leaf: ?PreparePrimitives@CPSCSingleSprite@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B087C0, 2967 bytes, thiscall, ebp frame, `ret 14h`; vtable 0x0129FB84 slot 13 (FableWin / Ego_r name sets).
// Callees: SetTrailPositionsFromSpline 0x00B08250 (protected, landed), GetTickRate 0x00AEA9F0, _CIfmod, CPSCSplineBase::GetPointAtIndex 0x00B219A0,
//   CVectorMap::LowerBound 0x00AF2C50, CParticleUpdateParamCallback slot 1, GetSystemTransform 0x00AF2250, GetGraphicBank 0x00AEAAA0, CGraphicDataBank
//   slot GetNoFramesInGraphic, SmoothFadeValue 0x00B05E10 (protected), ScaleParticleDimensions 0x00AF8F00 / 0x00B05B80 / 0x00AF8F80,
//   GetUnscaledWorldTransform 0x00B05840, CEnginePrimitiveSprite ctor 0x006558CE, CIEngine SetChildPrimitive / ClearChildPrimitive, __ftol2.
// Same TU: the header inlines retail calls as folded COMDATs (the three CParticleSystem::ScaleParticleDimensions overloads, the twin
//   0x00AFCC30's text and the landed 0x00B05B80 text), out of line as in retail; without their bodies the frame packs differently.
// Shape (EVIDENCED by bytes unless marked): TransformPoint with FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS (the same-TU landed
//   SetTrailPositionsFromSpline 0x00B08250 uses it; retail Z row E33, E13, E23); the trail colour is the implicit conversion
//   `CRGBColour trailColour = colour * colour.A;` (FableWin ??BCRGBFloatColour@@QBE?AVCRGBColour@@XZ; R, G, B converted and stored, then A);
//   midColour.A = MidColour.A * (1/255) right after the ctor, then the fade multiply; trail Flags stored before BlendOp (store order).
//   FITTED (ordinary C++ chosen by grading): `CIEngine* engine = info.Engine;` in the single-sprite set arm (retail holds the engine in edi
//   across the constructor call and spills primID to [ebp+4Ch]); `float fadeAlpha = 0.0f;` declared with the other locals at the top and
//   assigned system.GetFadeAlpha() where it is used (retail 0x00B08AFA loads it once there; defined only there, VC7.1 numbers it after the
//   colours and multiplies `fa * alpha`, keeping the start / end alpha values live: `fld st(0) ; fmul st(3)`; defined first it emits retail's
//   `fxch st(2) ; fmul st(2)`).
// Retail-asm: 0x00B08EEB `fistp dword ptr` (GFFloatToLongFloor, shared engine/GFFloatToLongFloor.h) and 0x00B08E08 `fistp dword ptr`
//   (GFFloatToLongNear inside GetRGBColourClamped): existing shared portable-twin helpers only.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETUPDATEPARAM
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
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
#include "engine/CCardinalSpline.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CParticleSystem.h"
#include "engine/CPSCBase.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index)const{return index<GraphicList.size()?&GraphicList[index]:0;}
inline unsigned long CGraphicInfoFrame::GetWidth()const{return GraphicInfo->Header.FrameWidth;}
inline unsigned long CGraphicInfoFrame::GetHeight()const{return GraphicInfo->Header.FrameHeight;}
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
long FABLE_FASTCALL GetTickRate();                      // 0x00AEA9F0
const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank();   // 0x00AEAAA0
class CPSCSplineBase : public CPSCBase {
public:
    class CSplineRandomOffsetData;
    class CSplinePosParamData;
    float SplineAnimSpeed;                              // +0x0c
    CCardinalSpline Spline;                             // +0x10
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData> SplinePosParamData;           // +0x2c
    long GetIndexCount() { return Spline.GetControlPointCount(); }
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
    virtual void PreparePrimitives(const float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
protected:
    void SetTrailPositionsFromSpline(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                     float width, float time);   // 0x00B08250
    float SmoothFadeValue(float value) const;           // 0x00B05E10
    long GetPrimID(long index) const { return PrimIDList.size() > (unsigned long)index ? PrimIDList[index] : -1; }
};
#define MAX(a, b) ((a) > (b) ? (a) : (b))
// Header inlines retail calls as folded COMDATs (0x00AF8F80 / 0x00AF8F00 / 0x00B05B80): the landed texts, out of line.
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CParticleSystem::ScaleParticleDimensions(float& dim)const{
    if(ScaleParticles){float scale=((Scale.X>Scale.Y?Scale.X:Scale.Y)>Scale.Z?(Scale.X>Scale.Y?Scale.X:Scale.Y):Scale.Z);dim*=scale;}
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CParticleSystem::ScaleParticleDimensions(C2DVector& dim)const{
    if(ScaleParticles){float scale=((Scale.X>Scale.Y?Scale.X:Scale.Y)>Scale.Z?(Scale.X>Scale.Y?Scale.X:Scale.Y):Scale.Z);dim.X*=scale;dim.Y*=scale;}
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CParticleSystem::ScaleParticleDimensions(unsigned long& dimension) const
{
    if (ScaleParticles)
        dimension *= (long)MAX(MAX(Scale.X, Scale.Y), Scale.Z);
}
void CPSCSingleSprite::PreparePrimitives(const float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                         const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle)
{
    if (!SpriteBankIndex && !TrailBankIndex) return;
    if (!SplineRandomOffsetData.IsNull())
        SetTrailPositionsFromSpline(system, params, TrailWidth, time);
    float fadeAlpha = 0.0f;
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
    midColour.A = MidColour.A * (1.0f / 255.0f);
    fadeAlpha = system.GetFadeAlpha();
    startColour.A *= fadeAlpha;
    endColour.A *= fadeAlpha;
    midColour.A *= fadeAlpha;
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
        const CGraphicInfo* graphic = bank.PeekGraphic(SpriteBankIndex);
        C2DVector size;
        float startSize = StartRenderSize;
        size.X = ((EndRenderSize - startSize) * animFraction + startSize) * sizeScale;
        const CGraphicInfoFrame* graphicFrame = graphic->PeekInfoFrame(frame);
        size.Y = (float)graphicFrame->GetHeight() / graphicFrame->GetWidth() * size.X;
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
            if (size.X > 0.0f && spriteColour.A) {
                CIEngine* engine = info.Engine;
                engine->SetChildPrimitive(handle, primID,
                    CEnginePrimitiveSprite(position, SpriteBankIndex, frame, flags, angle, size.X, size.Y,
                                           (EEnginePrimitiveBlendMode)blendMode, spriteColour.A, spriteColour.R, spriteColour.G,
                                           spriteColour.B, 0.0f, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)blendOp), 0);
            } else
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
        CRGBColour trailColour = colour * colour.A;
        TrailUpdatePrimitive.BlendMode = (EEnginePrimitiveBlendMode)blendMode;
        TrailUpdatePrimitive.HeadColour = trailColour;
        TrailUpdatePrimitive.MidColour = trailColour;
        TrailUpdatePrimitive.TailColour = trailColour;
        TrailUpdatePrimitive.Flags = flags;
        TrailUpdatePrimitive.BlendOp = (EEnginePrimitiveBlendOp)blendOp;
        system.GetUnscaledWorldTransform(TrailUpdatePrimitive.WorldTransform);
        TrailUpdatePrimitive.UpdateExisting = false;
        info.Engine->SetChildPrimitive(handle, GetPrimID(primIndex), TrailUpdatePrimitive, 0);
    }
}
}
