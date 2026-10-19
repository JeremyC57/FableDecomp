// Behaviour test for retail 0x00b16f60: the candidate source below is embedded verbatim (from src/00b16f60.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// NParticleEngine::CPSCOrbit::ReadBinary(CDataInputStream*) -- read the centre parameter, then the three orbit-setting records
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B16F60. Both reads are the inlined CDataInputStream::Read (64-bit position range check,
//   direct copy while the source chunk has room, else CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0), as in the
//   landed 0x007AD7F0.
// Layout: PDB (Ego_r) CDataInputStream names (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xc, SourceChunkStreamPos +0x10,
//   SourceChunkBytesRemaining +0x14) and CPSCOrbit/COrbitSettings names (CentreParam +0x0c, Orbits +0x10, 0x90 bytes).
//   Local model: engine/CDataInputStream.h is data-only and cannot carry the inline Read this function inlines.
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
namespace NParticleEngine {
struct COrbitSettings {
    long Type; bool Enabled; float Radius, Expand; bool CycleFlag;
    float CycleTime, SqueezeScale, SqueezeAngle, RotateSpeed, RotateStart, RotateSpeedRandom, RotateStartRandom;
};
struct CPSCOrbit {
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

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
using namespace NParticleEngine;
static int g_overflow; static long g_ovsize;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buf, long size) { ++g_overflow; g_ovsize = size; memset(buf, 0x77, size); StreamPos += size; }
int main() {
    static unsigned char src[4 + 0x90]; for (int i = 0; i < (int)sizeof src; ++i) src[i] = (unsigned char)(i * 7 + 1);
    CPSCOrbit o; memset(&o, 0, sizeof o);
    CDataInputStream in; memset(&in, 0, sizeof in); in.CurrentSourcePtr = src; in.SourceChunkBytesRemaining = sizeof src; in.StreamPos = 10;
    o.ReadBinary(&in);
    int ok = offsetof(CPSCOrbit, Orbits) == 0x10 && sizeof(o.Orbits) == 0x90;
    ok = ok && o.CentreParam == *(unsigned long*)src && !memcmp(o.Orbits, src + 4, 0x90) && in.StreamPos == 10 + 0x94;
    ok = ok && in.CurrentSourcePtr == src + sizeof src && in.SourceChunkBytesRemaining == 0 && g_overflow == 0;
    /* chunk exhausted: both reads go through the overflow path */
    in.SourceChunkBytesRemaining = 2; o.ReadBinary(&in);
    ok = ok && g_overflow == 2 && g_ovsize == 0x90 && o.CentreParam == 0x77777777 && ((unsigned char*)o.Orbits)[0x8f] == 0x77;
    /* position overflow beyond 0x7fffffff: nothing read */
    CPSCOrbit p; memset(&p, 0, sizeof p); in.StreamPos = 0x7ffffffe; in.SourceChunkBytesRemaining = 1000; g_overflow = 0;
    p.ReadBinary(&in);   /* CentreParam takes the unread (uninitialised) local, as in retail: not checked */
    ok = ok && g_overflow == 0 && p.Orbits[2].RotateStartRandom == 0 && in.StreamPos == 0x7ffffffe && in.SourceChunkBytesRemaining == 1000;
    printf(ok ? "00b16f60_TEST PASS\n" : "00b16f60_TEST FAIL\n"); return ok ? 0 : 1;
}