// NParticleEngine::CPSCRenderSprite::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle) -- vtable slot 13: set the sprite-group and trail-group child primitives, then per live particle a sprite (or NoCrossedSprites rotated copies) and a trail line, faded in / out, flickered through the Perlin noise and coloured along the start / mid / end ramp
// Grade: DIFFER(5338v5274) -- 10 grades (LOG.tsv a..j, k = this file on the shared headers); the instruction stream lines up with retail's (registers, x87 order and every call); residue: (1) retail emits no destructor for the three default CArray<long>() temporaries passed to vector<CArray<long> >::resize 0x00AFE910 (ours: `mov eax,[tmp] ; test ; je ; push ; call free` x3, +57 bytes); (2) frame 0x334 v 0x328 and a different slot permutation (same code, other [ebp+N]); (3) blend-field register order (retail esi/edi/ecx copies, zero in edx) and the zero register of the single-sprite SetPrimitiveInfo
// Leaf: ?PreparePrimitives@CPSCRenderSprite@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFCC30, 5274 bytes, thiscall, ebp frame (ebp = esp - 0x64), `ret 14h`.
// Identity: FableWin.pdb ?PreparePrimitives@CPSCRenderSprite@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z;
//   slot 13 of the CPSCRenderSprite vtable (chunk 737's proof: slot 13 = PreparePrimitives, `ret 14h`); catalog "vfunc_13".
// Callees: this slot 29 ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle&) (landed 0x00AFADC0; FableWin ?ClearAllPrimitives@CPSCRenderSprite@..),
//   NParticleEngine::GetTickRate 0x00AEA9F0 (result unused), GetOneOverTickRate 0x00AEAA00, GetGraphicBank 0x00AEAAA0,
//   GetParticleEmitterManager 0x00AEAAF0 (+8 = PeekPerlinNoise), NFractal::CPerlinNoise::Get2DNoise 0x00F36540, CIEngine slots 26
//   AddChildPrimitive / 27 SetChildPrimitive(5 args) / 28 SetChildPrimitive(4 args), GFGetInterpolatedColour 0x00A44250,
//   CEnginePrimitiveParticleSprite ctor 0x00AFE2A0, CEnginePrimitiveParticleLine ctor 0x00AFE220, vector<CArray<long> >::resize
//   0x00AFE910, vector<long>::resize 0x004428F0, vector<CEnginePrimitiveHandle>::resize(n) 0x00443F90, CParticleSystem::
//   GetSystemTransform 0x00AF2250, ClearParticle 0x00AF5090, ScaleParticleDimensions C2DVector& 0x00AF8F00 / float& 0x00AF8F80,
//   __ftol2 (the three ramp alphas * FadeAlpha), _CIfmod.
// Shape: CParticle::GetPrimID(long, long, long&) / SetPrimitiveInfo(long, long, const CEnginePrimitiveHandle&, long) (FableWin names)
//   inline over PrimIDList +0x84 / ParentHandleList +0x90; CEnginePrimitiveHandle::operator= (FableWin ??4.., inline: Clear then the
//   counted-pointer assignment, overlay header); GFSmoothFadeValue (landed 0x00AF9000) inline; the frame index is a double-evaluated
//   min of GFFloatToLongFloor; colours[3] ramp indexed by GFFloatToLongNear(lifeFraction * 511) >> 8.
// Retail-asm: 0x00AFCCAC `fistp dword ptr [ebp+60h]` (GFFloatToLongNear, unpack rounding), 0x00AFD424 `fistp dword ptr [edx]`
//   (GFFastFloatToLong in IsActive(float)), 0x00AFD846 (GFFloatToLongFloor), 0x00AFDCE5 (GFFloatToLongCeil) -- shared helper headers.
// Next idea: the missing temp destructors look like a TU / build difference (no C++ spelling tried removes them: named const local,
//   one-argument resize); try the remaining frame permutation only once that is understood.
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CMATRIX3X4
#include <math.h>
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/GFPackFloat.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "stlport/fable_containers.h"
extern float g_CosTable[1025];                          // 0x013CD550
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
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
class CEnginePrimitiveParticleSpriteGroup : public CEnginePrimitive {
public:
    CEnginePrimitiveParticleSpriteGroup(unsigned long bank_index, unsigned short flags, EEnginePrimitiveBlendMode blend_mode,
                                        EEnginePrimitiveBlendOp blend_op)
        : CEnginePrimitive((EEnginePrimitiveType)0x1b, (EEngineInterpolationSet)0), BankIndex(bank_index), Flags(flags),
          BlendMode(blend_mode), BlendOp(blend_op) { }
    unsigned long BankIndex;                            // +0x0c
    unsigned short Flags;                               // +0x10
    EEnginePrimitiveBlendMode BlendMode;                // +0x14
    EEnginePrimitiveBlendOp BlendOp;                    // +0x18
};
class CEnginePrimitiveParticleSpriteTrailGroup : public CEnginePrimitive {
public:
    CEnginePrimitiveParticleSpriteTrailGroup(unsigned long bank_index, unsigned short flags, EEnginePrimitiveBlendMode blend_mode,
                                             EEnginePrimitiveBlendOp blend_op)
        : CEnginePrimitive((EEnginePrimitiveType)0x1a, (EEngineInterpolationSet)0), BankIndex(bank_index), Flags(flags),
          BlendMode(blend_mode), BlendOp(blend_op) { }
    unsigned long BankIndex;                            // +0x0c
    unsigned short Flags;                               // +0x10
    EEnginePrimitiveBlendMode BlendMode;                // +0x14
    EEnginePrimitiveBlendOp BlendOp;                    // +0x18
};
class CEnginePrimitiveParticleSprite : public CEnginePrimitive {
public:
    CEnginePrimitiveParticleSprite(unsigned __int64 id, unsigned long frame, const C3DVector& pos, const C2DVector& size, float angle,
                                   const CRGBColour& colour, EEngineInterpolationSet interpolation_set);   // 0x00AFE2A0
    unsigned char _body[0x2c - 0xc];
};
class CEnginePrimitiveParticleLine : public CEnginePrimitive {
public:
    CEnginePrimitiveParticleLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                                 float width, EEngineInterpolationSet interpolation_set);                 // 0x00AFE220
    unsigned char _body[0x30 - 0xc];
};
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250
namespace NFractal {
class CPerlinNoise {
public:
    float Get2DNoise(const C2DVector& pos) const;      // 0x00F36540
};
}
namespace NParticleEngine {
long __fastcall GetTickRate();                          // 0x00AEA9F0
float __fastcall GetOneOverTickRate();                  // 0x00AEAA00
const CGraphicDataBank& __fastcall GetGraphicBank();    // 0x00AEAAA0
class CParticleEmitterManager {
public:
    unsigned char _pad00[8];
    NFractal::CPerlinNoise PerlinNoise;                 // +0x08
    const NFractal::CPerlinNoise& PeekPerlinNoise() const { return PerlinNoise; }
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();    // 0x00AEAAF0
__forceinline float __fastcall GFSmoothFadeValue(float t)            // out-of-line copy 0x00AF9000
{
    t = t * 0.5f;
    t = t * 1024.0f;
    long i = GFFloatToLongNear(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
}
template <class T> inline T GFClamp(T value, T low, T high) { return value < low ? low : value > high ? high : value; }
#pragma pack(push, 1)
class CParticle {
public:
    virtual ~CParticle();
    unsigned char _pad04[0x24 - 4];
    float Angle;                                        // +0x24
    signed char AngleChange;                            // +0x28
    C3DVector PositionBuffer[4];                        // +0x29
    unsigned char _pad59[0x72 - 0x59];
    short Lifetimer;                                    // +0x72
    unsigned char CreationTime;                         // +0x74
    unsigned char _pad75;
    short MaxLife;                                      // +0x76
    unsigned char FlickerIndex;                         // +0x78
    unsigned char FlickerTimeOffset;                    // +0x79
    bool Active;                                        // +0x7a
    unsigned char _pad7b[0x84 - 0x7b];
    CArray<CArray<long> > PrimIDList;                   // +0x84
    CArray<CEnginePrimitiveHandle> ParentHandleList;    // +0x90
    float GetAngle() const { return Angle; }
    float GetAngleChange() const { return AngleChange * (2.0f / 127.0f); }
    float GetCreationTime() const { return CreationTime * (1.0f / 255.0f); }
    long GetMaxLife() const { return MaxLife; }
    unsigned char GetFlickerIndex() const { return FlickerIndex; }
    unsigned char GetFlickerTimeOffset() const { return FlickerTimeOffset; }
    float GetInterpolatedLifetimer(float fraction) const { return Lifetimer - GetCreationTime() + fraction; }
    bool IsActive() const { return Active; }
    __forceinline bool IsActive(float time) const
    {
        if (Lifetimer > 0) return true;
        long creation = GFFastFloatToLong(time * 255.0f);
        return creation * (1.0f / 255.0f) >= GetCreationTime();
    }
    __forceinline C3DVector GetInterpolatedPos(float t) const { return PositionBuffer[1] + (PositionBuffer[0] - PositionBuffer[1]) * t; }
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
    __forceinline bool GetPrimID(long list, long index, long& id) const
    {
        if (PrimIDList.size() > (unsigned long)list && PrimIDList[list].size() > (unsigned long)index) {
            id = PrimIDList[list][index];
            return id >= 0;
        }
        return false;
    }
    __forceinline void SetPrimitiveInfo(long list, long index, const CEnginePrimitiveHandle& parent, long id)
    {
        if (PrimIDList.size() <= (unsigned long)list)
            PrimIDList.resize(list + 1, CArray<long>());
        if (PrimIDList[list].size() <= (unsigned long)index)
            PrimIDList[list].resize(index + 1, 0);
        PrimIDList[list][index] = id;
        if (ParentHandleList.size() <= (unsigned long)list)
            ParentHandleList.resize(list + 1);
        ParentHandleList[list] = parent;
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
    void ScaleParticleDimensions(float& size) const;    // 0x00AF8F80
    std::list<CParticle*>& GetParticleList() { return Particles; }
    float GetGlobalAlpha() const { return GlobalAlpha; }
    float GetFadeAlpha() const { return FadeAlpha; }
    __forceinline void TransformPosIfLocal(C3DVector& pos) const { if (StayWithEmitter) { CMatrix3x4 transform; GetSystemTransform(transform); transform.TransformPoint(pos); } }
};
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void _vslot01(); virtual void _vslot02(); virtual void _vslot03(); virtual void _vslot04(); virtual void _vslot05();
    virtual void _vslot06(); virtual void _vslot07(); virtual void _vslot08(); virtual void _vslot09(); virtual void _vslot10();
    virtual void _vslot11(); virtual void _vslot12();
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) = 0;   // 13
    virtual void _vslot14(); virtual void _vslot15(); virtual void _vslot16(); virtual void _vslot17(); virtual void _vslot18();
    virtual void _vslot19(); virtual void _vslot20(); virtual void _vslot21(); virtual void _vslot22(); virtual void _vslot23();
    virtual void _vslot24(); virtual void _vslot25(); virtual void _vslot26(); virtual void _vslot27(); virtual void _vslot28();
    virtual void ClearAllPrimitives(CIEngine* engine, CEnginePrimitiveHandle& handle);   // 29
};
class CPSCRenderSprite : public CPSCBase {
public:
    long          SpriteBankIndex;              // +0x0c
    long          TrailBankIndex;               // +0x10
    CRGBColour    StartColour;                  // +0x14
    CRGBColour    MidColour;                    // +0x18
    CRGBColour    EndColour;                    // +0x1c
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
    virtual void PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
};
void CPSCRenderSprite::PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                         const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle)
{
    if (!SpriteBankIndex && !TrailBankIndex) {
        ClearAllPrimitives(info.Engine, handle);
        return;
    }
    float alphaFadeMinimum = GFUnpackFloat(AlphaFadeMinimum, 7, 0.0f, 1.0f, 2);
    float sizeFadeMinimum = GFUnpackFloat(SizeFadeMinimum, 7, 0.0f, 1.0f, 2);
    float animationTime = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    float startSize = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    float endSize = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    float flickerSpeed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
    float flickerBias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
    float trailWidth = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    GetTickRate();
    float oneOverTickRate = GetOneOverTickRate();
    const CGraphicDataBank& bank = GetGraphicBank();
    const NFractal::CPerlinNoise& noise = GetParticleEmitterManager().PeekPerlinNoise();
    float requestedLength = TrailLengthInteger * 0.001f;
    bool longTrail;
    float trailLength;
    if (requestedLength > 2.0f) {
        longTrail = true;
        trailLength = 2.0f;
    } else {
        trailLength = requestedLength;
        longTrail = false;
    }
    float trailScale = trailLength * 0.5f;
    float fadeInEnd = FadeInEndInteger * 0.001f;
    float fadeOutBegin = FadeOutBeginInteger * 0.001f;
    float flickerMinAlpha = FlickerMinAlphaInteger * 0.001f;
    float flickerMinSize = FlickerMinSizeInteger * 0.001f;
    float flickerScale = (float)fabs(flickerBias) + 0.5f;
    float flickerOffset = flickerBias + 0.5f;
    float flickerTimeScale = oneOverTickRate * flickerSpeed;
    float animationScale = 1.0f / animationTime;
    float crossedAngleStep = 1.0f / NoCrossedSprites;
    float fadeAlpha = system.GetFadeAlpha();
    float fadeInScale = 1.0f / fadeInEnd;
    float fadeOutScale = 1.0f / (1.0f - fadeOutBegin);
    float flickerAlphaRange = 1.0f - flickerMinAlpha;
    float flickerSizeRange = 1.0f - flickerMinSize;
    float alphaFadeBase, alphaFadeRange;
    if (AlphaFadeEnable) {
        alphaFadeBase = alphaFadeMinimum;
        alphaFadeRange = 1.0f - alphaFadeMinimum;
    } else {
        alphaFadeRange = 0.0f;
        alphaFadeBase = 1.0f;
    }
    float sizeFadeBase, sizeFadeRange;
    if (SizeFadeEnable) {
        sizeFadeBase = sizeFadeMinimum;
        sizeFadeRange = 1.0f - sizeFadeMinimum;
    } else {
        sizeFadeRange = 0.0f;
        sizeFadeBase = 1.0f;
    }
    float angleScale = 0.5f;
    if ((Flags & 2) || (Flags & 4))
        angleScale = 1.0f;
    long blendOp = BlendOp;
    long blendMode = BlendMode;
    long trailBlendOp = TrailBlendOp;
    long trailBlendMode = TrailBlendMode;
    long trailList = 0;
    long noFrames = 0;
    float framesFloat = 0.0f;
    float aspect = 0.0f;
    if (SpriteBankIndex) {
        noFrames = bank.GetNoFramesInGraphic(SpriteBankIndex);
        framesFloat = (float)noFrames;
        const CGraphicInfo* frameGraphic = bank.GetGraphicInfo(SpriteBankIndex)->Frames[0].GraphicInfo;
        aspect = (float)frameGraphic->FrameHeight / frameGraphic->FrameWidth;
        info.Engine->SetChildPrimitive(handle, ChildPrimIndex, PrimHandle,
            CEnginePrimitiveParticleSpriteGroup(SpriteBankIndex, Flags, (EEnginePrimitiveBlendMode)blendMode,
                                                (EEnginePrimitiveBlendOp)blendOp), 0);
        trailList = 1;
    }
    long lastFrame = noFrames - 1;
    if (TrailBankIndex)
        info.Engine->SetChildPrimitive(handle, ChildTrailPrimIndex, TrailPrimHandle,
            CEnginePrimitiveParticleSpriteTrailGroup(TrailBankIndex, Flags, (EEnginePrimitiveBlendMode)trailBlendMode,
                                                     (EEnginePrimitiveBlendOp)trailBlendOp), 0);
    CRGBColour colours[3];
    colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
    colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
    colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 128);
    colours[0].A = (unsigned char)(colours[0].A * fadeAlpha);
    colours[1].A = (unsigned char)(colours[1].A * fadeAlpha);
    colours[2].A = (unsigned char)(colours[2].A * fadeAlpha);
    CRGBColour colour;
    C3DVector pos;
    for (CParticleSystem::ParticleIterator it = system.GetParticleList().begin(); it != system.GetParticleList().end(); ++it) {
        CParticle* particle = *it;
        float lifetimer = particle->GetInterpolatedLifetimer(time);
        if (particle->IsActive() && particle->IsActive(time) && particle->GetMaxLife()) {
            float lifeFraction = GFClamp(lifetimer / particle->GetMaxLife(), 0.0f, 1.0f);
            unsigned long colourIndex = GFFloatToLongNear(lifeFraction * 511.0f);
            unsigned long colourSegment = colourIndex >> 8;
            colour = GFGetInterpolatedColour(colours[colourSegment], colours[colourSegment + 1], colourIndex & 0xff);
            float fade = system.GetGlobalAlpha();
            if (lifeFraction < fadeInEnd)
                fade *= GFSmoothFadeValue(lifeFraction * fadeInScale);
            if (lifeFraction > fadeOutBegin)
                fade *= GFSmoothFadeValue((1.0f - lifeFraction) * fadeOutScale);
            float alphaScale = fade * alphaFadeRange + alphaFadeBase;
            float sizeScale = fade * sizeFadeRange + sizeFadeBase;
            if (FlickerEnable) {
                C2DVector noisePos;
                noisePos.X = lifetimer * flickerTimeScale + particle->GetFlickerTimeOffset();
                noisePos.Y = particle->GetFlickerIndex();
                float flicker = GFClamp(noise.Get2DNoise(noisePos) * flickerScale + flickerOffset, 0.0f, 1.0f);
                alphaScale *= flickerAlphaRange * flicker + flickerMinAlpha;
                sizeScale *= flickerSizeRange * flicker + flickerMinSize;
            }
            colour.A = (unsigned char)GFFloatToLongNear(colour.A * alphaScale);
            if (colour.A) {
                pos = particle->GetInterpolatedPos(time);
                system.TransformPosIfLocal(pos);
                if (SpriteBankIndex) {
                    float animFraction = lifeFraction;
                    if (ForceAnimationTime)
                        animFraction = (float)fmod((double)(animationScale * oneOverTickRate * lifetimer), 1.0);
                    float angle = particle->GetAngleChange() * time + particle->GetAngle();
                    float frameValue = animFraction * framesFloat;
                    long frame = GFFloatToLongFloor(frameValue) < lastFrame ? GFFloatToLongFloor(frameValue) : lastFrame;
                    C2DVector size;
                    size.X = ((endSize - startSize) * animFraction + startSize) * sizeScale;
                    size.Y = size.X * aspect;
                    system.ScaleParticleDimensions(size);
                    if (Flags & 0x40) {
                        for (long i = 0; i < NoCrossedSprites; ++i) {
                            float spriteAngle = (i + 0.5f) * angleScale * crossedAngleStep + angle;
                            long id;
                            if (particle->GetPrimID(0, i, id))
                                info.Engine->SetChildPrimitive(PrimHandle, id,
                                    CEnginePrimitiveParticleSprite(0, frame, pos, size, spriteAngle, colour, (EEngineInterpolationSet)0), 0);
                            else
                                particle->SetPrimitiveInfo(0, i, PrimHandle, info.Engine->AddChildPrimitive(PrimHandle,
                                    CEnginePrimitiveParticleSprite(0, frame, pos, size, spriteAngle, colour, (EEngineInterpolationSet)0), 0));
                        }
                    } else {
                        long id;
                        if (particle->GetPrimID(0, 0, id))
                            info.Engine->SetChildPrimitive(PrimHandle, id,
                                CEnginePrimitiveParticleSprite(0, frame, pos, size, angle, colour, (EEngineInterpolationSet)0), 0);
                        else
                            particle->SetPrimitiveInfo(0, 0, PrimHandle, info.Engine->AddChildPrimitive(PrimHandle,
                                CEnginePrimitiveParticleSprite(0, frame, pos, size, angle, colour, (EEngineInterpolationSet)0), 0));
                    }
                }
                if (TrailBankIndex) {
                    C3DVector tail;
                    tail = particle->GetRecordedPos(trailLength, time);
                    if (longTrail)
                        tail = pos + (tail - pos) * trailScale;
                    system.TransformPosIfLocal(tail);
                    float width = sizeScale * trailWidth;
                    system.ScaleParticleDimensions(width);
                    long id;
                    if (particle->GetPrimID(trailList, 0, id))
                        info.Engine->SetChildPrimitive(TrailPrimHandle, id,
                            CEnginePrimitiveParticleLine(0, pos, tail, colour, width, (EEngineInterpolationSet)0), 0);
                    else
                        particle->SetPrimitiveInfo(trailList, 0, TrailPrimHandle, info.Engine->AddChildPrimitive(TrailPrimHandle,
                            CEnginePrimitiveParticleLine(0, pos, tail, colour, width, (EEngineInterpolationSet)0), 0));
                }
            }
        } else if (lifetimer > particle->GetMaxLife())
            system.ClearParticle(it);
    }
}
}
