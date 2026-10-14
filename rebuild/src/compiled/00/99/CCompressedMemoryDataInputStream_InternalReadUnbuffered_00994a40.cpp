// CCompressedMemoryDataInputStream::InternalReadUnbuffered(unsigned char*, long) -- inflate straight into the caller's buffer and assert that the whole request was produced
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994A40, length 50 bytes (+ int3 padding).
// Callees: CCompressorZlib::DecompressStream(void*, ulong, bool*) @ 0x00A3A840 on the owned compressor
// (Compressor_ptr at +0x20 of engine/CCompressedMemoryDataInputStream.h), and the assertion hook at
// 0x013BC9FC (called when installed and the byte count differs from the request). The end-of-stream
// flag is a 1-byte local in the high byte of the frame slot (`lea eax,[esp+7]`), cleared after the
// pushes; the requested size stays in esi for the compare. Decorated name in the manifest:
// ?InternalReadUnbuffered@CCompressedMemoryDataInputStream@@EAEXPAEJ@Z (private, void, uchar*, long).
#include "engine/CCompressedMemoryDataInputStream.h"

struct CCompressorZlib_Calls {
    unsigned long DecompressStream(void* dest, unsigned long destSize, bool* endOfStream);   // 0x00A3A840
};

extern void (__cdecl* g_pfnAssertionFailed)();                  // 0x013BC9FC: installed debug hook
#define FABLE_ASSERT(cond) do { if (!(cond)) { if (g_pfnAssertionFailed != 0) g_pfnAssertionFailed(); } } while (0)

struct CCompressedMemoryDataInputStream_Retail {
    virtual void v00();
    unsigned char          _state[0x14];                        // +0x04..+0x17 (CDataStream)
    unsigned char*         Buffer;                              // +0x18
    long                   BufferSize;                          // +0x1c
    CCompressorZlib_Calls* Compressor_ptr;                      // +0x20
    void InternalReadUnbuffered(unsigned char* dest, long size);
};

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

void CCompressedMemoryDataInputStream_Retail::InternalReadUnbuffered(unsigned char* dest, long size)
{
    bool endOfStream = false;
    unsigned long bytesProduced = Compressor_ptr->DecompressStream(dest, size, &endOfStream);
    FABLE_ASSERT(bytesProduced == (unsigned long)size);
}