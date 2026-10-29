// NParticleEngine::CPSCRenderSprite::WriteBinary(CDataOutputStream*) -- serialize the sprite renderer: bank indices, the three colours, the packed bit-field integers/flags and the four fade/flicker flags
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF9620, 538 bytes (vtable slot 16 of CPSCRenderSprite, 0x0129F67C).
// Callees (lane names, as in the landed CPSCAttractor/CPSCLight WriteBinary): WriteLong 0x00993EB0, WriteColour
//   0x00993F30, WriteULong 0x00993EA0 (propagated label "WriteCBYTE"), WriteEBOOL 0x00993E30.
// Layout: PDB (Ego_r) CPSCRenderSprite names/types; retail offsets equal the PDB: unsigned long bit-field words
//   +0x20..+0x34, unsigned char flag bits at +0x38.
// Shape: one stream call per member in declaration order.
#include "engine/CDataOutputStream.h"
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* PStringData; };
class CRGBColour { public: union { struct { unsigned char B, G, R, A; }; unsigned long Colour; }; };
class CEnginePrimitiveHandle { public: long Index; long Generation; };
namespace NParticleEngine {
class CPSCRenderSprite { public:
    unsigned char _base[0xc];                           // CPSCBase
    long SpriteBankIndex;                               // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour StartColour;                             // +0x14
    CRGBColour MidColour;                               // +0x18
    CRGBColour EndColour;                               // +0x1c
    unsigned long BlendMode : 3;                        // +0x20
    unsigned long TrailBlendMode : 3;                   
    unsigned long BlendOp : 3;                          
    unsigned long TrailBlendOp : 3;                     
    unsigned long Flags : 10;                           
    unsigned long FadeInEndInteger : 10;                
    unsigned long FadeOutBeginInteger : 10;             // +0x24
    unsigned long TrailLengthInteger : 12;              
    unsigned long FlickerMinAlphaInteger : 10;          
    unsigned long FlickerMinSizeInteger : 10;           // +0x28
    unsigned long NoCrossedSprites : 5;                 
    unsigned long StartRenderSize : 11;                 
    unsigned long AlphaFadeMinimum : 7;                 // +0x2c
    unsigned long EndRenderSize : 11;                   
    unsigned long FlickerBias : 8;                      
    unsigned long AnimationTimeSecs : 14;               // +0x30
    unsigned long SizeFadeMinimum : 7;                  
    unsigned long TrailWidth : 10;                      
    unsigned long FlickerSpeed : 12;                    // +0x34
    unsigned long UseStartColour : 1;                   
    unsigned long UseMidColour : 1;                     
    unsigned long UseEndColour : 1;                     
    unsigned char AlphaFadeEnable : 1;                  // +0x38
    unsigned char SizeFadeEnable : 1;                   
    unsigned char FlickerEnable : 1;                    
    unsigned char ForceAnimationTime : 1;               
    CEnginePrimitiveHandle PrimHandle;                  // +0x3c
    long ChildPrimIndex;                                // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;             // +0x48
    long ChildTrailPrimIndex;                           // +0x50
    void WriteBinary(CDataOutputStream* out);
};
void CPSCRenderSprite::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream* out = (CDataOutputStream*)destination;
    out->WriteSLONG(SpriteBankIndex);
    out->WriteSLONG(TrailBankIndex);
    out->WriteColour(StartColour);
    out->WriteColour(MidColour);
    out->WriteColour(EndColour);
    out->WriteULONG(BlendMode);
    out->WriteULONG(TrailBlendMode);
    out->WriteULONG(BlendOp);
    out->WriteULONG(TrailBlendOp);
    out->WriteULONG(Flags);
    out->WriteULONG(FadeInEndInteger);
    out->WriteULONG(FadeOutBeginInteger);
    out->WriteULONG(TrailLengthInteger);
    out->WriteULONG(FlickerMinAlphaInteger);
    out->WriteULONG(FlickerMinSizeInteger);
    out->WriteULONG(NoCrossedSprites);
    out->WriteULONG(StartRenderSize);
    out->WriteULONG(AlphaFadeMinimum);
    out->WriteULONG(EndRenderSize);
    out->WriteULONG(FlickerBias);
    out->WriteULONG(AnimationTimeSecs);
    out->WriteULONG(SizeFadeMinimum);
    out->WriteULONG(TrailWidth);
    out->WriteULONG(FlickerSpeed);
    out->WriteEBOOL(UseStartColour);
    out->WriteEBOOL(UseMidColour);
    out->WriteEBOOL(UseEndColour);
    out->WriteEBOOL(AlphaFadeEnable);
    out->WriteEBOOL(SizeFadeEnable);
    out->WriteEBOOL(FlickerEnable);
    out->WriteEBOOL(ForceAnimationTime);
}
}