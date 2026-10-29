// CDataInputStream::ReadCompressed(void* data, long size) -- read a block written by WriteCompressed: a short compressed byte count (-1 escapes to a long), the compressed bytes into a scratch buffer, LZSS-decompress all but the last three bytes into data, then read those three raw; an uncompressed block (count <= 0) is read straight into data
// Leaf: ?ReadCompressed@
// (Ego_r.pdb ?ReadCompressed@CDataInputStream@@UAEXPAXJ@Z is virtual; modelled non-virtually through CDataInputStream_Methods as the landed ReadZlibCompressed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995FB0, length 629 bytes (+ int3 padding), `ret 8` (virtual).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed),
//          vector<unsigned char>(n) @ 0x00514E20, LZSS_Decompress(src, srcLen, dst, &dstLen, work) @ 0x00C069D0
//          (cdecl), g_pfnAssertionFailed @ 0x013BC9FC (installed debug hook), free @ 0x00BFEA14.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: the CByteArray / inline Read / ReadLong models of the landed ReadZlibCompressed 0x00996230; the short count
//   comes from an inline ReadShort() widened with movsx; the decompressed length starts as `size` and the assertion
//   fires unless it came out as size - 3.
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStream_Methods   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"

extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern void (__cdecl* g_pfnAssertionFailed)();               // 0x013BC9FC
extern "C" int __cdecl LZSS_Decompress(const unsigned char* src, unsigned int srcLen, unsigned char* dst,
                                        unsigned int* dstLen, void* work);   // 0x00C069D0

struct CByteArray {
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                             // 0x00514E20
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
};

struct CDataInputStream_Methods : CDataInputStream {
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline short ReadShort() { short value; Read(&value, sizeof(value)); return value; }
    void ReadCompressed(void* data, long size);
};

void CDataInputStream_Methods::ReadCompressed(void* data, long size)
{
    if (size > 0) {
        long compressedSize = ReadShort();
        if (compressedSize == -1)
            compressedSize = ReadLong();
        if (compressedSize > 0) {
            CByteArray buffer(compressedSize);
            Read(buffer.GetData(), compressedSize);
            unsigned int written = size;
            LZSS_Decompress(buffer.GetData(), compressedSize, (unsigned char*)data, &written, 0);
            if (written != (unsigned int)(size - 3)) {
                if (g_pfnAssertionFailed != 0)
                    g_pfnAssertionFailed();
            }
            Read((unsigned char*)data + size - 3, 3);
        } else
            Read(data, size);
    }
}