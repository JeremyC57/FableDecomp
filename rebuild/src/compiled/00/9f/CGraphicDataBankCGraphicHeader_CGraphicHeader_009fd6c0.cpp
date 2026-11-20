// CGraphicDataBank::CGraphicHeader::CGraphicHeader() -- an empty graphic header: zero sizes, one frame, no pixel format, no mipmaps, not a flat animated graphic
// Leaf: ??0CGraphicHeader@CGraphicDataBank@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FD6C0, 58 bytes. Name: Ego_r/FableWin PDB ??0CGraphicHeader@CGraphicDataBank@@QAE@XZ.
//   Evidence: the layout below is the PDB CGraphicDataBank::CGraphicHeader (0x1C bytes) store for store, and the sole caller
//   CEngineSurfaceCompositionManager::WriteTextureToBank (0x00B2BF1D) then fills Width/Height/FrameWidth/... words and
//   clears the IsFlatAnimatedGraphic bit of this local. The catalog label "C3DAnimationSequenceData::C3DAnimationSequenceData"
//   is propagated (that PDB class is 0x2C bytes with a char* first).
// Shape: a member-initialiser list in declaration order (body assignments put PixelFormat's ctor first); CPixelFormat's inline ctor (FormatIndex = -1); the one-bit field is read-modify-written (`and dl,0FEh`).
// NF141 (2026-11-20, q6182 one definition): was a strong (nodup) out-of-line definition over local CPixelFormat / CGraphicDataBank /
//   CGraphicHeader models (the CPixelFormat one through a renaming include). The constructor is header-inline in retail (expanded in
//   CGraphicDataBank::NotifyEntryAdded 0x009FF606..0x009FF650; this out-of-line copy is the COMDAT the WriteTextureToBank TU called), and the
//   ported 0x009FF590 now emits the shared header's inline body as a pick-any COMDAT, which would clash (LNK2005) with a strong owner. So the
//   owner is the ONE shared inline body of engine/CGraphicDataBank.h (CGraphicDataBank::CGraphicHeader, retail STATICBLOCKS branch) emitted
//   pick-any through the nf86 / nf88 scaffolding below; no object defines a nodup copy.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICDATABANK_CGRAPHICHEADER_LOAD
#include "engine/CGraphicInfo.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest out-of-line use of the header's inline CGraphicHeader constructor (a local header loaded
// from a stream, the form retail's NotifyEntryAdded 0x009FF590 expands), so this object emits the constructor as a pick-any COMDAT.
// Internal to this TU.
void ReferenceCGraphicHeaderCtor(CDataInputStream& stream)
{
    CGraphicDataBank::CGraphicHeader header;
    header.Load(stream);
}
}
