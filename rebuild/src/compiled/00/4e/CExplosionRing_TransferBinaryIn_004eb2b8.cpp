// CExplosionRing::TransferBinaryIn(CDataInputStream* stream) -- read the ring back in the TransferBinaryOut order: NumExplosions (+4), then RingRadius (+0), AngleOffset (+8) and SecondsBeforeSpawnNewRing (+0xc)
// Leaf: TransferBinaryIn@CExplosionRing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EB2B8, length 359 bytes.
// Pragma: #pragma optimize("s", on) (unaligned entry; ebp frame with `this` spilled to [ebp-4]).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the slow path of the inlined Read).
// Identity: the landed mirror CExplosionRing::TransferBinaryOut @ 0x004EB1D9 writes the count word (+4) first, then the
//   three floats +0/+8/+0xc. Layout: Ego_r.pdb CExplosionRing (16 bytes) RingRadius +0, NumExplosions +4,
//   AngleOffset +8, SecondsBeforeSpawnNewRing +0xc (no generated engine header).
// Shape: the inlined CMemoryDataInputStream::Read of the landed CReactionMatch_MRPyramid::TransferBinaryIn 0x006FF810
//   (64-bit position range check, chunk fast path copying one dword with movsd, overflow reader) four times into one
//   local homed in the dead stream-argument slot. The reads are by-value `ReadLong()`/`ReadFloat()` inline helpers
//   (a shared address-taken local in the body puts `this` in eax for the first store; the helpers keep it in ecx).
#pragma optimize("s", on)

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long v; Read(&v, sizeof(v)); return v; }
    __forceinline float ReadFloat() { float v; Read(&v, sizeof(v)); return v; }
};

class CExplosionRing { public:
    float RingRadius;                      // +0x00
    long  NumExplosions;                   // +0x04
    float AngleOffset;                     // +0x08
    float SecondsBeforeSpawnNewRing;       // +0x0c
    void TransferBinaryIn(CDataInputStream* stream);
};

void CExplosionRing::TransferBinaryIn(CDataInputStream* stream)
{
    NumExplosions = stream->ReadLong();
    RingRadius = stream->ReadFloat();
    AngleOffset = stream->ReadFloat();
    SecondsBeforeSpawnNewRing = stream->ReadFloat();
}