// CDataInputStream::CDataInputStream(void* source, long size, long chunkBytesRemaining) -- CBase(), then the stream positioned at the start of a memory source
// Leaf: ??0CDataInputStream@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993B50, length 47 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x0129A6B0 = CDataInputStream (rtti-vtables.tsv).
// Shape: the two zeroed positions share the `xor eax,eax` register; the three arguments are loaded into
// ecx/edx/eax before the vtable store and written after it (the scheduler interleaves the copies). Layout
// from engine/CDataInputStream.h (PDB): StreamPos +4, StreamSize +8, CurrentSourcePtr +0xc,
// SourceChunkStreamPos +0x10, SourceChunkBytesRemaining +0x14.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                   // 0x0099A2F0
};

class CDataInputStream : public CBaseClassNonCopyable { public:              // vtable 0x0129A6B0; same layout as engine/CDataInputStream.h
    unsigned long StreamPos;                   // +0x04
    long          StreamSize;                  // +0x08
    void*         CurrentSourcePtr;            // +0x0c
    unsigned long SourceChunkStreamPos;        // +0x10
    long          SourceChunkBytesRemaining;   // +0x14
    CDataInputStream(void* source, long size, long chunkBytesRemaining);
};

CDataInputStream::CDataInputStream(void* source, long size, long chunkBytesRemaining)
    : CBaseClassNonCopyable(), StreamPos(0), StreamSize(size), CurrentSourcePtr(source), SourceChunkStreamPos(0),
      SourceChunkBytesRemaining(chunkBytesRemaining)
{
}