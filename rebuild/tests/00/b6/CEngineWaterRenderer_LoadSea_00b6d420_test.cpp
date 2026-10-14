#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_ctor = 0, g_dtor = 0; static void* g_args[5];
struct CMemoryDataInputStream { int x; };
struct CIndexBuffer { unsigned char _body[0x14]; };
struct CVertexBufferWin32 { unsigned char _body[0x34]; };
struct CWaterSeaGenerator { long m_state; CWaterSeaGenerator() { ++g_ctor; } ~CWaterSeaGenerator() { ++g_dtor; }
    void LoadSea(CMemoryDataInputStream* s, CVertexBufferWin32* vb, long* vc, CIndexBuffer* ib, long* pc) { g_args[0] = s; g_args[1] = vb; g_args[2] = vc; g_args[3] = ib; g_args[4] = pc; *vc = 100; *pc = 50; } };
struct CEngineWaterRenderer { unsigned char _pad0[0x68]; CIndexBuffer SeaBackgroundIndexBuffer; CVertexBufferWin32 SeaBackgroundVertexBuffer; long SeaBackgroundVertexCount; long SeaBackgroundPolyCount; unsigned char _pad1[0x276 - 0xb8]; bool HaveBackgroundSea;
    void LoadSea(CMemoryDataInputStream* stream) { CWaterSeaGenerator generator; long polyCount = 0;
        generator.LoadSea(stream, &SeaBackgroundVertexBuffer, &SeaBackgroundVertexCount, &SeaBackgroundIndexBuffer, &polyCount);
        SeaBackgroundPolyCount = polyCount - 2; HaveBackgroundSea = true; } };
int main()
{
    CEngineWaterRenderer* r = (CEngineWaterRenderer*)std::malloc(sizeof(CEngineWaterRenderer)); std::memset(r, 0, sizeof(*r)); CMemoryDataInputStream s;
    r->LoadSea(&s);
    bool ok = g_ctor == 1 && g_dtor == 1 && g_args[0] == &s && g_args[1] == &r->SeaBackgroundVertexBuffer && g_args[2] == &r->SeaBackgroundVertexCount && g_args[3] == &r->SeaBackgroundIndexBuffer
        && r->SeaBackgroundVertexCount == 100 && r->SeaBackgroundPolyCount == 48 && r->HaveBackgroundSea;
    std::free(r);
    if (!ok) { std::printf("00b6d420_TEST FAIL\n"); return 1; }
    std::printf("00b6d420_TEST PASS\n"); return 0;
}