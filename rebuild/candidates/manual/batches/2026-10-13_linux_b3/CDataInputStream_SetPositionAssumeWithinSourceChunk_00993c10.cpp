// CDataInputStream::SetPositionAssumeWithinSourceChunk(unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993C10, oracle length 39 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[ecx+4] ; push esi ; push edi ; mov edi,[ecx+0Ch] ; mov esi,edx ; sub esi,eax ; add edi,esi ; mov esi,[ecx+14h] ; sub eax,edx ; add esi,eax ; mov [ecx+0Ch],edi ; pop edi ; mov [ecx+14h],esi ; mov [ecx+4],edx ; pop esi ; ret 4
// engine/CDataInputStream.h (PDB): +0x04 StreamPos, +0x0C CurrentSourcePtr, +0x14
// SourceChunkBytesRemaining. Seeks within the current source chunk: the source pointer moves by
// the delta, the bytes-remaining count shrinks by it, and the stream position is updated.
#include "engine/CDataInputStream.h"

struct CDataInputStream_Methods : CDataInputStream {
    void SetPositionAssumeWithinSourceChunk(unsigned long position);
};

void CDataInputStream_Methods::SetPositionAssumeWithinSourceChunk(unsigned long position)
{
    long delta = position - this->StreamPos;
    this->CurrentSourcePtr           += delta;
    this->SourceChunkBytesRemaining  -= delta;
    this->StreamPos                   = position;
}
