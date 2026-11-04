// CCompressedMemoryDataInputStream::SetPosition(unsigned long position) -- seek inside the current decompressed chunk directly; seek forward past it by inflating and discarding, then hand the position to CDataInputStream::SetPosition
// Grade: DIFFER(ours v retail) -- DIFFER(141v141) (try j): on the Ego_r.pdb protected inline helpers CanSeekToPosWithinSourceChunk / SetPositionAssumeWithinSourceChunk (same-TU bodies; they fix the chunk-end CSE, the store order and the edi/ebp colouring) only the inlined InternalReadUnbuffered argument setup differs: retail loads Buffer (edx) and the compressor (ecx) before `lea eax,[eos]` and pushes eax/ebp/edx; ours loads Buffer into eax, pushes &eos through ecx first and loads the compressor after it. A scoped_ptr Compressor (operator->) fixes that order but swaps the position/chunk registers (142v141).
// Flags: none
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
    unsigned long DecompressStream(void* dest, unsigned long destSize, bool& endOfStream);   // 0x00A3A840
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
    bool CanSeekToPosWithinSourceChunk(long position)
    {
        return (unsigned long)position >= SourceChunkStreamPos && (unsigned long)position <= StreamPos + SourceChunkBytesRemaining;
    }
    void SetPositionAssumeWithinSourceChunk(unsigned long position)
    {
        CurrentSourcePtr += position - StreamPos;
        SourceChunkBytesRemaining += StreamPos - position;
        StreamPos = position;
    }
};

struct CCompressedMemoryDataInputStream_Retail : CDataInputStream_Retail {
    unsigned char*         Buffer;                              // +0x18
    long                   BufferSize;                          // +0x1c
    CCompressorZlib_Calls* Compressor_ptr;                      // +0x20 scoped_ptr<CCompressorZlib>
    virtual void SetPosition(unsigned long position);
    virtual void InternalReadUnbuffered(unsigned char* dest, long size)   // slot 10 (0x00994A40), inlined here
    {
        bool endOfStream = false;
        unsigned long bytesProduced = Compressor_ptr->DecompressStream(dest, size, endOfStream);
        FABLE_ASSERT(bytesProduced == (unsigned long)size);
    }
};

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

void CCompressedMemoryDataInputStream_Retail::SetPosition(unsigned long position)
{
    unsigned long current = GetPosition();
    if (position == current)
        return;
    if (CanSeekToPosWithinSourceChunk(position)) {
        SetPositionAssumeWithinSourceChunk(position);
        return;
    }
    if (position > current) {
        while (position > current) {
            unsigned long chunk = (unsigned long)BufferSize < position - current ? BufferSize : position - current;
            CCompressedMemoryDataInputStream_Retail::InternalReadUnbuffered(Buffer, chunk);
            current += chunk;
        }
        CDataInputStream_Retail::SetPosition(position);
    }
}
