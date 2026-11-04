// NParticleEngine::CPSCRenderSprite::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- vtable slot 19: grow the sphere over every live particle's sprite (half its scaled diagonal) and trail (the segment to its recorded tail); clears both primitive handles when nothing was added
// Grade: DIFFER(2406v2405) -- 10 grades (LOG.tsv a..j, k = this file on the shared headers); same instruction stream and frame (0x1C8, ebp-0x68): residue is register allocation rotated by one register around the frame-graphic aspect (movzx pair), the IsActive(float) lea (eax v ecx), the pos copy and the `this` reload, plus the X/Z term order of the first (pos) TransformPoint (retail X: E11*x, E31*z, E21*y; Z: E33*z, E13*x, E23*y) while the tail transforms keep the X form
// Leaf: ?AddToBoundingSphere@CPSCRenderSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFAE50, 2405 bytes, thiscall, `ret 10h`.
// Identity: FableWin.pdb ?AddToBoundingSphere@CPSCRenderSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z;
//   slot 19 of the CPSCRenderSprite vtable (slot 19 AddToBoundingSphere of CPSCBase 0x00AF2A30 and CPSCLight 0x00B18AF0, `ret 10h`).
// Callees: C3DBoundingSphere::AddToBounds(const C3DVector&, float) 0x00ADDD80, NParticleEngine::GetTickRate 0x00AEA9F0,
//   NParticleEngine::GetGraphicBank 0x00AEAAA0 (+ CGraphicDataBank slot 20 GetNoFramesInGraphic 0x00A013E0), CParticleSystem::
//   GetSystemTransform 0x00AF2250 (x3), ClearParticle 0x00AF5090, ScaleParticleDimensions(C2DVector&) 0x00AF8F00, _CIfmod.
// Shape: packed floats through engine/GFPackFloat.h GFUnpackFloat; CParticle::IsActive(float) / GetRecordedPos (landed body 0x00AFE380)
//   __forceinline (they hold the inline-asm helpers); `C3DVector pos` declared before the loop and assigned (retail copies through a temp).
// Retail-asm: 0x00AFAEC6 `fistp dword ptr [ebp+24h]` (GFFloatToLongNear in the unpack rounding), GFFastFloatToLong in IsActive(float) and
//   GFFloatToLongCeil in GetRecordedPos -- the shared engine/GFFloatToLongNear.h / GFFastFloatToLong.h / GFFloatToLongCeil.h.
// Next idea: build pos / tail through a C3DVector ctor temp (retail's integer copy of the GetInterpolatedPos result), or the
//   two-argument CMatrix3x4::TransformPoint(in, out) for the first transform only (its X/Z order differs from the tail ones).
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CArray.h"
#include "engine/GFPackFloat.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/CBaseClassNonCopyable.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/CEnginePrimitiveHandle.h"
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C
class CGraphicInfo;
class CGraphicInfoFrame {
public:
    unsigned char _pad00[0xc];
    CGraphicInfo* GraphicInfo;                          // +0x0c
};
class CGraphicInfo {
public:
    unsigned short Width, Height, Depth;
    unsigned short FrameWidth;                          // +0x06
    unsigned short FrameHeight;                         // +0x08
    unsigned short FrameCount;                          // +0x0a
    unsigned char _pad0c[0x28 - 0xc];
    CGraphicInfoFrame* Frames;                          // +0x28
};
class CGraphicDataBank {
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
namespace NParticleEngine {
long __fastcall GetTickRate();                          // 0x00AEA9F0
const CGraphicDataBank& __fastcall GetGraphicBank();    // 0x00AEAAA0
#pragma pack(push, 1)
class CParticle {
public:
    virtual ~CParticle();
    unsigned char _pad04[0x29 - 4];
    C3DVector PositionBuffer[4];                        // +0x29
    unsigned char _pad59[0x72 - 0x59];
    short Lifetimer;                                    // +0x72
    unsigned char CreationTime;                         // +0x74
    unsigned char _pad75;
    short MaxLife;                                      // +0x76
    unsigned char FlickerIndex;                         // +0x78
    unsigned char FlickerTimeOffset;                    // +0x79
    bool Active;                                        // +0x7a
    float GetCreationTime() const { return CreationTime * (1.0f / 255.0f); }
    long GetMaxLife() const { return MaxLife; }
    float GetInterpolatedLifetimer(float fraction) const { return Lifetimer - GetCreationTime() + fraction; }
    bool IsActive() const { return Active; }
    __forceinline bool IsActive(float time) const
    {
        if (Lifetimer > 0) return true;
        long creation = GFFastFloatToLong(time * 255.0f);
        return creation * (1.0f / 255.0f) >= GetCreationTime();
    }
    C3DVector GetInterpolatedPos(float t) const { return PositionBuffer[1] + (PositionBuffer[0] - PositionBuffer[1]) * t; }
    __forceinline C3DVector GetRecordedPos(float time, float offset) const
    {
        float pos = time + 1.0f - offset;
        long index = GFFloatToLongCeil(pos);
        if (index < 1)
            index = 1;
        else if (index > 3)
            index = 3;
        float frac = (float)index - pos;
        const C3DVector& newer = PositionBuffer[index - 1];
        const C3DVector& older = PositionBuffer[index];
        return older + (newer - older) * frac;
    }
};
#pragma pack(pop)
template <class K, class V> class CParticleUpdateParameters;
class CParticleSystem {
public:
    typedef std::list<CParticle*>::iterator ParticleIterator;
    unsigned char _pad00[0x1c];
    std::list<CParticle*> Particles;                    // +0x1c
    unsigned char _pad20[0x50 - 0x20];
    bool Initialised : 1;                               // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    float GlobalAlpha;                                  // +0x54
    float FadeAlpha;                                    // +0x58
    float NormalisedTime;                               // +0x5c
    void ClearParticle(ParticleIterator& it);           // 0x00AF5090
    void GetSystemTransform(CMatrix3x4& matrix) const;  // 0x00AF2250
    void ScaleParticleDimensions(C2DVector& size) const;   // 0x00AF8F00
    std::list<CParticle*>& GetParticleList() { return Particles; }
    float GetNormalisedTime() const { return NormalisedTimeEnabled ? NormalisedTime : 0.0f; }
    __forceinline void TransformPosIfLocal(C3DVector& pos) const { if (StayWithEmitter) { CMatrix3x4 transform; GetSystemTransform(transform); transform.TransformPoint(pos); } }
};
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual bool AddToBoundingSphere(C3DBoundingSphere&, float, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&) = 0;
};
class CPSCRenderSprite : public CPSCBase {
public:
    long          SpriteBankIndex;              // +0x0c
    long          TrailBankIndex;               // +0x10
    unsigned long StartColour;                  // +0x14
    unsigned long MidColour;                    // +0x18
    unsigned long EndColour;                    // +0x1c
    unsigned long BlendMode : 3;                // +0x20
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;     // +0x24
    unsigned long TrailLengthInteger : 12;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;   // +0x28
    unsigned long NoCrossedSprites : 5;
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;         // +0x2c
    unsigned long EndRenderSize : 11;
    unsigned long FlickerBias : 8;
    unsigned long AnimationTimeSecs : 14;       // +0x30
    unsigned long SizeFadeMinimum : 7;
    unsigned long TrailWidth : 10;
    unsigned long FlickerSpeed : 12;            // +0x34
    unsigned long UseStartColour : 1;
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned char AlphaFadeEnable : 1;          // +0x38
    unsigned char SizeFadeEnable : 1;
    unsigned char FlickerEnable : 1;
    unsigned char ForceAnimationTime : 1;
    unsigned char _pad39[3];
    CEnginePrimitiveHandle PrimHandle;          // +0x3c
    long          ChildPrimIndex;               // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;     // +0x48
    long          ChildTrailPrimIndex;          // +0x50
    virtual bool AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system,
                                     const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};
bool CPSCRenderSprite::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system,
                                           const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    if (!SpriteBankIndex && !TrailBankIndex)
        return false;
    float startSize = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    float endSize = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    float animTime = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    float trailWidth = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    float trailLength = TrailLengthInteger * 0.001f;
    bool longTrail = trailLength > 2.0f;
    float trailScale = (longTrail ? 2.0f : trailLength) * 0.5f;
    float animScale = 1.0f / ((float)GetTickRate() * animTime);
    float lifeFraction = system.GetNormalisedTime() < 0.0f ? 0.0f : system.GetNormalisedTime() > 1.0f ? 1.0f : system.GetNormalisedTime();
    const CGraphicDataBank& bank = GetGraphicBank();
    if (SpriteBankIndex)
        bank.GetNoFramesInGraphic(SpriteBankIndex);
    float animFraction = lifeFraction;
    if (animFraction < 0.0f) animFraction = 0.0f;
    else {
        float limit = 1.0f - DELTA_NOTIONAL_ZERO;
        if (animFraction > limit) animFraction = limit;
    }
    float aspect = 0.0f;
    if (SpriteBankIndex) {
        const CGraphicInfo* frameGraphic = bank.GetGraphicInfo(SpriteBankIndex)->Frames[0].GraphicInfo;
        aspect = (float)frameGraphic->FrameHeight / frameGraphic->FrameWidth;
    }
    bool added = false;
    C3DVector pos;
    for (CParticleSystem::ParticleIterator it = system.GetParticleList().begin(); it != system.GetParticleList().end(); ++it) {
        CParticle* particle = *it;
        if (particle->IsActive() && particle->IsActive(timeOffset)) {
            pos = particle->GetInterpolatedPos(timeOffset);
            system.TransformPosIfLocal(pos);
            if (TrailBankIndex) {
                C3DVector tail;
                if (longTrail) {
                    tail = particle->GetRecordedPos(2.0f, timeOffset);
                    system.TransformPosIfLocal(tail);
                    tail = pos + (tail - pos) * trailScale;
                } else {
                    tail = particle->GetRecordedPos(trailLength, timeOffset);
                    system.TransformPosIfLocal(tail);
                }
                C3DVector delta;
                delta = tail - pos;
                float length = delta.GetMagnitude();
                if (trailWidth > length) length = trailWidth;
                sphere.AddToBounds(pos + delta * 0.5f, length * 0.5f);
                added = true;
            }
            if (SpriteBankIndex) {
                if (ForceAnimationTime)
                    animFraction = (float)fmod((double)(particle->GetInterpolatedLifetimer(timeOffset) * animScale), 1.0);
                C2DVector size;
                size.X = (endSize - startSize) * animFraction + startSize;
                size.Y = size.X * aspect;
                system.ScaleParticleDimensions(size);
                sphere.AddToBounds(pos, (float)sqrt(size.Y * size.Y + size.X * size.X) * 0.5f);
                added = true;
            }
        } else if (particle->GetInterpolatedLifetimer(timeOffset) > particle->GetMaxLife())
            system.ClearParticle(it);
    }
    if (!added) {
        PrimHandle.Clear();
        TrailPrimHandle.Clear();
    }
    return added;
}
}
