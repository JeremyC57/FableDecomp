// Behaviour test for 00b18390: final.cpp copied verbatim (memcpy wrapper and 32-bit long under g++), the stream's
// overflow read, Read3DVector and the point array's resize stubbed so both read paths and the user-point loop are observable.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* CopyBytes(void* d, const void* s, unsigned int n) { return std::memcpy(d, s, n); }
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 int64_t
#define long int
extern "C" void* fable_memcpy(void* d, const void* s, unsigned int n) { return CopyBytes(d, s, n); }
#define memcpy fable_memcpy
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

// ---- final.cpp verbatim ----
// NParticleEngine::CPSCAttractor::ReadBinary(CDataInputStream*) -- read the attractor settings and its user points back (the mirror of WriteBinary 0x00B17F70; the position-param name word is skipped)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18390, 785 bytes (vtable slot 15 of CPSCAttractor, 0x012A04AC).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (every read is the inlined CDataInputStream::Read,
//   as in the landed CPSCOrbit::ReadBinary 0x00B16F60), CArray<C3DVector>::resize(n, const C3DVector&) 0x0075C040
//   (as in the landed CPSCAttractor::DatabaseCopy 0x00B18060), CDataInputStream::Read3DVector 0x009950F0 (C3DVector by
//   hidden pointer, as in the landed C3DMeshFileConvexVolumeChunk::ReadFromFile 0x00AAAB10).
// Layout: PDB (Ego_r) CPSCAttractor (retail offsets equal the PDB, the point CArray is a 12-byte STLport vector):
//   flag bits +0x0c, AttractorInfluenceFallOff +0x10, AttractorPositionParam +0x14, AttractorPositionParamName +0x18,
//   radius/force +0x1c/+0x20, AttractorUserPointsArray +0x24. CDataInputStream as in the landed readers.
// Shape: the eight inlined reads in WriteBinary order; the name word is read and dropped (no store survives); the flags
//   are bool bit-field assignments from a one-byte read (`test ; setne ; xor ; and ; xor`).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    C3DVector Read3DVector();                           // 0x009950F0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};
struct CVector3D {                                      // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() { return _M_start; }
    C3DVector& operator[](unsigned long n) { return begin()[n]; }
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
};
namespace NParticleEngine {
struct CPSCAttractor {
    unsigned char _base[0xc];                           // CPSCBase
    bool AttractorEnabled : 1;                          // +0x0c bit 0
    bool AttractorUseParamPosition : 1;                 // +0x0c bit 1
    long AttractorInfluenceFallOff;                     // +0x10
    unsigned long AttractorPositionParam;               // +0x14
    unsigned long AttractorPositionParamName;           // +0x18
    float AttractorInfluenceRadius;                     // +0x1c
    float AttractorInfluenceForce;                      // +0x20
    CVector3D AttractorUserPointsArray;                 // +0x24
    void ReadBinary(CDataInputStream* in);
};
void CPSCAttractor::ReadBinary(CDataInputStream* in)
{
    AttractorEnabled = in->ReadBool();
    AttractorUseParamPosition = in->ReadBool();
    AttractorInfluenceFallOff = in->ReadLong();
    AttractorPositionParam = in->ReadULong();
    in->ReadULong();
    AttractorInfluenceRadius = in->ReadFloat();
    AttractorInfluenceForce = in->ReadFloat();
    unsigned long count = in->ReadULong();
    AttractorUserPointsArray.resize(count);
    for (unsigned long i = 0; i < count; ++i)
        AttractorUserPointsArray[i] = in->Read3DVector();
}
}

// ---- stubs ----
static unsigned char g_stream[256];
static unsigned long g_streamLen = 0;
static int g_overflowCalls = 0;
static int g_vectorReads = 0;
static int g_resizeCalls = 0;
static unsigned int g_resizeSize = 0;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflowCalls;
    CopyBytes(buffer, g_stream + StreamPos, (unsigned int)size);
    StreamPos += size;
    CurrentSourcePtr = g_stream + StreamPos;
    SourceChunkBytesRemaining = (long)(g_streamLen - StreamPos);
}
C3DVector CDataInputStream::Read3DVector()
{
    ++g_vectorReads;
    C3DVector v;
    Read(&v, sizeof(v));
    return v;
}
void CVector3D::resize(unsigned int newSize, const C3DVector& value)
{
    ++g_resizeCalls; g_resizeSize = newSize;
    _M_start = (C3DVector*)std::malloc(newSize * sizeof(C3DVector) + 1);
    _M_finish = _M_start + newSize;
    _M_end_of_storage = _M_finish;
    for (unsigned int i = 0; i < newSize; ++i) _M_start[i] = value;
}

static void Put(const void* p, unsigned int n) { CopyBytes(g_stream + g_streamLen, p, n); g_streamLen += n; }
static void PutByte(unsigned char b) { Put(&b, 1); }
static void PutLong(long v) { Put(&v, 4); }
static void PutFloat(float v) { Put(&v, 4); }

static void BuildStream(unsigned char enabled, unsigned char useParam)
{
    g_streamLen = 0;
    PutByte(enabled); PutByte(useParam);
    PutLong(7); PutLong(0x11); PutLong(0x22);
    PutFloat(1.5f); PutFloat(2.5f);
    PutLong(2);
    PutFloat(1.0f); PutFloat(2.0f); PutFloat(3.0f);
    PutFloat(4.0f); PutFloat(5.0f); PutFloat(6.0f);
}

static bool Run(long chunkBytes, unsigned char enabled, unsigned char useParam, bool expectOverflow)
{
    BuildStream(enabled, useParam);
    g_overflowCalls = 0; g_vectorReads = 0; g_resizeCalls = 0; g_resizeSize = 0;
    CDataInputStream in;
    in.__vftable = 0; in.StreamPos = 0; in.StreamSize = (long)g_streamLen; in.CurrentSourcePtr = g_stream;
    in.SourceChunkStreamPos = 0; in.SourceChunkBytesRemaining = chunkBytes;
    static NParticleEngine::CPSCAttractor a;
    std::memset(&a, 0, sizeof(a));
    a.AttractorUseParamPosition = true;    // bit 1 preset: the flag read must clear it when the byte is zero
    a.ReadBinary(&in);
    bool ok = true;
    ok = ok && a.AttractorEnabled == (enabled != 0) && a.AttractorUseParamPosition == (useParam != 0);
    ok = ok && a.AttractorInfluenceFallOff == 7 && a.AttractorPositionParam == 0x11 && a.AttractorPositionParamName == 0;
    ok = ok && a.AttractorInfluenceRadius == 1.5f && a.AttractorInfluenceForce == 2.5f;
    ok = ok && g_resizeCalls == 1 && g_resizeSize == 2 && g_vectorReads == 2;
    ok = ok && a.AttractorUserPointsArray._M_start[0].X == 1.0f && a.AttractorUserPointsArray._M_start[0].Z == 3.0f;
    ok = ok && a.AttractorUserPointsArray._M_start[1].Y == 5.0f && a.AttractorUserPointsArray._M_start[1].Z == 6.0f;
    ok = ok && in.StreamPos == g_streamLen;
    ok = ok && (expectOverflow ? g_overflowCalls > 0 : g_overflowCalls == 0);
    std::free(a.AttractorUserPointsArray._M_start);
    return ok;
}

int main()
{
    CHECK(Run(256, 1, 0, false));      // whole stream in the source chunk: every read takes the inline memcpy path
    CHECK(Run(0, 0, 1, true));         // nothing in the chunk: every read goes through ReadWithSrcChunkOverflow
    CHECK(Run(3, 1, 1, true));         // the chunk runs out inside the longs: both paths in one run
    if (fails) { printf("00b18390_TEST FAIL (%d)\n", fails); return 1; }
    printf("00b18390_TEST PASS\n");
    return 0;
}