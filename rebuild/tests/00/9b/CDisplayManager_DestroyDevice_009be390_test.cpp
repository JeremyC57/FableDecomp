#include <cstdio>
static int g_released = 0; static const void* g_uninit[2]; static int g_n = 0;
struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { ++g_released; return 0; }
};
struct CSurface { unsigned char _body[0x10]; void Uninitialise() { if (g_n < 2) g_uninit[g_n] = this; ++g_n; } };
struct CDisplayManager {
    unsigned char _pad[0x2c]; CSurface BackBufferSurface; CSurface DepthBufferSurface; unsigned char _pad2[0xc]; IDirect3DDevice9* D3DDevice;
    void DestroyDevice() { BackBufferSurface.Uninitialise(); DepthBufferSurface.Uninitialise(); if (D3DDevice) D3DDevice->Release(); }
};
int main()
{
    IDirect3DDevice9 dev; CDisplayManager m; m.D3DDevice = 0;
    m.DestroyDevice();
    if (g_n != 2 || g_uninit[0] != &m.BackBufferSurface || g_uninit[1] != &m.DepthBufferSurface || g_released != 0) { std::printf("009be390_TEST FAIL\n"); return 1; }
    m.D3DDevice = &dev; m.DestroyDevice();
    if (g_n != 4 || g_released != 1 || (char*)&m.D3DDevice - (char*)&m != 0x58 || (char*)&m.DepthBufferSurface - (char*)&m != 0x3c) { std::printf("009be390_TEST FAIL\n"); return 1; }
    std::printf("009be390_TEST PASS\n"); return 0;
}