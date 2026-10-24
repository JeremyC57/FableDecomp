// CPersistTraits<short>::TransferBinaryIn(CMemoryDataInputStream*, short*) -- binary persist read of a 4-byte value narrowed to short
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CBC70, 136 bytes (+8 int3 padding), `__fastcall` (the empty traits object in ecx is never read), `ret 8`.
// Callee: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Shape: the inlined CMemoryDataInputStream::Read widens `position + size` to __int64 (zero high dword materialised in
// eax) and rejects it outside [0, 0x7FFFFFFF]; with enough bytes left in the current chunk it copies from the chunk
// pointer (+0xC) into the local value (homed in the dead stream-argument slot), else ReadWithSrcChunkOverflow.
// The value is persisted as a 4-byte long and narrowed on the store (`mov word ptr [out],di`); each range-check exit
// gets its own `mov reg16,[value] ; mov [out],reg16` return tail.
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

template <> void CPersistTraits<short>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, short* out) const
{
    long value;
    stream->Read(&value, sizeof(value));
    *out = (short)value;
}