// C3DMeshFileGroupChunk::GetBoneIndex() const -- the group's bone index (+0x2C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB9F60.
// Leaf: ?GetBoneIndex@C3DMeshFileGroupChunk@@QBEJXZ
// Retail bytes:  mov eax,[ecx+2Ch] ; ret
// Name fix (agentNF36 2026-11-16): was the generated stand-in __fastcall DrawGetEnvironment(const unsigned char*). Ego_r.pdb + FableWin.pdb
// name sets hold ?GetBoneIndex@C3DMeshFileGroupChunk@@QBEJXZ; the landed callers 0x00ACDB80 and 0x00AC4150 bind it; the row now uses
// engine/C3DMeshFileGroupChunk.h (_ACCESSORS opt-in: BoneIndex +0x2C).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK_ACCESSORS
#include "engine/C3DMeshFileGroupChunk.h"

long C3DMeshFileGroupChunk::GetBoneIndex() const
{
    return BoneIndex;
}
