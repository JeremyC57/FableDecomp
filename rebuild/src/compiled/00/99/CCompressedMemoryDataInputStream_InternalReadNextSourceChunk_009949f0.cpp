// CCompressedMemoryDataInputStream::InternalReadNextSourceChunk(unsigned char**, unsigned long*) -- inflate the next chunk into the stream's buffer and hand back the buffer and the byte count (asserting that something was produced)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009949F0, length 69 bytes (+ int3 padding).
// Callees: CCompressorZlib::DecompressStream(void*, ulong, bool*) @ 0x00A3A840 on the owned compressor
// (Compressor_ptr at +0x20 of engine/CCompressedMemoryDataInputStream.h, a scoped_ptr), and the
// assertion hook: the function pointer at 0x013BC9FC is called when the assertion fails and the hook
// is installed. Buffer +0x18 / BufferSize +0x1c as the header; the end-of-stream flag is a 1-byte
// local homed in the high byte of the frame slot (`lea eax,[esp+0Bh]`) whose clear is scheduled after
// the pushes. The byte count is kept in edi across the hook call; `n > 0` is the unsigned compare
// (`test ; ja`). The manifest's doubled "CCompressedMemoryDataInputStream::CCompressedMemory
// DataInputStream::" prefix is a naming artefact; vtable 0x0129A758 slot 8 is this body.
#include "engine/CCompressedMemoryDataInputStream.h"

struct CCompressorZlib_Calls {
    unsigned long DecompressStream(void* dest, unsigned long destSize, bool* endOfStream);   // 0x00A3A840
};

template<class T> struct CScopedPointer {                       // scoped_ptr<T>: one owning pointer
    T* ptr;
    T* operator->() const { return ptr; }
};

extern void (__cdecl* g_pfnAssertionFailed)();                  // 0x013BC9FC: installed debug hook
#define FABLE_ASSERT(cond) do { if (!(cond)) { if (g_pfnAssertionFailed != 0) g_pfnAssertionFailed(); } } while (0)

struct CCompressedMemoryDataInputStream_Retail {
    virtual void v00();
    unsigned char          _state[0x14];                        // +0x04..+0x17 (CDataStream)
    unsigned char*         Buffer;                              // +0x18
    long                   BufferSize;                          // +0x1c
    CScopedPointer<CCompressorZlib_Calls> Compressor;           // +0x20 (scoped_ptr: ptr)
    void InternalReadNextSourceChunk(unsigned char** chunkData, unsigned long* chunkSize);
};

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

void CCompressedMemoryDataInputStream_Retail::InternalReadNextSourceChunk(unsigned char** chunkData, unsigned long* chunkSize)
{
    bool endOfStream = false;
    unsigned long bytesProduced = Compressor->DecompressStream(Buffer, BufferSize, &endOfStream);
    FABLE_ASSERT(bytesProduced > 0);
    *chunkData = Buffer;
    *chunkSize = bytesProduced;
}