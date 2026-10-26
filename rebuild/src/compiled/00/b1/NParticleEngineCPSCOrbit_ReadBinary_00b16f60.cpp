// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test hand-transcribed with renamed types)
// NParticleEngine::CPSCOrbit::ReadBinary(CDataInputStream*) -- read the centre parameter, then the three orbit-setting records
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B16F60. Both reads are the inlined CDataInputStream::Read (64-bit position range check,
//   direct copy while the source chunk has room, else CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0), as in the
//   landed 0x007AD7F0.
// Layout: PDB (Ego_r) CDataInputStream names (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xc, SourceChunkStreamPos +0x10,
//   SourceChunkBytesRemaining +0x14) and CPSCOrbit/COrbitSettings names (CentreParam +0x0c, Orbits +0x10, 0x90 bytes).
//   Local model: engine/CDataInputStream.h is data-only and cannot carry the inline Read this function inlines.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
class CDataInputStream { public:
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
namespace NParticleEngine {
struct COrbitSettings {
    long Type; bool Enabled; float Radius, Expand; bool CycleFlag;
    float CycleTime, SqueezeScale, SqueezeAngle, RotateSpeed, RotateStart, RotateSpeedRandom, RotateStartRandom;
};
class CPSCOrbit { public:
    unsigned char _base[0xc];                           // CPSCBase
    unsigned long CentreParam;                          // +0x0c
    COrbitSettings Orbits[3];                           // +0x10
    void ReadBinary(CDataInputStream* in);
};
void CPSCOrbit::ReadBinary(CDataInputStream* in)
{
    unsigned long value; in->Read(&value, sizeof(value)); CentreParam = value;
    in->Read(Orbits, sizeof(Orbits));
}
}