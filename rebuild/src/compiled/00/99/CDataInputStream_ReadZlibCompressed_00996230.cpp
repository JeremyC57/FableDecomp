// CDataInputStream::ReadZlibCompressed(void* data, long size) -- read a block written by WriteZlibCompressed: the compressed byte count, the compressed bytes into a scratch buffer, then zlib-inflate them into data (the assertion hook fires if fewer than size bytes come out)
// Leaf: ?ReadZlibCompressed@ (Ego_r.pdb ?ReadZlibCompressed@CDataInputStream@@UAEXPAXJ@Z, virtual; modelled through CDataInputStream_Methods)
// Retail: Fable.exe @ 0x00996230, 341 bytes, `ret 8`.
// Body: size <= 0 -> nothing. Read(&compressedSize, 4); CByteArray buffer(compressedSize) (vector<unsigned char>(n)
// 0x00514E20); Read(buffer.GetData(), compressedSize); CCompressorZlib (CACompressor(GetWorkspaceSize()) 0x00A3AEA0 /
// 0x00A3A7D0, vtable 0x01245974, Init 0x00A3AA10); InternalDecompress(buffer.GetData(), compressedSize, data, size)
// (0x00A3AB00) != size -> g_pfnAssertionFailed (0x013BC9FC) if installed; ~CCompressorZlib 0x00A3ACC0; free 0x00BFEA14.
// The inline stream Read is the landed CMemoryDataInputStream::Read (64-bit position range check, chunk memcpy fast
// path, ReadWithSrcChunkOverflow 0x00993CA0).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: CByteArray / CCompressorZlib models of the landed CDataOutputStream::WriteZlibCompressed 0x009967E0, the
// FABLE_ASSERT hook of the landed CCompressedMemoryDataInputStream sources. The count comes from an inline ReadLong()
// with its own local (a caller local is homed in the dead `size` slot); Read starts with `if (size <= 0) return;` and
// tests `size <= SourceChunkBytesRemaining` (size on the left for `cmp ebp,[ebx+14h] ; jg`).
// Opt: /O2 (16-aligned entry).
#include "engine/CDataInputStream.h"

extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern void (__cdecl* g_pfnAssertionFailed)();               // 0x013BC9FC: installed debug hook

struct CACompressor {                                        // landed 0x00A3AEA0 (vtable 0x0129B424)
    virtual ~CACompressor();
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);                        // 0x00A3AEA0
};
struct CCompressorZlib : CACompressor {                      // vtable 0x01245974, retail 0x58 bytes
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();                          // 0x00A3A7D0
    void Init();                                             // 0x00A3AA10
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AB00
    virtual ~CCompressorZlib();                              // 0x00A3ACC0
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
};
struct CByteArray {                                          // retail vector<unsigned char>: begin/end/capacity
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                             // 0x00514E20
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
};

struct CDataInputStream_Methods : CDataInputStream {
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    void ReadZlibCompressed(void* data, long size);
};

void CDataInputStream_Methods::ReadZlibCompressed(void* data, long size)
{
    if (size > 0) {
        long compressedSize = ReadLong();
        CByteArray buffer(compressedSize);
        Read(buffer.GetData(), compressedSize);
        CCompressorZlib compressor;
        unsigned long written = compressor.InternalDecompress(buffer.GetData(), compressedSize, data, size);
        if (written != (unsigned long)size) {
            if (g_pfnAssertionFailed != 0)
                g_pfnAssertionFailed();
        }
    }
}