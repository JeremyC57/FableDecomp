// CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStream*, unsigned __int64*) -- binary persist read of one 8-byte value
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044B890, 121 bytes, `__fastcall` (the empty traits object in ecx is never read), `ret 8`.
// Callee: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Shape: the inlined CMemoryDataInputStream::Read widens `position + size` to __int64 (zero high dword materialised in
// eax) and rejects it outside [0, 0x7FFFFFFF]; with enough bytes left in the current chunk it copies from the chunk
// pointer (+0xC) into the local value, else ReadWithSrcChunkOverflow.
// The 8-byte value gets its own frame (`sub esp,8`); the fast path copies both dwords into it, and every exit shares
// one tail that copies the local to the output.
// The stream uses engine/CMemoryDataInputStream.h with an inline Read (the header's `position` is long; the sum is
// taken unsigned, matching retail's zero high dword: xor eax,eax, not cdq).
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
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, T* out) const;
};

template <> void CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, unsigned __int64* out) const
{
    unsigned __int64 value;
    stream->Read(&value, sizeof(value));
    *out = value;
}