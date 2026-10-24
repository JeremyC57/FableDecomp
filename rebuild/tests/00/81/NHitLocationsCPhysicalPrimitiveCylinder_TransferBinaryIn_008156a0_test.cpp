#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/008156a0.cpp) ----
// NHitLocations::CPhysicalPrimitiveCylinder::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the base name, then Radius and Height (two inlined 4-byte stream reads)
// Leaf: ?TransferBinaryIn@CPhysicalPrimitiveCylinder@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008156A0, 203 bytes (+5 int3 padding).
// Name: FableWin.pdb ?TransferBinaryIn@CPhysicalPrimitiveCylinder@NHitLocations@@UAEXAAVCMemoryDataInputStream@@@Z;
// slot 5 of the vtable 0x0125BA4C (rtti-vtables.tsv; slot 5 of the sphere is the landed TransferBinaryIn 0x00815620).
// Callees: CDefString::TransferBinaryIn(CMemoryDataInputStream*) @ 0x0044FC00 (on BaseName),
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: Ego_r.pdb CPhysicalPrimitiveCylinder {vptr, CDefString BaseName +4, float Radius +8, float Height +0xC};
// the stream model is the landed sphere sibling's (position +4, chunk pointer +0xC, remaining +0x14).
// Shape: the sphere sibling's body with a second float: each inlined CMemoryDataInputStream::Read(&v, 4)
// (64-bit position range check, chunk copy or overflow read) fills ONE float local (shared by both reads; two locals give a second
// frame slot, `push ecx`) homed in the dead
// stream-argument slot, then the member is stored; the second read's member store is duplicated
// into both exits. `ret 4`.

struct CMemoryDataInputStream {
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
};

struct CDefString {
    long TablePos;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

namespace NHitLocations {

struct CPhysicalPrimitiveCylinder {
    virtual void v00();
    CDefString BaseName;                           // +0x04
    float      Radius;                             // +0x08
    float      Height;                             // +0x0C
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CPhysicalPrimitiveCylinder::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    BaseName.TransferBinaryIn(&stream);
    float value;
    stream.Read(&value, sizeof(value));
    Radius = value;
    stream.Read(&value, sizeof(value));
    Height = value;
}

} // namespace NHitLocations

// ---- stubs for the external callees ----

static int g_overflowCalls = 0;
void CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflowCalls;
    float v = 9.5f;
    memcpy(buffer, &v, size);
}
void CDefString::TransferBinaryIn(CMemoryDataInputStream* stream) { stream->Read(&TablePos, 4); }
void NHitLocations::CPhysicalPrimitiveCylinder::v00() {}

static void run()
{

    unsigned char buffer[12];
    int index = 7;
    float radius = 2.5f, height = 4.0f;
    memcpy(buffer, &index, 4);
    memcpy(buffer + 4, &radius, 4);
    memcpy(buffer + 8, &height, 4);
    CMemoryDataInputStream stream;
    stream.m_pVtable = 0;
    stream.m_position = 0;
    stream.StreamSize = 12;
    stream.m_pData = buffer;
    stream.SourceChunkStreamPos = 0;
    stream.m_remaining = 12;
    NHitLocations::CPhysicalPrimitiveCylinder c;
    c.BaseName.TablePos = 0;
    c.Radius = 0.0f;
    c.Height = 0.0f;
    c.TransferBinaryIn(stream);
    CHECK(c.BaseName.TablePos == 7);
    CHECK(c.Radius == 2.5f);
    CHECK(c.Height == 4.0f);
    CHECK(stream.m_position == 12 && stream.m_remaining == 0 && stream.m_pData == buffer + 12);
    CHECK(g_overflowCalls == 0);
    // a chunk with only the index left: both floats come through the overflow reader
    stream.m_position = 0;
    stream.m_pData = buffer;
    stream.m_remaining = 4;
    c.TransferBinaryIn(stream);
    CHECK(g_overflowCalls == 2);
    CHECK(c.Radius == 9.5f && c.Height == 9.5f);
}

int main()
{
    run();
    if (!g_ok) { std::printf("008156a0_TEST FAIL\n"); return 1; }
    std::printf("008156a0_TEST PASS\n");
    return 0;
}