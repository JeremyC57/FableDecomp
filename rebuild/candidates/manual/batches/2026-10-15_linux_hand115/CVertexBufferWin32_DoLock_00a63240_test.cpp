#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_notified = 0; static void* g_notifiedBuffer = 0; static float g_weight = 0; static unsigned long g_offset = 0, g_size = 0, g_flags = 0; static int g_mapped;
struct IDirect3DVertexBuffer {
    virtual long __stdcall QueryInterface(void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual long __stdcall v03() { return 0; } virtual long __stdcall v04() { return 0; } virtual long __stdcall v05() { return 0; }
    virtual long __stdcall v06() { return 0; } virtual long __stdcall v07() { return 0; } virtual long __stdcall v08() { return 0; }
    virtual long __stdcall v09() { return 0; } virtual long __stdcall v10() { return 0; }
    virtual long __stdcall Lock(unsigned long offset, unsigned long size, void** data, unsigned long flags) { g_offset = offset; g_size = size; g_flags = flags; if (*data != 0) return -1; *data = &g_mapped; return 0; }
};
struct CRenderManagerCore { void VertexBufferLocked(IDirect3DVertexBuffer* b, float w) { ++g_notified; g_notifiedBuffer = b; g_weight = w; } };
struct CDisplayManager { void* __vftable; void* SystemManager; CRenderManagerCore* PRenderManager; };
struct CSystemManager { unsigned char _pad00[0x60]; CDisplayManager* PDisplayManager; void* PDisplayManagerCount; };
static CSystemManager g_system; static CDisplayManager g_display; static CRenderManagerCore g_core;
CSystemManager* __fastcall GFGetSystemManager() { return &g_system; }
struct CVertexBufferWin32 {
    virtual void v00() { }
    IDirect3DVertexBuffer* m_pBuffer; unsigned long m_size;
    void* DoLock(unsigned long flags, unsigned long offset, unsigned long size)
    {
        if (size == 0) size = m_size;
        CSystemManager* system = GFGetSystemManager();
        system->PDisplayManager->PRenderManager->VertexBufferLocked(m_pBuffer, 1.0f);
        void* data = 0; m_pBuffer->Lock(offset, size, &data, flags); return data;
    }
};
int main()
{
    g_system.PDisplayManager = &g_display; g_display.PRenderManager = &g_core; IDirect3DVertexBuffer buffer;
    CVertexBufferWin32 v; v.m_pBuffer = &buffer; v.m_size = 4096;
    if (v.DoLock(0x1000, 16, 0) != &g_mapped || g_offset != 16 || g_size != 4096 || g_flags != 0x1000 || g_notified != 1 || g_notifiedBuffer != &buffer || g_weight != 1.0f) { std::printf("00a63240_TEST FAIL\n"); return 1; }
    if (v.DoLock(0, 0, 64) != &g_mapped || g_offset != 0 || g_size != 64 || g_flags != 0 || g_notified != 2) { std::printf("00a63240_TEST FAIL\n"); return 1; }
    std::printf("00a63240_TEST PASS\n"); return 0;
}
