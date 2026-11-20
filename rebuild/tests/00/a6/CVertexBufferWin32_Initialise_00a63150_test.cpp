// 00a63150 test: CVertexBufferWin32::Initialise on the shared header, linked against the row object (CreateVertexBuffer through the manager chain, the recorded fields, the result test).
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include <stdio.h>
#include <string.h>
// Test doubles: raw storage at the retail offsets (CSystemManager::PDisplayManager +0x60, CDisplayManager::D3DDevice +0x58), a device
// whose vtable has only CreateVertexBuffer (slot 26, +0x68); CVertexBufferWin32 has out-of-line virtuals, so the test uses raw storage.
static unsigned char g_system[0x180], g_display[0x2cc];
static void Put(unsigned char* base, unsigned int offset, void* value) { memcpy(base + offset, &value, sizeof(value)); }
CSystemManager* __fastcall GFGetSystemManager() { return (CSystemManager*)g_system; }
static unsigned long g_create[4]; static void* g_createThis = 0; static void** g_shared = (void**)1; static long g_result = 0; static int g_created;
static long __stdcall FakeCreateVertexBuffer(void* self, unsigned int length, unsigned long usage, unsigned long fvf, unsigned long pool, IDirect3DVertexBuffer9** out, void** shared)
{
    g_createThis = self; g_create[0] = length; g_create[1] = usage; g_create[2] = fvf; g_create[3] = pool; g_shared = shared;
    *out = (IDirect3DVertexBuffer9*)&g_created; return g_result;
}
static void* g_deviceVtbl[120]; static void* g_device[1] = { g_deviceVtbl };
static unsigned char g_raw[sizeof(CVertexBufferWin32)];

int main()
{
    g_deviceVtbl[26] = (void*)&FakeCreateVertexBuffer;
    Put(g_system, 0x60, g_display); Put(g_display, 0x58, g_device);
    for (int round = 0; round < 2; ++round) {
        memset(g_raw, 0xEE, sizeof(g_raw));
        CVertexBufferWin32* vb = (CVertexBufferWin32*)g_raw;
        g_result = round == 0 ? 0 : (long)0x8876086CL;
        bool ok = vb->Initialise(VERTEX_NORM_TEX1, 30, 24, 8, VB_POOL_MANAGED);
        const CVertexCompressionParams& p = vb->CompressionParams;
        if (ok != (round == 0) || g_createThis != (void*)g_device || g_create[0] != 720 || g_create[1] != 8 || g_create[2] != 274 || g_create[3] != 1 || g_shared != 0
            || vb->PVertexBuffer != (IDirect3DVertexBuffer9*)&g_created || vb->Length != 720 || vb->Stride != 24 || vb->Type != VERTEX_NORM_TEX1
            || p.PosScaleX != 1.0f || p.PosScaleY != 1.0f || p.PosScaleZ != 1.0f || p.PosScaleW != 1.0f
            || p.PosBiasX != 0.0f || p.PosBiasY != 0.0f || p.PosBiasZ != 0.0f || p.PosBiasW != 0.0f) { printf("00a63150_TEST FAIL %d\n", round); return 1; }
    }
    printf("00a63150_TEST PASS\n"); return 0;
}
