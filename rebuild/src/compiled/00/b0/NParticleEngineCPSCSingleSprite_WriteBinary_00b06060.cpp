// NParticleEngine::CPSCSingleSprite::WriteBinary(CDataOutputStream*) -- serialize the single-sprite renderer's settings, then the spline base's
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B06060, 573 bytes (vtable slot 16 of CPSCSingleSprite, 0x0129FB84).
// Callees (lane names, as in the landed CPSCAttractor/CPSCLight WriteBinary): WriteLong 0x00993EB0, WriteColour
//   0x00993F30, WriteFloat 0x00993EE0, WriteULong 0x00993EA0 (propagated label "WriteCBYTE"), WriteEBOOL 0x00993E30,
//   CPSCSplineBase::WriteBinary 0x00B21630 (the base-class part, called non-virtually last).
// Layout: PDB (Ego_r) CPSCSingleSprite names/types, but every retail offset is the PDB one minus 4
//   (SpriteBankIndex is read at +0x34, the PDB puts it at +0x38): the CPSCSplineBase base is 0x34 bytes in retail
//   (its CArray is 12 bytes, not the PDB's 16).
// Shape: one stream call per member in declaration order (EnteringTimelineEnd is runtime state, not written).
#include "engine/CDataOutputStream.h"
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* PStringData; };
class CRGBColour { public: union { struct { unsigned char B, G, R, A; }; unsigned long Colour; }; };
class CEnginePrimitiveHandle { public: long Index; long Generation; };
namespace NParticleEngine {
class CPSCSplineBase { public:
    unsigned char _base[0x34];                          // CPSCBase + spline data (retail size)
    void WriteBinary(CDataOutputStream* out);           // 0x00B21630
};
class CPSCSingleSprite : public CPSCSplineBase { public:
    long SpriteBankIndex;                               // +0x34
    CRGBColour StartColour;                             // +0x38
    CRGBColour MidColour;                               // +0x3c
    CRGBColour EndColour;                               // +0x40
    float AnimationTimeSecs;                            // +0x44
    float StartRenderSize;                              
    float EndRenderSize;                                
    long SpriteAlignment;                               // +0x50
    long NoCrossedSprites;                              
    float InitialAngle;                                 // +0x58
    long FadeInEndInteger;                              // +0x5c
    long FadeOutBeginInteger;                           
    float AlphaFadeMinimum;                             // +0x64
    float SizeFadeMinimum;                              
    unsigned long PositionParam;                        // +0x6c
    long BlendMode;                                     // +0x70
    long BlendOp;                                       
    long TrailBlendMode;                                
    long TrailBlendOp;                                  
    long TrailBankIndex;                                
    float TrailWidth;                                   // +0x84
    long MaxTrailLength;                                // +0x88
    long StayWithEmitterIntFactor;                      // +0x8c
    bool UseStartColour;                                // +0x90
    bool UseMidColour;                                  
    bool UseEndColour;                                  
    bool SelfIlluminating : 1;                          // +0x93
    bool AlphaFadeEnable : 1;                           
    bool RotateAroundCentre : 1;                        
    bool FaceMe2D : 1;                                  
    bool FaceMe3D : 1;                                  
    bool CrossedSprites : 1;                            
    bool SizeFadeEnable : 1;                            
    bool ForceAnimationTime : 1;                        
    bool StayWithEmitter : 1;                           // +0x94
    bool UsePosition;                                   // +0x95
    bool UseSplinePoints;                               // +0x96
    bool EnteringTimelineEnd;                           // +0x97
    void WriteBinary(CDataOutputStream* out);
};
void CPSCSingleSprite::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream* out = (CDataOutputStream*)destination;
    out->WriteSLONG(SpriteBankIndex);
    out->WriteColour(StartColour);
    out->WriteColour(MidColour);
    out->WriteColour(EndColour);
    out->WriteFloat(AnimationTimeSecs);
    out->WriteFloat(StartRenderSize);
    out->WriteFloat(EndRenderSize);
    out->WriteSLONG(SpriteAlignment);
    out->WriteSLONG(NoCrossedSprites);
    out->WriteFloat(InitialAngle);
    out->WriteSLONG(FadeInEndInteger);
    out->WriteSLONG(FadeOutBeginInteger);
    out->WriteFloat(AlphaFadeMinimum);
    out->WriteFloat(SizeFadeMinimum);
    out->WriteULONG(PositionParam);
    out->WriteSLONG(BlendMode);
    out->WriteSLONG(BlendOp);
    out->WriteSLONG(TrailBlendMode);
    out->WriteSLONG(TrailBlendOp);
    out->WriteSLONG(TrailBankIndex);
    out->WriteFloat(TrailWidth);
    out->WriteSLONG(MaxTrailLength);
    out->WriteSLONG(StayWithEmitterIntFactor);
    out->WriteEBOOL(UseStartColour);
    out->WriteEBOOL(UseMidColour);
    out->WriteEBOOL(UseEndColour);
    out->WriteEBOOL(SelfIlluminating);
    out->WriteEBOOL(AlphaFadeEnable);
    out->WriteEBOOL(RotateAroundCentre);
    out->WriteEBOOL(FaceMe2D);
    out->WriteEBOOL(FaceMe3D);
    out->WriteEBOOL(CrossedSprites);
    out->WriteEBOOL(SizeFadeEnable);
    out->WriteEBOOL(ForceAnimationTime);
    out->WriteEBOOL(StayWithEmitter);
    out->WriteEBOOL(UsePosition);
    out->WriteEBOOL(UseSplinePoints);
    CPSCSplineBase::WriteBinary(destination);
}
}