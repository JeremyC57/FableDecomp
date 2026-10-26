// CDataInputStream::ReadDateAndTime() -- read a CDateAndTime (long year, then four bytes and two words) from the stream and return it by value
// Leaf: ReadDateAndTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995770, length 629 bytes (+ int3 padding), `ret 4` (the hidden result pointer).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) when the current source
//          chunk is short, CDateAndTime::CDateAndTime(long, ulong, ulong, ulong, ulong, ulong, ulong) @ 0x009E9A70.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: seven inlined Read(buffer, size) calls of the landed ReadColour 0x009945E0 family (64-bit position range check,
//   intrinsic memcpy, `+=` bookkeeping); each byte/word field is read through an inline value reader into one shared
//   frame word and widened into its own unsigned long local; the result is constructed straight into the hidden pointer.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDateAndTime { public:
    unsigned char _data[0x1c];
    CDateAndTime(long year, unsigned long month, unsigned long day, unsigned long hour, unsigned long minute,
                 unsigned long second, unsigned long milliseconds);   // 0x009E9A70
};

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
    template <class T> __forceinline T ReadValue(T*)
    {
        T value;
        Read(&value, sizeof(T));
        return value;
    }
    CDateAndTime ReadDateAndTime();
};

CDateAndTime CDataInputStreamReader::ReadDateAndTime()
{
    long year = ReadValue((long*)0);
    unsigned long month = ReadValue((unsigned char*)0);
    unsigned long day = ReadValue((unsigned char*)0);
    unsigned long hour = ReadValue((unsigned char*)0);
    unsigned long minute = ReadValue((unsigned char*)0);
    unsigned long second = ReadValue((unsigned short*)0);
    unsigned long milliseconds = ReadValue((unsigned short*)0);
    return CDateAndTime(year, month, day, hour, minute, second, milliseconds);
}