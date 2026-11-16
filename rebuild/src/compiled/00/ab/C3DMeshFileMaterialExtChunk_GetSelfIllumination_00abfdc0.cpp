// C3DMeshFileMaterialExtChunk::GetSelfIllumination() const -- the chunk's self-illumination word
// Leaf: ?GetSelfIllumination@C3DMeshFileMaterialExtChunk@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABFDC0: mov eax,[ecx+10h] ; ret (one retail caller, 0x00AC5476 in the landed
//   C3DMeshFileEngineMaterialChunk ctor 0x00AC53F0, which binds this symbol on GetExtendedProperties()'s chunk).
// Name: Ego_r.pdb 0x00C3C8E0 + FableWin.pdb 0x0347D170 ?GetSelfIllumination@C3DMeshFileMaterialExtChunk@@QBEKXZ; SelfIllumination +0x10
//   (engine/C3DMeshFileMaterialExtChunk.h, retail sizeof 0x14). NF33 (2026-11-16): was the propagated label CWorld::DrawGetEnvironment.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK_METHODS
#include "fable_string.h"
#include "engine/C3DMeshFileMaterialExtChunk.h"
unsigned long C3DMeshFileMaterialExtChunk::GetSelfIllumination() const
{
    return SelfIllumination;
}
