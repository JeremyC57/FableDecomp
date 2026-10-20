// CDefClassBase::InternalLoadBinary(CDataInputStream& in) -- read the three definition flags (Setup, Template, DefaultValsApplied) from the binary stream, one byte each
// Leaf: InternalLoadBinary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ACB20, length 292 bytes (`ret 4`, virtual `?InternalLoadBinary@CDefClassBase@@UAEXAAVCDataInputStream@@@Z`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (the slow path of each inlined Read).
// Layout: engine/CDefClassBase.h (PDefManager +0x1c, GlobalIndex +0x20) flattens the +0x24 bit-field group into one byte; the
// PDB bits are Setup (bit 0), Template (bit 1), DefaultValsApplied (bit 2), so `this` is modelled locally with real bit-fields.
// The stream is the CDataInputStream model of the landed CPSCOrbit::ReadBinary 0x00B16F60 (64-bit position range check,
// direct copy while the source chunk has room, else ReadWithSrcChunkOverflow).
// Shape: each flag is read into its OWN block-scoped `bool` (the three share the dead stream-argument slot; one reused local or
// three function-scope locals give a `push ecx` frame and extra stores); the value is read back on the slow path and
// left as is when the range check fails; each flag store is the `xor ; and ; xor` bit-field merge.
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

struct CDefinitionManager;

struct CDefClassBase {
    unsigned char       _base_0x04[0x18];           // +0x04 (vptr at +0x00)
    CDefinitionManager* PDefManager;                // +0x1c
    unsigned long       GlobalIndex;                // +0x20
    unsigned char       Setup : 1;                  // +0x24 bit 0
    unsigned char       Template : 1;               // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;     // +0x24 bit 2
    virtual void InternalLoadBinary(CDataInputStream& in);
};

void CDefClassBase::InternalLoadBinary(CDataInputStream& in)
{
    {
        bool setup;
        in.Read(&setup, 1);
        Setup = setup;
    }
    {
        bool isTemplate;
        in.Read(&isTemplate, 1);
        Template = isTemplate;
    }
    {
        bool applied;
        in.Read(&applied, 1);
        DefaultValsApplied = applied;
    }
}