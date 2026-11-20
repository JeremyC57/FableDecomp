#ifndef FABLE_ENGINE_CPSCSINGLESPRITE_H
#define FABLE_ENGINE_CPSCSINGLESPRITE_H
// New shared header (hand3c3, row 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere): NParticleEngine::CPSCSingleSprite over the
// shared CPSCSplineBase. Layout: Ego_r.pdb CPSCSingleSprite (struct_layouts_egor.tsv, 328 bytes; every member name and type below)
// at retail's offsets: retail's CPSCSplineBase is 52 bytes (4 lower than Ego_r's 56) and its CEnginePrimitiveSpriteTrail 0x80
// (8 lower), so SpriteBankIndex is +0x34, TrailUpdatePrimitive +0xAC and PrimIDList +0x12C (the landed same-class rows: 0x00B087C0
// PreparePrimitives, 0x00B05C60 GetComponentCost, 0x00B081A0 AddTrailPosition, 0x00B07570 ClearAllPrimitives).
// Needs FABLE_RETAIL_CPSCSPLINEBASE_PASS23 (the base's members) and FABLE_RETAIL_CARRAY + an STLport vector opt-in (PrimIDList).
#include "engine/CPSCSplineBase.h"
#include "engine/CRGBColour.h"
#include "engine/CArray.h"
#include "engine/CEnginePrimitiveSpriteTrail.h"
namespace NParticleEngine {
class CPSCSingleSprite : public CPSCSplineBase {
public:
#ifdef FABLE_RETAIL_CPSCSINGLESPRITE_ADDTOBOUNDINGSPHERE
    // hand3c3 opt-in (row 0x00B075D0): Ego_r.pdb ?AddToBoundingSphere@CPSCSingleSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@M
    // AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z, the CPSCBase override; Ego_d.pdb parameter names.
    // Declaration only: no layout change (the slot is CPSCBase's).
    virtual bool AddToBoundingSphere(C3DBoundingSphere& bsphere, const float interpolate, CParticleSystem& system,
                                     const CParticleUpdateParameters<unsigned long, unsigned long>& params);   // 0x00B075D0
#endif
protected:
    long          SpriteBankIndex;              // +0x34
    CRGBColour    StartColour;                  // +0x38
    CRGBColour    MidColour;                    // +0x3c
    CRGBColour    EndColour;                    // +0x40
    float         AnimationTimeSecs;            // +0x44
    float         StartRenderSize;              // +0x48
    float         EndRenderSize;                // +0x4c
    long          SpriteAlignment;              // +0x50
    long          NoCrossedSprites;             // +0x54
    float         InitialAngle;                 // +0x58
    long          FadeInEndInteger;             // +0x5c
    long          FadeOutBeginInteger;          // +0x60
    float         AlphaFadeMinimum;             // +0x64
    float         SizeFadeMinimum;              // +0x68
    unsigned long PositionParam;                // +0x6c
    long          BlendMode;                    // +0x70
    long          BlendOp;                      // +0x74
    long          TrailBlendMode;               // +0x78
    long          TrailBlendOp;                 // +0x7c
    long          TrailBankIndex;               // +0x80
    float         TrailWidth;                   // +0x84
    long          MaxTrailLength;               // +0x88
    long          StayWithEmitterIntFactor;     // +0x8c
    bool          UseStartColour;               // +0x90
    bool          UseMidColour;                 // +0x91
    bool          UseEndColour;                 // +0x92
    bool          SelfIlluminating : 1;         // +0x93 bit 0
    bool          AlphaFadeEnable : 1;
    bool          RotateAroundCentre : 1;
    bool          FaceMe2D : 1;
    bool          FaceMe3D : 1;
    bool          CrossedSprites : 1;
    bool          SizeFadeEnable : 1;
    bool          ForceAnimationTime : 1;
    bool          StayWithEmitter : 1;          // +0x94 bit 0
    bool          UsePosition;                  // +0x95
    bool          UseSplinePoints;              // +0x96
    bool          EnteringTimelineEnd;          // +0x97
    unsigned long LastUpdateFrame;              // +0x98
    long          Lifetimer;                    // +0x9c
    C3DVector     Offset;                       // +0xa0
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;   // +0xac
    CArray<long>  PrimIDList;                   // +0x12c
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCSingleSprite) == 0x138);
#endif
