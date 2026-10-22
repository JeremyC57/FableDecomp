// CGlyph::Read(CDataInputStream* in) -- read one font glyph: the four texture-rectangle floats (left, top, right, bottom) then the offset, width and advance shorts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB96B0, length 656 bytes (`ret 4`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the slow path of each inlined read).
// Shape: seven inlined CDataInputStream::Read calls (the landed NParticleEngine::CPSCOrbit::ReadBinary 0x00B16F60 model:
//   64-bit position range check, direct copy while the source chunk has room, else ReadWithSrcChunkOverflow), each into a
//   block-scoped value local (a function-scope one gets its own frame word) that VC homes in the dead `in` argument slot ([esp+10h]) and then copies into the member through an
//   integer register. The last read's four exits each get their own epilogue.
// Layout: Ego_r.pdb CGlyph (0x18): Left +0, Top +4, Right +8, Bottom +0xc, Offset +0x10, Width +0x12, Advance +0x14.
//   Local stream model: engine/CDataInputStream.h is data-only and cannot carry the inline Read.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};

struct CGlyph {
    float Left;                                         // +0x00
    float Top;                                          // +0x04
    float Right;                                        // +0x08
    float Bottom;                                       // +0x0c
    short Offset;                                       // +0x10
    short Width;                                        // +0x12
    short Advance;                                      // +0x14
    void Read(CDataInputStream* in);
};

void CGlyph::Read(CDataInputStream* in)
{
    { float f; in->Read(&f, sizeof(f)); Left = f; }
    { float f; in->Read(&f, sizeof(f)); Top = f; }
    { float f; in->Read(&f, sizeof(f)); Right = f; }
    { float f; in->Read(&f, sizeof(f)); Bottom = f; }
    { short s; in->Read(&s, sizeof(s)); Offset = s; }
    { short s; in->Read(&s, sizeof(s)); Width = s; }
    { short s; in->Read(&s, sizeof(s)); Advance = s; }
}