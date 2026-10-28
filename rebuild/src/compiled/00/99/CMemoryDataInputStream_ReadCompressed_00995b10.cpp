// CMemoryDataInputStream::ReadCompressed(void* data, long size) -- the in-memory override of CDataInputStream::ReadCompressed: a short compressed byte count (-1 escapes to a long), then LZSS-decompress straight out of the source buffer (skipping the compressed bytes) into all but the last three bytes of data, and read those three raw; an uncompressed block (count <= 0) is read straight into data
// Leaf: ?ReadCompressed@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00995B10, length 544 bytes (+ int3 padding), `ret 8` (virtual).
// Callees: virtual GetPosition (slot 2, +8), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   LZSS_Decompress(src, srcLen, dst, &dstLen, work) 0x00C069D0 (cdecl), g_pfnAssertionFailed 0x013BC9FC (debug hook).
// Layout: engine/CMemoryDataInputStream.h (+4 position, +0xc chunk pointer, +0x14 chunk bytes left, +0x18 StreamSource);
//   the header is flattened (`void* __vftable`), so a local polymorphic model carries the GetPosition slot.
// Shape: the landed CDataInputStream::ReadCompressed 0x00995FB0 with its scratch vector replaced by an inline
//   GetDataAndSkip(n): read the position, range-check the new one, seek within the chunk (the landed
//   SetPositionAssumeWithinSourceChunk 0x00993C10 body, two separate deltas) and return StreamSource + old position, or 0.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern void (__cdecl* g_pfnAssertionFailed)();               // 0x013BC9FC
extern "C" int __cdecl LZSS_Decompress(const unsigned char* src, unsigned int srcLen, unsigned char* dst,
                                        unsigned int* dstLen, void* work);   // 0x00C069D0

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetPosition();                     // slot 2 (+8)
    unsigned long        StreamPos;                          // +0x04
    long                 StreamSize;                         // +0x08
    const unsigned char* CurrentSourcePtr;                   // +0x0c
    unsigned long        SourceChunkStreamPos;               // +0x10
    long                 SourceChunkBytesRemaining;          // +0x14
    const unsigned char* StreamSource;                       // +0x18

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
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline short ReadShort() { short value; Read(&value, sizeof(value)); return value; }
    __forceinline void SetPositionAssumeWithinSourceChunk(unsigned long position)
    {
        CurrentSourcePtr += position - StreamPos;
        SourceChunkBytesRemaining += StreamPos - position;
        StreamPos = position;
    }
    __forceinline const unsigned char* GetDataAndSkip(long bytes)
    {
        unsigned long position = GetPosition();
        __int64 newPosition = (__int64)(position + bytes);
        if (IsValidPosition(newPosition)) {
            SetPositionAssumeWithinSourceChunk((unsigned long)newPosition);
            return StreamSource + position;
        }
        return 0;
    }
    virtual void ReadCompressed(void* data, long size);
};

void CMemoryDataInputStream::ReadCompressed(void* data, long size)
{
    if (size > 0) {
        long compressedSize = ReadShort();
        if (compressedSize == -1)
            compressedSize = ReadLong();
        if (compressedSize > 0) {
            const unsigned char* source = GetDataAndSkip(compressedSize);
            unsigned int written = size;
            LZSS_Decompress(source, compressedSize, (unsigned char*)data, &written, 0);
            if (written != (unsigned int)(size - 3)) {
                if (g_pfnAssertionFailed != 0)
                    g_pfnAssertionFailed();
            }
            Read((unsigned char*)data + size - 3, 3);
        } else
            Read(data, size);
    }
}