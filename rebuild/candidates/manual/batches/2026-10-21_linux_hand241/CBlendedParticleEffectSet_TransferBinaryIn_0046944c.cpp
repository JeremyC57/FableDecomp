// CBlendedParticleEffectSet::TransferBinaryIn(CDataInputStream& in) -- read the two particle indices then the two blend values (the mirror of the landed TransferBinaryOut 0x00469345)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046944C, length 359 bytes (`ret 4`); size-optimised def TU (ebp frame,
//          `leave`), the same TU as the landed TransferBinaryOut.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x4, the slow path of the inlined Read).
// Layout: Ego_r.pdb CBlendedParticleEffectSet (16 bytes): Particle1Index +0, Particle1Value +4, Particle2Index +8,
//          Particle2Value +0xc. CDataInputStream per the landed 0x006FF810 / 0x00B16F60 model.
// Shape: four inlined CDataInputStream::Read calls (64-bit position range check, chunk fast path with `movsd`, overflow
//          reader) into one 4-byte local homed in the dead stream-argument slot (the float members take its bits), each followed by a store to its member;
//          `this` is spilled to [ebp-4].
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

struct CBlendedParticleEffectSet {
    long  Particle1Index;                       // +0x00
    float Particle1Value;                       // +0x04
    long  Particle2Index;                       // +0x08
    float Particle2Value;                       // +0x0c
    void TransferBinaryIn(CDataInputStream& in);
};

void CBlendedParticleEffectSet::TransferBinaryIn(CDataInputStream& in)
{
    Particle1Index = in.ReadLong();
    Particle2Index = in.ReadLong();
    Particle1Value = in.ReadFloat();
    Particle2Value = in.ReadFloat();
}
