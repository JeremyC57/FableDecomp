#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_released = 0, g_notified = 0; static void* g_notifiedBuffer = 0; static float g_weight = 0;
struct IDirect3DVertexBuffer {
    virtual long __stdcall QueryInterface(void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { ++g_released; return 0; }
};
struct CRenderManagerCore { void VertexBufferReleased(IDirect3DVertexBuffer* b, float w) { ++g_notified; g_notifiedBuffer = b; g_weight = w; } };
struct CDisplayManager { void* __vftable; void* SystemManager; CRenderManagerCore* PRenderManager; };
struct CSystemManager { unsigned char _pad00[0x60]; CDisplayManager* PDisplayManager; void* PDisplayManagerCount; };
static CSystemManager g_system; static CDisplayManager g_display; static CRenderManagerCore g_core;
CSystemManager* __fastcall GFGetSystemManager() { return &g_system; }
struct CVertexBufferWin32 {
    virtual void v00() { }
    IDirect3DVertexBuffer* m_pBuffer; unsigned long m_size;
    void Uninitialise() { if (m_pBuffer) { CSystemManager* system = GFGetSystemManager(); system->PDisplayManager->PRenderManager->VertexBufferReleased(m_pBuffer, 1.0f); m_pBuffer->Release(); m_pBuffer = 0; } }
};
int main()
{
    g_system.PDisplayManager = &g_display; g_display.PRenderManager = &g_core; IDirect3DVertexBuffer buffer;
    CVertexBufferWin32 v; v.m_pBuffer = 0; v.Uninitialise();
    if (g_released != 0 || g_notified != 0) { std::printf("00a63200_TEST FAIL\n"); return 1; }
    v.m_pBuffer = &buffer; v.Uninitialise();
    if (g_released != 1 || g_notified != 1 || g_notifiedBuffer != &buffer || g_weight != 1.0f || v.m_pBuffer != 0) { std::printf("00a63200_TEST FAIL\n"); return 1; }
    std::printf("00a63200_TEST PASS\n"); return 0;
}