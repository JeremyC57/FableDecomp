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
struct C3DVector { float X, Y, Z; };
struct CCharString { void* PStringData; };
struct CRGBColour { union { struct { unsigned char B, G, R, A; }; unsigned long Colour; }; };
struct CEnginePrimitiveHandle { long Index; long Generation; };
struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                        // 0x00993E30
    void WriteLong(long value);                         // 0x00993EB0
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteColour(const CRGBColour& value);          // 0x00993F30
    void Write3DVector(const C3DVector& value);         // 0x00993FD0
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
};
namespace NParticleEngine {
struct CPSCSplineBase {
    unsigned char _base[0x34];                          // CPSCBase + spline data (retail size)
    void WriteBinary(CDataOutputStream* out);           // 0x00B21630
};
struct CPSCSingleSprite : CPSCSplineBase {
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
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)destination;
    out->WriteLong(SpriteBankIndex);
    out->WriteColour(StartColour);
    out->WriteColour(MidColour);
    out->WriteColour(EndColour);
    out->WriteFloat(AnimationTimeSecs);
    out->WriteFloat(StartRenderSize);
    out->WriteFloat(EndRenderSize);
    out->WriteLong(SpriteAlignment);
    out->WriteLong(NoCrossedSprites);
    out->WriteFloat(InitialAngle);
    out->WriteLong(FadeInEndInteger);
    out->WriteLong(FadeOutBeginInteger);
    out->WriteFloat(AlphaFadeMinimum);
    out->WriteFloat(SizeFadeMinimum);
    out->WriteULong(PositionParam);
    out->WriteLong(BlendMode);
    out->WriteLong(BlendOp);
    out->WriteLong(TrailBlendMode);
    out->WriteLong(TrailBlendOp);
    out->WriteLong(TrailBankIndex);
    out->WriteFloat(TrailWidth);
    out->WriteLong(MaxTrailLength);
    out->WriteLong(StayWithEmitterIntFactor);
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