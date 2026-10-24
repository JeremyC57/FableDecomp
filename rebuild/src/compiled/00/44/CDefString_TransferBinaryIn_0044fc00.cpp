// CDefString::TransferBinaryIn(CDataInputStream&) -- binary persist read of a def string: its 4-byte table position
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044FC00, 128 bytes, `__fastcall`, `ret 4`. Callee: CDataInputStream::ReadWithSrcChunkOverflow
//   @ 0x00993CA0.
// Identity: FableWin.pdb ?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z; Ego_r.pdb CDefString = {long TablePos}.
//   The callers call it on CDefString members (`lea ecx,[esi+4] ; push stream ; call`, 0x0046AC32..) and
//   GFSerialiseMapBinaryIn<CDefString,long> (0x006DBA50) calls it on the value of its {long, CDefString} pair (whose
//   default CDefString holds -1). The catalog's `CPersistTraits<float>::TransferBinaryIn` label is propagated and wrong.
// Shape: the inlined CDataInputStream::Read (64-bit range check on `StreamPos + 4`, zero high dword in eax; fast copy
//   from CurrentSourcePtr (+0xC) with SourceChunkBytesRemaining (+0x14) and StreamPos (+4) updated, else
//   ReadWithSrcChunkOverflow) fills a local homed in the dead stream slot; the value is copied as a dword and the fast
//   path forwards the source dword straight into TablePos. Each range-check exit has its own return tail.
// The stream uses engine/CDataInputStream.h with an inline Read in a methods view.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CDefString {
public:
    long TablePos;                                                     // +0x00
    void TransferBinaryIn(CDataInputStream& stream);
};

void CDefString::TransferBinaryIn(CDataInputStream& stream)
{
    long tablePos;
    static_cast<CDataInputStreamReader&>(stream).Read(&tablePos, sizeof(tablePos));
    TablePos = tablePos;
}