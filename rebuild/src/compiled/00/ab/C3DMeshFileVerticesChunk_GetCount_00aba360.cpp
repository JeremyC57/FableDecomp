// C3DMeshFileVerticesChunk::GetCount() const -- the number of vertices in VertexList (32-byte C3DVertex2, STLport vector +0x10/+0x14)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABA360, oracle length 10 bytes.
// Leaf: ?GetCount@C3DMeshFileVerticesChunk@@QBEKXZ
// Retail bytes:  mov eax,[ecx+14h] ; sub eax,[ecx+10h] ; sar eax,5 ; ret
// Name fix (agentNF36 2026-11-16): was the generated stand-in CShaderPreParser::GetConstantCount. Ego_r.pdb + FableWin.pdb name sets hold
// ?GetCount@C3DMeshFileVerticesChunk@@QBEKXZ; the landed callers 0x00ACDB80 and 0x00A91640 bind it; the row now uses
// engine/C3DMeshFileVerticesChunk.h (_PEEKVERTEX opt-in: VertexList +0x10).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK_PEEKVERTEX
#include "engine/C3DMeshFileVerticesChunk.h"

unsigned long C3DMeshFileVerticesChunk::GetCount() const
{
    return VertexList.size();
}
