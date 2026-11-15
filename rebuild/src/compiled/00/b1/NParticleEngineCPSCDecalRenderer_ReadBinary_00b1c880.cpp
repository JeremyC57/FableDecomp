// NParticleEngine::CPSCDecalRenderer::ReadBinary(CDataInputStream&) -- read the decal renderer back (the mirror of WriteBinary 0x00B1BA10)
// Leaf: ?ReadBinary@CPSCDecalRenderer@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1C880, 3266 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb + Ego_r.pdb name sets ?ReadBinary@CPSCDecalRenderer@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of
//   the CPSCDecalRenderer vtable 0x012A09BC (slot 16 = the landed WriteBinary 0x00B1BA10, slot 18 = the landed DatabaseCopy 0x00B1B810).
// Layout: engine/CPSCDecalRenderer.h (new shared header in this hand-back: Ego_r layout, the landed same-class rows' offsets);
//   the CDataInputStream readers are the shared FABLE_RETAIL_CDATAINPUTSTREAM_READERS inlines (ReadULONG / ReadSLONG / ReadFloat /
//   ReadEBOOL / ReadColour). Each value is stored into its field in the WriteBinary order; the bit fields merge into their packed words.
// History: earlier lanes left a 4-line or-operand-order residue on SizeFadeMinimum (+0x28 bits 25-31); RELOCATION_MATCH against the
//   2026-11-14 headers with the source unchanged (wfD6).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READERS
#include "engine/CDataInputStream.h"
#include "engine/CRGBColour.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CPSCDecalRenderer.h"
namespace NParticleEngine {
void CPSCDecalRenderer::ReadBinary(CDataInputStream& in)
{
    DecalLifeSecs = in.ReadFloat();
    DecalBankIndex = in.ReadSLONG();
    StartColour = in.ReadColour();
    MidColour = in.ReadColour();
    EndColour = in.ReadColour();
    BlendMode = in.ReadULONG();
    BlendOp = in.ReadULONG();
    Flags = in.ReadULONG();
    FadeInEndInteger = in.ReadULONG();
    FadeOutBeginInteger = in.ReadULONG();
    StartRenderSize = in.ReadULONG();
    AlphaFadeMinimum = in.ReadULONG();
    EndRenderSize = in.ReadULONG();
    AnimationTimeSecs = in.ReadULONG();
    SizeFadeMinimum = in.ReadULONG();
    UseStartColour = in.ReadEBOOL();
    UseMidColour = in.ReadEBOOL();
    UseEndColour = in.ReadEBOOL();
    AlphaFadeEnable = in.ReadEBOOL();
    SizeFadeEnable = in.ReadEBOOL();
    ForceAnimationTime = in.ReadEBOOL();
    FlickerMinAlphaInteger = in.ReadULONG();
    FlickerMinSizeInteger = in.ReadULONG();
    FlickerBias = in.ReadULONG();
    FlickerSpeed = in.ReadULONG();
    FlickerEnable = in.ReadEBOOL();
    PoolingEnable = in.ReadEBOOL();
    PoolIncreaseAlphaEnable = in.ReadEBOOL();
    PoolIncreaseSizeEnable = in.ReadEBOOL();
    PoolIncreaseLifeEnable = in.ReadEBOOL();
    MaxPoolSize = in.ReadFloat();
    MaxPoolAlpha = in.ReadFloat();
    MaxPoolLife = in.ReadFloat();
    PoolIncreaseFrameEnable = in.ReadEBOOL();
    PoolFrameIncreaseRate = in.ReadFloat();
    StencilCubeWidth = in.ReadFloat();
}
}