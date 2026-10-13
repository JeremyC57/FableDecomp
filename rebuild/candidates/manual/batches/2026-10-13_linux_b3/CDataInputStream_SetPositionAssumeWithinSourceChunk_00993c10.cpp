// CDataInputStream::SetPositionAssumeWithinSourceChunk(unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993C10, oracle length 39 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[ecx+4] ; push esi ; push edi ; mov edi,[ecx+0Ch] ; mov esi,edx ; sub esi,eax ; add edi,esi ; mov esi,[ecx+14h] ; sub eax,edx ; add esi,eax ; mov [ecx+0Ch],edi ; pop edi ; mov [ecx+14h],esi ; mov [ecx+4],edx ; pop esi ; ret 4
// engine/CDataInputStream.h (PDB): +0x04 StreamPos, +0x0C CurrentSourcePtr, +0x14
// SourceChunkBytesRemaining. Seeks within the current source chunk: the source pointer moves by
// the delta, the bytes-remaining count shrinks by it, and the stream position is updated.
//
// Lane feedback: SRC_FAIL was pointer arithmetic on the header's `void*`; cast to unsigned char*. The retail
// body computes the forward and backward deltas separately (no shared local). MATCH.
#include "engine/CDataInputStream.h"

struct CDataInputStream_Methods : CDataInputStream {
    void SetPositionAssumeWithinSourceChunk(unsigned long position);
};

void CDataInputStream_Methods::SetPositionAssumeWithinSourceChunk(unsigned long position)
{
    // Two independently computed deltas (retail does not share `position - StreamPos` between
    // the pointer advance and the bytes-remaining update; a common `delta` local is 14 bytes shorter).
    this->CurrentSourcePtr           = (unsigned char*)this->CurrentSourcePtr + (position - this->StreamPos);
    this->SourceChunkBytesRemaining += this->StreamPos - position;
    this->StreamPos                  = position;
}
