// NParticleEngine::CPSCRenderSprite::ReadBinary(CDataInputStream& in) -- vtable slot 15: read the sprite renderer back in WriteBinary order (two bank indices, three colours, the 19 packed settings as ULONGs, the seven flags as EBOOLs)
// Leaf: ?ReadBinary@CPSCRenderSprite@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFA2B0, 2830 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb ?ReadBinary@CPSCRenderSprite@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of the CPSCRenderSprite
//   vtable (slot 15 ReadBinary of CPSCSingleSprite 0x00B068F0 / CPSCLight 0x00B193D0, landed; the mirror WriteBinary 0x00AF9620 is slot 16).
//   Catalog name "CPSCRenderSprite::ReadBinary" (no namespace).
// Callees: CDataInputStream::ReadColour 0x009945E0; ReadSLONG / ReadULONG / ReadEBOOL inline (FABLE_RETAIL_CDATAINPUTSTREAM) around
//   the out-of-line chunk-overflow reader 0x00993CA0.
// Layout: retail CPSCRenderSprite as in InterpolateTo 0x00AF9840 (local model; no shared header yet).
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
    CEnginePrimitiveHandle PrimHandle;          // +0x3c
    long          ChildPrimIndex;               // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;     // +0x48
    long          ChildTrailPrimIndex;          // +0x50
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCRenderSprite::ReadBinary(CDataInputStream& in)
{
    SpriteBankIndex = in.ReadSLONG();
    TrailBankIndex = in.ReadSLONG();
    StartColour = in.ReadColour();
    MidColour = in.ReadColour();
    EndColour = in.ReadColour();
    BlendMode = in.ReadULONG();
    TrailBlendMode = in.ReadULONG();
    BlendOp = in.ReadULONG();
    TrailBlendOp = in.ReadULONG();
    Flags = in.ReadULONG();
    FadeInEndInteger = in.ReadULONG();
    FadeOutBeginInteger = in.ReadULONG();
    TrailLengthInteger = in.ReadULONG();
    FlickerMinAlphaInteger = in.ReadULONG();
    FlickerMinSizeInteger = in.ReadULONG();
    NoCrossedSprites = in.ReadULONG();
    StartRenderSize = in.ReadULONG();
    AlphaFadeMinimum = in.ReadULONG();
    EndRenderSize = in.ReadULONG();
    FlickerBias = in.ReadULONG();
    AnimationTimeSecs = in.ReadULONG();
    SizeFadeMinimum = in.ReadULONG();
    TrailWidth = in.ReadULONG();
    FlickerSpeed = in.ReadULONG();
    UseStartColour = in.ReadEBOOL();
    UseMidColour = in.ReadEBOOL();
    UseEndColour = in.ReadEBOOL();
    AlphaFadeEnable = in.ReadEBOOL();
    SizeFadeEnable = in.ReadEBOOL();
    FlickerEnable = in.ReadEBOOL();
    ForceAnimationTime = in.ReadEBOOL();
}
}
