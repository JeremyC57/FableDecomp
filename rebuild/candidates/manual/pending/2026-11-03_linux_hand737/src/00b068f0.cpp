// NParticleEngine::CPSCSingleSprite::ReadBinary(CDataInputStream& in) -- vtable slot 15: read the single-sprite renderer's settings back in WriteBinary order, then the CPSCSplineBase part
// Leaf: ?ReadBinary@CPSCSingleSprite@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B068F0, 3185 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb ?ReadBinary@CPSCSingleSprite@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of the
//   CPSCSingleSprite vtable 0x0129FB84 (rtti-vtables.tsv); the mirror of the landed CPSCSingleSprite::WriteBinary 0x00B06060.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (x33), CDataInputStream::ReadColour 0x009945E0 (x3),
//   CPSCSplineBase::ReadBinary 0x00B22B40 (the base part, called non-virtually last; FableWin
//   ?ReadBinary@CPSCSplineBase@NParticleEngine@@UAEXAAVCDataInputStream@@@Z, slot 15 of the CPSCSplineBase vtable 0x012A0B14 --
//   NAME_FIXES: reloc_bindings has ?ReadBinary@CPSCSplineBase@NParticleEngine@@QAEXAAUInputCalls@@@Z).
// Inlined: CDataInputStream::ReadSLONG / ReadULONG / ReadFloat / ReadEBOOL (engine/CDataInputStream.h FABLE_RETAIL_CDATAINPUTSTREAM).
// Layout: PDB (Ego_r) CPSCSingleSprite names / types with every offset 4 below the PDB (SpriteBankIndex +0x34): the CPSCSplineBase
//   base is 0x34 bytes in retail (STLport CArray, as the landed WriteBinary records); its members are not touched here (opaque).
// Shape: one read per member; UseStartColour / UseMidColour / UseEndColour / UsePosition / UseSplinePoints are plain bools
//   (`test ; setne ; mov`), the eight +0x93 flags and StayWithEmitter (+0x94) bit-fields.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/CDataInputStream.h"
#include "engine/CBaseClassNonCopyable.h"
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void ReadBinary(CDataInputStream&) = 0;
};
class CPSCSplineBase : public CPSCBase {
public:
    unsigned char _spline[0x28];                        // +0x0c SplineAnimSpeed, Spline, SplineRandomOffsetData, SplinePosParamData (retail 0x34)
    virtual void ReadBinary(CDataInputStream& in);      // 0x00B22B40
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
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCSingleSprite::ReadBinary(CDataInputStream& in)
{
    SpriteBankIndex = in.ReadSLONG();
    StartColour = in.ReadColour();
    MidColour = in.ReadColour();
    EndColour = in.ReadColour();
    AnimationTimeSecs = in.ReadFloat();
    StartRenderSize = in.ReadFloat();
    EndRenderSize = in.ReadFloat();
    SpriteAlignment = in.ReadSLONG();
    NoCrossedSprites = in.ReadSLONG();
    InitialAngle = in.ReadFloat();
    FadeInEndInteger = in.ReadSLONG();
    FadeOutBeginInteger = in.ReadSLONG();
    AlphaFadeMinimum = in.ReadFloat();
    SizeFadeMinimum = in.ReadFloat();
    PositionParam = in.ReadULONG();
    BlendMode = in.ReadSLONG();
    BlendOp = in.ReadSLONG();
    TrailBlendMode = in.ReadSLONG();
    TrailBlendOp = in.ReadSLONG();
    TrailBankIndex = in.ReadSLONG();
    TrailWidth = in.ReadFloat();
    MaxTrailLength = in.ReadSLONG();
    StayWithEmitterIntFactor = in.ReadSLONG();
    UseStartColour = in.ReadEBOOL();
    UseMidColour = in.ReadEBOOL();
    UseEndColour = in.ReadEBOOL();
    SelfIlluminating = in.ReadEBOOL();
    AlphaFadeEnable = in.ReadEBOOL();
    RotateAroundCentre = in.ReadEBOOL();
    FaceMe2D = in.ReadEBOOL();
    FaceMe3D = in.ReadEBOOL();
    CrossedSprites = in.ReadEBOOL();
    SizeFadeEnable = in.ReadEBOOL();
    ForceAnimationTime = in.ReadEBOOL();
    StayWithEmitter = in.ReadEBOOL();
    UsePosition = in.ReadEBOOL();
    UseSplinePoints = in.ReadEBOOL();
    CPSCSplineBase::ReadBinary(in);
}
}
