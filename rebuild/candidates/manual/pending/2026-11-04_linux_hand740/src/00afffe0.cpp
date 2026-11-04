// NParticleEngine::CPSCRenderMesh::ReadBinary(CDataInputStream& in) -- vtable slot 15: read every render-mesh member from a binary emitter stream
// Leaf: ?ReadBinary@CPSCRenderMesh@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFFFE0, 3536 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb ?ReadBinary@CPSCRenderMesh@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of vtable 0x0129F96C, between
//   InterpolateTo (14) and WriteBinary (16, landed 0x00AFEE00), as in the CPSCSingleSprite / CPSCLight vtables (chunk 737).
// Shape: one `X = in.ReadSLONG/ReadULONG/ReadEBOOL/ReadColour()` per member, in WriteBinary's order (each inline read is the
//   CDataInputStream fast path with ReadWithSrcChunkOverflow 0x00993CA0 as the overflow call; colours through ReadColour 0x009945E0).
// Header debt: no shared CPSCRenderMesh header (local model, Ego_r.pdb layout).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/CDataInputStream.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
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
class CPSCRenderMesh : public CPSCBase {
public:
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
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCRenderMesh::ReadBinary(CDataInputStream& in)
{
    BankIndex = in.ReadSLONG();
    TrailBankIndex = in.ReadSLONG();
    TrailStartColour = in.ReadColour();
    TrailMidColour = in.ReadColour();
    TrailEndColour = in.ReadColour();
    StartColour = in.ReadColour();
    MidColour = in.ReadColour();
    EndColour = in.ReadColour();
    BlendMode = in.ReadULONG();
    TrailBlendMode = in.ReadULONG();
    BlendOp = in.ReadULONG();
    TrailBlendOp = in.ReadULONG();
    FadeInEndInteger = in.ReadULONG();
    FadeOutBeginInteger = in.ReadULONG();
    TrailLengthInteger = in.ReadULONG();
    FlickerMinAlphaInteger = in.ReadULONG();
    FlickerMinSizeInteger = in.ReadULONG();
    StartRenderSizeX = in.ReadULONG();
    StartRenderSizeY = in.ReadULONG();
    StartRenderSizeZ = in.ReadULONG();
    CentredOnPos = in.ReadEBOOL();
    AlphaFadeEnable = in.ReadEBOOL();
    EndRenderSizeX = in.ReadULONG();
    EndRenderSizeY = in.ReadULONG();
    EndRenderSizeZ = in.ReadULONG();
    SizeFadeEnable = in.ReadEBOOL();
    FlickerEnable = in.ReadEBOOL();
    TrailUseStartColour = in.ReadEBOOL();
    TrailUseMidColour = in.ReadEBOOL();
    TrailUseEndColour = in.ReadEBOOL();
    UseRenderSizeParam = in.ReadEBOOL();
    FlickerBias = in.ReadULONG();
    FlickerSpeed = in.ReadULONG();
    AlphaFadeMinimum = in.ReadULONG();
    UseStartColour = in.ReadEBOOL();
    SizeFadeMinimum = in.ReadULONG();
    UseMidColour = in.ReadEBOOL();
    TrailWidth = in.ReadULONG();
    UseEndColour = in.ReadEBOOL();
    RenderSizeParam = in.ReadULONG();
}
}
