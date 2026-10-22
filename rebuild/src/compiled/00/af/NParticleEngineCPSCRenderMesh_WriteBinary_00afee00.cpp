// NParticleEngine::CPSCRenderMesh::WriteBinary(CDataOutputStream*) -- serialize the mesh renderer: bank indices, the six trail/mesh colours, the packed bit-field integers/flags and the render-size param
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFEE00, 666 bytes. Label correction: the catalog row says
//   CPSCRenderSprite::WriteBinary, but the only reference is vtable slot 16 of NParticleEngine::CPSCRenderMesh (0x0129F96C).
// Callees (lane names, as in the landed CPSCAttractor/CPSCLight WriteBinary): WriteLong 0x00993EB0, WriteColour
//   0x00993F30, WriteULong 0x00993EA0 (propagated label "WriteCBYTE"), WriteEBOOL 0x00993E30.
// Layout: PDB (Ego_r) CPSCRenderMesh names/types; retail offsets equal the PDB: unsigned long bit-field words
//   +0x2c..+0x44, RenderSizeParam +0x48.
// Shape: one stream call per member in declaration order.
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
struct CPSCRenderMesh {
    unsigned char _base[0xc];                           // CPSCBase
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour TrailStartColour;                        // +0x14
    CRGBColour TrailMidColour;                          // +0x18
    CRGBColour TrailEndColour;                          // +0x1c
    CRGBColour StartColour;                             // +0x20
    CRGBColour MidColour;                               // +0x24
    CRGBColour EndColour;                               // +0x28
    unsigned long BlendMode : 3;                        // +0x2c
    unsigned long TrailBlendMode : 3;                   
    unsigned long BlendOp : 3;                          
    unsigned long TrailBlendOp : 3;                     
    unsigned long FadeInEndInteger : 10;                
    unsigned long FadeOutBeginInteger : 10;             
    unsigned long TrailLengthInteger : 12;              // +0x30
    unsigned long FlickerMinAlphaInteger : 10;          
    unsigned long FlickerMinSizeInteger : 10;           
    unsigned long StartRenderSizeX : 11;                // +0x34
    unsigned long StartRenderSizeY : 11;                
    unsigned long StartRenderSizeZ : 11;                // +0x38
    unsigned long CentredOnPos : 1;                     
    unsigned long AlphaFadeEnable : 1;                  
    unsigned long EndRenderSizeX : 11;                  
    unsigned long EndRenderSizeY : 11;                  // +0x3c
    unsigned long EndRenderSizeZ : 11;                  
    unsigned long SizeFadeEnable : 1;                   
    unsigned long FlickerEnable : 1;                    
    unsigned long TrailUseStartColour : 1;              
    unsigned long TrailUseMidColour : 1;                
    unsigned long TrailUseEndColour : 1;                
    unsigned long UseRenderSizeParam : 1;               
    unsigned long FlickerBias : 8;                      // +0x40
    unsigned long FlickerSpeed : 12;                    
    unsigned long AlphaFadeMinimum : 7;                 
    unsigned long UseStartColour : 1;                   
    unsigned long SizeFadeMinimum : 7;                  // +0x44
    unsigned long UseMidColour : 1;                     
    unsigned long TrailWidth : 10;                      
    unsigned long UseEndColour : 1;                     
    unsigned long RenderSizeParam;                      // +0x48
    CEnginePrimitiveHandle PrimHandle;                  // +0x4c
    long ChildPrimIndex;                                // +0x54
    CEnginePrimitiveHandle TrailPrimHandle;             // +0x58
    long ChildTrailPrimIndex;                           // +0x60
    void WriteBinary(CDataOutputStream* out);
};
void CPSCRenderMesh::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)destination;
    out->WriteLong(BankIndex);
    out->WriteLong(TrailBankIndex);
    out->WriteColour(TrailStartColour);
    out->WriteColour(TrailMidColour);
    out->WriteColour(TrailEndColour);
    out->WriteColour(StartColour);
    out->WriteColour(MidColour);
    out->WriteColour(EndColour);
    out->WriteULong(BlendMode);
    out->WriteULong(TrailBlendMode);
    out->WriteULong(BlendOp);
    out->WriteULong(TrailBlendOp);
    out->WriteULong(FadeInEndInteger);
    out->WriteULong(FadeOutBeginInteger);
    out->WriteULong(TrailLengthInteger);
    out->WriteULong(FlickerMinAlphaInteger);
    out->WriteULong(FlickerMinSizeInteger);
    out->WriteULong(StartRenderSizeX);
    out->WriteULong(StartRenderSizeY);
    out->WriteULong(StartRenderSizeZ);
    out->WriteEBOOL(CentredOnPos);
    out->WriteEBOOL(AlphaFadeEnable);
    out->WriteULong(EndRenderSizeX);
    out->WriteULong(EndRenderSizeY);
    out->WriteULong(EndRenderSizeZ);
    out->WriteEBOOL(SizeFadeEnable);
    out->WriteEBOOL(FlickerEnable);
    out->WriteEBOOL(TrailUseStartColour);
    out->WriteEBOOL(TrailUseMidColour);
    out->WriteEBOOL(TrailUseEndColour);
    out->WriteEBOOL(UseRenderSizeParam);
    out->WriteULong(FlickerBias);
    out->WriteULong(FlickerSpeed);
    out->WriteULong(AlphaFadeMinimum);
    out->WriteEBOOL(UseStartColour);
    out->WriteULong(SizeFadeMinimum);
    out->WriteEBOOL(UseMidColour);
    out->WriteULong(TrailWidth);
    out->WriteEBOOL(UseEndColour);
    out->WriteULong(RenderSizeParam);
}
}