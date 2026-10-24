// CPersistTraits<double>::TransferBinaryIn(CMemoryDataInputStream*, double*) -- binary persist read of a 4-byte integer widened to double
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AA360, 109 bytes. Callee: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Shape: the empty traits object is `this` (ecx, never read); the stream and the output arrive on the
// stack (`ret 8`). The inlined CMemoryDataInputStream::Read widens `position + 4` to __int64 (the
// zero high dword is materialised in eax) and rejects it outside [0, 0x7FFFFFFF]; with 4 bytes
// left in the current chunk it copies the dword from the chunk pointer (+0xC) into the local
// `value` (homed in the dead stream-argument slot), else it defers to ReadWithSrcChunkOverflow.
// The value read is a 4-byte INTEGER (`fild dword`) stored as a double (`fstp qword`): the double
// traits persist the value as a long. Every exit (range fail, overflow read, fast path) ends in
// `fild value ; fstp [out]`.
// The stream uses engine/CMemoryDataInputStream.h with an inline Read (the header's `position` is
// long; the sum is taken unsigned, matching retail's zero high dword: xor eax,eax, not cdq).
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)((unsigned long)position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};


template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, T* out) const;
};

template <> void CPersistTraits<double>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, double* out) const
{
    long value;
    stream->Read(&value, sizeof(value));
    *out = value;
}