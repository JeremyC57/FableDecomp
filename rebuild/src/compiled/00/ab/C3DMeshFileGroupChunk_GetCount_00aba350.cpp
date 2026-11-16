// C3DMeshFileGroupChunk::GetCount() const -- the number of blend vertices in VertexList (8-byte C3DVertexBlend2, STLport vector +0x30/+0x34)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABA350, oracle length 10 bytes.
// Leaf: ?GetCount@C3DMeshFileGroupChunk@@QBEKXZ
// Retail bytes:  mov eax,[ecx+34h] ; sub eax,[ecx+30h] ; sar eax,3 ; ret
// Name fix (agentNF36 2026-11-16): was the generated stand-in CShaderPreParser::GetConstantCount. Ego_r.pdb + FableWin.pdb name sets hold
// ?GetCount@C3DMeshFileGroupChunk@@QBEKXZ; the landed callers 0x00ACDB80 and 0x00AC4150 bind it; the row now uses engine/C3DMeshFileGroupChunk.h
// (_ACCESSORS opt-in: VertexList +0x30, retail sizeof 0x3C).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK_ACCESSORS
#include "engine/C3DMeshFileGroupChunk.h"

unsigned long C3DMeshFileGroupChunk::GetCount() const
{
    return VertexList.size();
}
