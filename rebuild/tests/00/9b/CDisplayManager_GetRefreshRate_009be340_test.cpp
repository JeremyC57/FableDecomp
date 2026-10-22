// Behaviour test for 009be340 CDisplayManager::GetRefreshRate: query adapter 0's display mode, throw
// CCreateDeviceFailed on a failing HRESULT, otherwise return its refresh rate.
#include <cstdio>
struct D3DDISPLAYMODE { unsigned int Width, Height, RefreshRate; int Format; };
struct IDirect3D9 {
    int hr; unsigned int adapterSeen;
    int GetAdapterDisplayMode(unsigned int adapter, D3DDISPLAYMODE* mode) { adapterSeen = adapter; mode->Width = 640; mode->Height = 480; mode->RefreshRate = 75; mode->Format = 22; return hr; }
};
struct CCreateDeviceFailed { CCreateDeviceFailed() {} };
struct CDisplayManager {
    IDirect3D9* PD3D;
    unsigned int GetRefreshRate()
    {
        D3DDISPLAYMODE mode;
        if (PD3D->GetAdapterDisplayMode(0, &mode) < 0)
            throw CCreateDeviceFailed();
        return mode.RefreshRate;
    }
};
int main()
{
    IDirect3D9 d3d; d3d.hr = 0; d3d.adapterSeen = 9;
    CDisplayManager m; m.PD3D = &d3d;
    if (m.GetRefreshRate() != 75 || d3d.adapterSeen != 0) { std::printf("009be340_TEST FAIL\n"); return 1; }
    d3d.hr = -2005530516;          /* D3DERR_INVALIDCALL */
    int thrown = 0;
    try { m.GetRefreshRate(); } catch (const CCreateDeviceFailed&) { thrown = 1; }
    if (!thrown) { std::printf("009be340_TEST FAIL\n"); return 1; }
    std::printf("009be340_TEST PASS\n");
    return 0;
}