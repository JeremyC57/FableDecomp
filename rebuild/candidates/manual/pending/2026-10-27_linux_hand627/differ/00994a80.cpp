// CCompressedMemoryDataInputStream::SetPosition(unsigned long position) -- seek inside the current decompressed chunk directly; seek forward past it by inflating and discarding, then hand the position to CDataInputStream::SetPosition
// Grade: DIFFER(142v141) -- register colouring only: retail keeps `position` in edi (saved at entry) and the chunk size in ebp (pushed on the inflate path), the advanced source pointer in the dead `current` register ebx, and stores StreamPos before CurrentSourcePtr; VC7.1 here saves ebp at entry for `position`, shrink-wraps edi for the chunk/pointer and stores CurrentSourcePtr first (one byte longer)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00994A80, 141 bytes, vtable 0x0129A758 slot 1, `ret 4`. PDB ?SetPosition@CCompressedMemoryDataInputStream@@UAEXK@Z.
// Callees: GetPosition (slot 2, ?GetPosition@CDataInputStream@@UBEKXZ), CCompressorZlib::DecompressStream(void*, unsigned long, bool&)
//   0x00A3A840 (Ego_r.pdb ?DecompressStream@CCompressorZlib@@QAEKPAXKAA_N@Z -- a bool REFERENCE, the landed spelling's bool* is wrong),
//   the assertion hook g_pfnAssertionFailed 0x013BC9FC, CDataInputStream::SetPosition 0x00993BC0 (qualified base call; PDB UAEXK, virtual).
// Shape: the in-chunk seek is the PDB's protected inlines CDataInputStream::CanSeekToPosWithinSourceChunk(long) +
//   SetPositionAssumeWithinSourceChunk(unsigned long) (same TU, both in the Ego_r name set): with them the chunk end is
//   retail's `mov ecx,[4] ; mov edx,[14h] ; lea eax,[edx+ecx]` (the earlier explicit form gave `add eax,edx`). The forward
//   skip inflates min(BufferSize, distance) bytes at a time through the inlined InternalReadUnbuffered (end-of-stream flag
//   in the dead `position` argument slot). Tried this round: helper inlines (this file), explicit store order with an
//   old-position local, a rotated while loop with an early `position < current` return, a skip-size local, one nested
//   if/else body; local/parameter names (no effect at /O2). Next: the bool-expression lever of 0x007723F0 does not apply
//   (void), try `if (position > current) { ...; CDataInputStream::SetPosition(position); }` written as the only exit
//   with the in-chunk branch as `else if`, or the loop's InternalReadUnbuffered as a same-TU noinline stand-in.
// Leaf: ?SetPosition@CCompressedMemoryDataInputStream@@UAEXK@Z
#define CDataInputStream CDataInputStreamLayout
#include "engine/CDataInputStream.h"
#undef CDataInputStream
#define CCompressedMemoryDataInputStream CCompressedMemoryDataInputStreamLayout
#include "engine/CCompressedMemoryDataInputStream.h"
#undef CCompressedMemoryDataInputStream

class CCompressorZlib { public:
    unsigned long DecompressStream(void* dest, unsigned long destSize, bool& endOfStream);   // 0x00A3A840
};

extern void (__cdecl* g_pfnAssertionFailed)();                  // 0x013BC9FC: installed debug hook
#define FABLE_ASSERT(cond) do { if (!(cond)) { if (g_pfnAssertionFailed != 0) g_pfnAssertionFailed(); } } while (0)

#include "engine/CBaseClass.h"
struct CBaseClassNonCopyable : CBaseClass { virtual ~CBaseClassNonCopyable(); };
class CDataStream : public CBaseClassNonCopyable { public:
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long position) = 0;   // slot 1 (+4)
    virtual unsigned long GetPosition() const = 0;          // slot 2 (+8)
};
class CDataInputStream : public CDataStream { public:
    unsigned long  StreamPos;                 // +0x04
    long           StreamSize;                // +0x08
    unsigned char* CurrentSourcePtr;          // +0x0c
    unsigned long  SourceChunkStreamPos;      // +0x10
    long           SourceChunkBytesRemaining; // +0x14
    virtual void SetPosition(unsigned long position);       // 0x00993BC0
protected:
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
class CCompressedMemoryDataInputStream : public CDataInputStream { public:
    unsigned char*   Buffer;                  // +0x18
    long             BufferSize;              // +0x1c
    CCompressorZlib* Compressor_ptr;          // +0x20 scoped_ptr<CCompressorZlib>
    virtual void SetPosition(unsigned long position);
private:
    void InternalReadUnbuffered(unsigned char* dest, long size)   // 0x00994A40
    {
        bool endOfStream = false;
        unsigned long bytesProduced = Compressor_ptr->DecompressStream(dest, size, endOfStream);
        FABLE_ASSERT(bytesProduced == (unsigned long)size);
    }
};

void CCompressedMemoryDataInputStream::SetPosition(unsigned long position)
{
    unsigned long current = GetPosition();
    if (position == current)
        return;
    if (CanSeekToPosWithinSourceChunk(position)) {
        SetPositionAssumeWithinSourceChunk(position);
        return;
    }
    if (position > current) {
        do {
            unsigned long chunk = BufferSize;
            if (chunk >= position - current)
                chunk = position - current;
            InternalReadUnbuffered(Buffer, chunk);
            current += chunk;
        } while (position > current);
        CDataInputStream::SetPosition(position);
    }
}
