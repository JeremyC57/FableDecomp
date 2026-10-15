// CCompressedMemoryDataInputStream::SetPosition(unsigned long position) -- seek inside the current decompressed chunk directly; seek forward past it by inflating and discarding, then hand the position to CDataInputStream::SetPosition
// Grade: DIFFER(138v141) -- register colouring: retail keeps `position` in edi (saved in the prologue) and the chunk size in ebp (saved only on the inflate path); the lane swaps them (ebp saved up front), forms the chunk end with `mov eax,[14h] ; add eax,edx` instead of `mov edx,[14h] ; lea eax,[edx+ecx]`, and folds the source-pointer advance as `ptr - StreamPos + position`. Tried: plain body, locals in store order, declaration-order swap, inlined InternalReadUnbuffered (fixed the push order).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00994A80, 141 bytes (144 with padding). Vtable 0x0129A758 slot 1.
// Callees: GetPosition (vtable slot 2, `call [eax+8]`), CCompressorZlib::DecompressStream(void*, ulong, bool*)
//          0x00A3A840 on the owned compressor, the assertion hook through 0x013BC9FC,
//          CDataInputStream::SetPosition 0x00993BC0 (base call, non-virtual).
//
// Shape: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xc, SourceChunkStreamPos +0x10,
// SourceChunkBytesRemaining +0x14) and engine/CCompressedMemoryDataInputStream.h (Buffer +0x18,
// BufferSize +0x1c, Compressor_ptr +0x20); both headers declare the vptr as a plain `void*`, so the
// class is modelled polymorphically here with the same names and its size is checked against the
// header (as the landed InternalReadUnbuffered 0x00994A40). A target inside the current chunk only
// moves the source pointer (no CurrentSourcePtr null test, unlike the base). A target behind the
// current position is ignored; a forward one inflates min(BufferSize, distance) bytes at a time into
// the scratch buffer (the end-of-stream flag lives in the dead `position` argument slot, as position
// is kept in edi), asserting each full read, then the base re-synchronises the chunk state.
#include "engine/CCompressedMemoryDataInputStream.h"

struct CCompressorZlib_Calls {
    unsigned long DecompressStream(void* dest, unsigned long destSize, bool* endOfStream);   // 0x00A3A840
};

extern void (__cdecl* g_pfnAssertionFailed)();                  // 0x013BC9FC: installed debug hook
#define FABLE_ASSERT(cond) do { if (!(cond)) { if (g_pfnAssertionFailed != 0) g_pfnAssertionFailed(); } } while (0)

struct CDataInputStream_Retail {
    virtual ~CDataInputStream_Retail();                         // slot 0
    virtual void SetPosition(unsigned long position);           // slot 1 (base 0x00993BC0)
    virtual unsigned long GetPosition();                        // slot 2
    unsigned long  StreamPos;                                   // +0x04
    long           StreamSize;                                  // +0x08
    unsigned char* CurrentSourcePtr;                            // +0x0c
    unsigned long  SourceChunkStreamPos;                        // +0x10
    long           SourceChunkBytesRemaining;                   // +0x14
};

struct CCompressedMemoryDataInputStream_Retail : CDataInputStream_Retail {
    unsigned char*         Buffer;                              // +0x18
    long                   BufferSize;                          // +0x1c
    CCompressorZlib_Calls* Compressor_ptr;                      // +0x20 scoped_ptr<CCompressorZlib>
    virtual void SetPosition(unsigned long position);
    virtual void InternalReadUnbuffered(unsigned char* dest, long size)   // slot 10 (0x00994A40), inlined here
    {
        bool endOfStream = false;
        unsigned long bytesProduced = Compressor_ptr->DecompressStream(dest, size, &endOfStream);
        FABLE_ASSERT(bytesProduced == (unsigned long)size);
    }
};

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

void CCompressedMemoryDataInputStream_Retail::SetPosition(unsigned long position)
{
    unsigned long chunk;
    unsigned long current = GetPosition();
    if (position == current)
        return;
    if (position >= SourceChunkStreamPos) {
        unsigned long chunkEnd = SourceChunkBytesRemaining + StreamPos;
        if (position <= chunkEnd) {
            unsigned char* source = CurrentSourcePtr + (position - StreamPos);
            long remaining = chunkEnd - position;
            StreamPos = position;
            CurrentSourcePtr = source;
            SourceChunkBytesRemaining = remaining;
            return;
        }
    }
    if (position <= current)
        return;
    do {
        chunk = BufferSize;
        unsigned long left = position - current;
        if (chunk >= left)
            chunk = left;
        CCompressedMemoryDataInputStream_Retail::InternalReadUnbuffered(Buffer, chunk);
        current += chunk;
    } while (position > current);
    CDataInputStream_Retail::SetPosition(position);
}
