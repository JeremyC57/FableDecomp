// CMirroredTextureHighlightsChunk::GetCount() const -- the TriangleVertexIndexList element count (end - begin over 4-byte entries)
// Leaf: ?GetCount@CMirroredTextureHighlightsChunk@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC4F20 (an identical-COMDAT fold: the same bytes serve other 4-byte-element counts).
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CClass_00ac4f20::GetCount_00ac4f20) onto the shared
//   engine/CMirroredTextureHighlightsChunk.h (opt-ins FABLE_RETAIL_CMIRROREDTEXTUREHIGHLIGHTSCHUNK + FABLE_RETAIL_C3DPRIMITIVE2_LOADER, which
//   names 0x00AC4F20; TriangleVertexIndexList CArray<unsigned long> +0x10): Ego_r.pdb + FableWin.pdb ?GetCount@CMirroredTextureHighlightsChunk@@QBEKXZ;
//   C3DPrimitive2::InitialiseFromMeshFileData 0x00A91640 calls it on the MTEH chunk (0x00A92464) and binds this symbol.
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CMIRROREDTEXTUREHIGHLIGHTSCHUNK
#include "engine/CMirroredTextureHighlightsChunk.h"

unsigned long CMirroredTextureHighlightsChunk::GetCount() const
{
    return TriangleVertexIndexList.size();
}
