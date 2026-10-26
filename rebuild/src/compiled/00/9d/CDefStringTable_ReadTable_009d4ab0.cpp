// CDefStringTable::ReadTable(CDataInputStream&) -- read the magic (discarded), the random id, the string count and the blob size, then size the string stream, fill it from the input stream and publish the blob start (past its first word) in the global string-table pointer
// Leaf: ReadTable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4AB0, length 368 bytes (+ int3 padding), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) for the four inlined 4-byte reads,
// CMemoryDataOutputStream::ReserveSize(long) @ 0x00996BC0 (a thunk onto the +8 buffer, label unverified), CMemoryDataOutputStream::
// ReadFromInputStream(CDataInputStream&, long) @ 0x00996970 (a thunk onto 0x009964E0, label unverified), CMemoryDataOutputStream::PeekData()
// @ 0x00995E70. Global 0x013CA824: the string-table data pointer (blob + 4).
// Layout: retail CDefStringTable as the landed WriteTable (0x009D4A50): StringStream +0x0c (24 bytes), StringCount +0x24, RandomID +0x2c.
// CDataInputStream: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: four inlined ReadLong()s of the landed ReadPixelFormat (0x00994670) family; the helper's frame word lives in the dead
// argument slot (no frame of its own); the stream sub-object is addressed once (`add edi,0Ch`) through a named pointer.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {         // the engine layout plus the retail member functions
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, 4);
        return value;
    }
};

class CMemoryDataOutputStream { public:                           // 24 bytes
    unsigned char _body[0x18];
    void ReadFromInputStream(CDataInputStreamReader& in, long size);   // 0x00996970
    void* PeekData();                                      // 0x00995E70
public:
    void ReserveSize(unsigned long);   // 0x00996BC0
};
extern void* g_pStringTableData;                           // 0x013CA824

struct CDefStringTable {
    unsigned char           StringMap[0xc];                // +0x00 map<CCharString, long>
    CMemoryDataOutputStream StringStream;                  // +0x0c
    unsigned long           StringCount;                   // +0x24
    unsigned long           SharedCount;                   // +0x28
    unsigned long           RandomID;                      // +0x2c
    void ReadTable(CDataInputStreamReader& in);
};

void CDefStringTable::ReadTable(CDataInputStreamReader& in)
{
    in.ReadLong();
    RandomID = in.ReadLong();
    StringCount = in.ReadLong();
    long size = in.ReadLong();
    CMemoryDataOutputStream* data = &StringStream;
    data->ReserveSize(size);
    data->ReadFromInputStream(in, size);
    g_pStringTableData = (unsigned char*)data->PeekData() + 4;
}