// CPersistTraits<bool>::TransferBinaryIn(CDataInputStream*, bool*) -- read one byte and store it as a bool (non-zero = true)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403EB0, 114 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: inline 1-byte CDataInputStream::Read into a non-bool temporary homed in the dead stream-argument slot, then
//   `*value = byte != 0` (test/setne); VC7.1 duplicates the store tail into the fast path. Speed-optimised (default) TU.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
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
    __forceinline unsigned char ReadByte() { unsigned char value; Read(&value, sizeof(value)); return value; }
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    void TransferBinaryIn(CDataInputStream* stream, bool* value);
};

void CPersistTraits<bool>::TransferBinaryIn(CDataInputStream* stream, bool* value)
{
    *value = ((CDataInputStreamReader*)stream)->ReadByte() != 0;
}