// Behaviour test for 00bfc600: final.cpp copied verbatim (32-bit `long`, `__int64`, calling-convention keywords and the
// CRT memcpy name under g++; build with `g++ -w -fpermissive`); the stream's slow path and the four LoadVerts
// instantiations are stubbed to record their order and to fail on demand. CPatchTesselationEdgeStrip::Load
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
extern "C" void* fable_memcpy(void* d, const void* s, unsigned int n) { return CopyBytes(d, s, n); }
#define memcpy fable_memcpy
#define long int
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPatchTesselationEdgeStrip::Load(CMemoryDataInputStream* stream, CMovableResourceHandler* handler) -- read StartPos and Size (words) and the Vertical flag from the stream, then load the four vertex arrays through the LoadVerts<T> member template; false as soon as one fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC600, length 360 bytes (+ int3 padding), `ret 8`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the slow path of the inline reads);
// LoadVerts<CVertex> 0x00BFC880, LoadVerts<CFanBaseVertex> 0x00BFCA20, LoadVerts<CWaterVertex> 0x00BFCBC0,
// LoadVerts<CWaterFanBaseVertex> 0x00BFCD60 (the four instantiations in member order, as the landed Save 0x00BFC770).
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, the landed Save/Reset model); the stream reader is the
// landed CMemoryDataInputStream inline Read (0x00632E00 family: position +4, chunk pointer +0xC, bytes left +0x14).
// Shape: each value is an inline by-value ReadUShort/ReadBool whose local shares one frame slot; the flag is
// `value != 0` (setne); the four loads are early-returning tests with the last one's bool as the result.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
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
    __forceinline unsigned short ReadUShort()
    {
        unsigned short value;
        Read(&value, sizeof(value));
        return value;
    }
    __forceinline bool ReadBool()
    {
        unsigned char value;
        Read(&value, sizeof(value));
        return value != 0;
    }
};

struct CMovableResourceHandler;

template <class T>
struct CEdgeStipVertexArray {
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
};

struct CPatchTesselationEdgeStrip {                // vtable 0x012A8318 (CBase-derived), 0x2c bytes
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    virtual void v00();
    unsigned short StartPos;                                            // +0x04
    unsigned short Size;                                                // +0x06
    CEdgeStipVertexArray<CVertex>             Verts;                    // +0x08
    CEdgeStipVertexArray<CFanBaseVertex>      FanBaseVerts;             // +0x10
    CEdgeStipVertexArray<CWaterVertex>        WaterVerts;               // +0x18
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;        // +0x20
    bool Tesselated;                                                    // +0x28
    bool Vertical;                                                      // +0x29
    template <class T> bool LoadVerts(CEdgeStipVertexArray<T>* verts, CMemoryDataInputStream* stream, CMovableResourceHandler* handler);   // 0x00BFC880..0x00BFCD60
    bool Load(CMemoryDataInputStream* stream, CMovableResourceHandler* handler);
};

bool CPatchTesselationEdgeStrip::Load(CMemoryDataInputStream* stream, CMovableResourceHandler* handler)
{
    StartPos = stream->ReadUShort();
    Size = stream->ReadUShort();
    Vertical = stream->ReadBool();
    if (!LoadVerts(&Verts, stream, handler))
        return false;
    if (!LoadVerts(&FanBaseVerts, stream, handler))
        return false;
    if (!LoadVerts(&WaterVerts, stream, handler))
        return false;
    if (!LoadVerts(&WaterFanBaseVerts, stream, handler))
        return false;
    return true;
}

// ---- stubs ----
struct CPatchTesselationEdgeStrip::CVertex { int v; };
struct CPatchTesselationEdgeStrip::CFanBaseVertex { int v; };
struct CPatchTesselationEdgeStrip::CWaterVertex { int v; };
struct CPatchTesselationEdgeStrip::CWaterFanBaseVertex { int v; };
struct CMovableResourceHandler { int tag; };
static int g_overflowCalls = 0;
void CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflowCalls;
    for (long i = 0; i < size; ++i) buffer[i] = (unsigned char)(0x80 + i);
    m_position += size;
}
static int g_loadCalls = 0; static const void* g_loadArrays[8]; static bool g_loadResults[8];
static CMemoryDataInputStream* g_loadStream = 0; static CMovableResourceHandler* g_loadHandler = 0;
template <class T>
bool CPatchTesselationEdgeStrip::LoadVerts(CEdgeStipVertexArray<T>* verts, CMemoryDataInputStream* stream, CMovableResourceHandler* handler)
{
    if (stream != g_loadStream || handler != g_loadHandler || g_loadCalls >= 8) { g_loadCalls = 100; return false; }
    g_loadArrays[g_loadCalls] = verts;
    return g_loadResults[g_loadCalls++];
}
void CPatchTesselationEdgeStrip::v00() {}
static unsigned char g_bytes[] = { 0x34, 0x12, 0x78, 0x56, 0x01, 0xff, 0xee };
static bool Run(CPatchTesselationEdgeStrip& strip, CMemoryDataInputStream& stream, CMovableResourceHandler& handler,
                bool r0, bool r1, bool r2, bool r3, long remaining)
{
    std::memset(&stream, 0, sizeof(stream));
    stream.m_pData = g_bytes; stream.m_remaining = remaining; stream.m_position = 100;
    g_loadResults[0] = r0; g_loadResults[1] = r1; g_loadResults[2] = r2; g_loadResults[3] = r3;
    g_loadCalls = 0; g_overflowCalls = 0; g_loadStream = &stream; g_loadHandler = &handler;
    strip.StartPos = 0; strip.Size = 0; strip.Vertical = true; strip.Tesselated = true;
    return strip.Load(&stream, &handler);
}
int main()
{
    CPatchTesselationEdgeStrip strip; std::memset((void*)&strip, 0, sizeof(strip));
    CMemoryDataInputStream stream; CMovableResourceHandler handler; handler.tag = 7;
    // all four succeed
    CHECK(Run(strip, stream, handler, true, true, true, true, 100) == true);
    CHECK(strip.StartPos == 0x1234 && strip.Size == 0x5678 && strip.Vertical == true && strip.Tesselated == true);
    CHECK(stream.m_position == 105 && stream.m_remaining == 95 && stream.m_pData == g_bytes + 5 && g_overflowCalls == 0);
    CHECK(g_loadCalls == 4 && g_loadArrays[0] == &strip.Verts && g_loadArrays[1] == &strip.FanBaseVerts &&
          g_loadArrays[2] == &strip.WaterVerts && g_loadArrays[3] == &strip.WaterFanBaseVerts);
    // each failure stops the sequence
    CHECK(Run(strip, stream, handler, false, true, true, true, 100) == false && g_loadCalls == 1);
    CHECK(Run(strip, stream, handler, true, false, true, true, 100) == false && g_loadCalls == 2);
    CHECK(Run(strip, stream, handler, true, true, false, true, 100) == false && g_loadCalls == 3);
    CHECK(Run(strip, stream, handler, true, true, true, false, 100) == false && g_loadCalls == 4);
    // the flag byte zero gives Vertical == false; a short chunk takes the overflow path
    g_bytes[4] = 0;
    CHECK(Run(strip, stream, handler, true, true, true, true, 100) == true && strip.Vertical == false);
    g_bytes[4] = 1;
    CHECK(Run(strip, stream, handler, true, true, true, true, 3) == true);
    CHECK(strip.StartPos == 0x1234 && g_overflowCalls == 1 && strip.Size == 0x8180 && strip.Vertical == true);   // the second word takes the overflow path, the flag byte fits the chunk again
    if (fails == 0) printf("00bfc600_TEST PASS\n"); else printf("FAILS: %d\n", fails);
    return fails ? 1 : 0;
}