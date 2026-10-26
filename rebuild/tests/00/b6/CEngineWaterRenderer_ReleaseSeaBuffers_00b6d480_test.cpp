#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_order = 0, g_vb = 0, g_ib = 0;
class CIndexBuffer { public: unsigned char _body[0x14]; void Uninitialise() { g_ib = ++g_order; } };
class CVertexBufferWin32 { public: unsigned char _body[0x34]; void Uninitialise() { g_vb = ++g_order; } };
class CEngineWaterRenderer { public: unsigned char _pad0[0x68]; CIndexBuffer SeaBackgroundIndexBuffer; CVertexBufferWin32 SeaBackgroundVertexBuffer; long SeaBackgroundVertexCount; long SeaBackgroundPolyCount; unsigned char _pad1[0x276 - 0xb8]; bool HaveBackgroundSea;
    void ReleaseSeaBuffers() { SeaBackgroundVertexBuffer.Uninitialise(); SeaBackgroundVertexCount = 0; SeaBackgroundIndexBuffer.Uninitialise(); SeaBackgroundPolyCount = 0; HaveBackgroundSea = false; } };
int main()
{
    CEngineWaterRenderer* r = (CEngineWaterRenderer*)std::malloc(sizeof(CEngineWaterRenderer)); std::memset(r, 0xee, sizeof(*r));
    r->ReleaseSeaBuffers();
    bool ok = g_vb == 1 && g_ib == 2 && r->SeaBackgroundVertexCount == 0 && r->SeaBackgroundPolyCount == 0 && !r->HaveBackgroundSea;
    std::free(r);
    if (!ok) { std::printf("00b6d480_TEST FAIL\n"); return 1; }
    std::printf("00b6d480_TEST PASS\n"); return 0;
}