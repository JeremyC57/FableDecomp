#include <cstdio>
typedef long HRESULT;
struct IDirect3DDevice9 {
    virtual void __stdcall _v00(); virtual void __stdcall _v01(); virtual void __stdcall _v02(); virtual void __stdcall _v03();
    virtual void __stdcall _v04(); virtual void __stdcall _v05(); virtual void __stdcall _v06(); virtual void __stdcall _v07();
    virtual void __stdcall _v08(); virtual void __stdcall _v09(); virtual void __stdcall _v10(); virtual void __stdcall _v11();
    virtual void __stdcall _v12(); virtual void __stdcall _v13(); virtual void __stdcall _v14(); virtual void __stdcall _v15();
    virtual void __stdcall _v16();
    virtual HRESULT __stdcall Present(const void* source_rect, const void* dest_rect, void* dest_window, const void* dirty_region);   // slot 17
};
const HRESULT D3D_OK = 0;
const HRESULT D3DERR_DEVICELOST = (HRESULT)0x88760868;
class CVsyncCallbackTimer {
public:
    void OnPresent();                                    // 0x009F73C0 (name not confirmed)
};
class CDisplayManager {
public:
    unsigned char        _pad000[0x58];
    IDirect3DDevice9*    D3DDevice;                      // +0x058
    unsigned char        _pad05c[0x19c - 0x5c];
    long                 NoBackBuffers;                  // +0x19c
    unsigned char        _pad1a0[0x218 - 0x1a0];
    long                 CurrentScreenIndex;             // +0x218
    unsigned char        _pad21c[0x255 - 0x21c];
    bool                 DeviceLostFlag;                 // +0x255
    unsigned char        _pad256[0x25c - 0x256];
    long                 FrameCounter;                   // +0x25c
    unsigned char        _pad260[0x294 - 0x260];
    CVsyncCallbackTimer* VsyncCallbackTimer;             // +0x294 (scoped_ptr)
    bool SwapScreens();
    bool CheckPresentResult(HRESULT hr)
    {
        if (hr == D3DERR_DEVICELOST)
        {
            DeviceLostFlag = true;
            return false;
        }
        if (hr != D3D_OK)
            return false;
        return true;
    }
};
bool CDisplayManager::SwapScreens()
{
    ++FrameCounter;
    if (!CheckPresentResult(D3DDevice->Present(0, 0, 0, 0)))
        return false;
    VsyncCallbackTimer->OnPresent();
    CurrentScreenIndex = (CurrentScreenIndex + 1) % (NoBackBuffers + 1);
    return true;
}

void __stdcall IDirect3DDevice9::_v00() {}
void __stdcall IDirect3DDevice9::_v01() {}
void __stdcall IDirect3DDevice9::_v02() {}
void __stdcall IDirect3DDevice9::_v03() {}
void __stdcall IDirect3DDevice9::_v04() {}
void __stdcall IDirect3DDevice9::_v05() {}
void __stdcall IDirect3DDevice9::_v06() {}
void __stdcall IDirect3DDevice9::_v07() {}
void __stdcall IDirect3DDevice9::_v08() {}
void __stdcall IDirect3DDevice9::_v09() {}
void __stdcall IDirect3DDevice9::_v10() {}
void __stdcall IDirect3DDevice9::_v11() {}
void __stdcall IDirect3DDevice9::_v12() {}
void __stdcall IDirect3DDevice9::_v13() {}
void __stdcall IDirect3DDevice9::_v14() {}
void __stdcall IDirect3DDevice9::_v15() {}
void __stdcall IDirect3DDevice9::_v16() {}
HRESULT __stdcall IDirect3DDevice9::Present(const void*, const void*, void*, const void*) { return 1; }
static HRESULT g_hr; static int g_presents, g_ticks;
struct CTestDevice : IDirect3DDevice9 {
    void __stdcall _v00() {} void __stdcall _v01() {} void __stdcall _v02() {} void __stdcall _v03() {}
    void __stdcall _v04() {} void __stdcall _v05() {} void __stdcall _v06() {} void __stdcall _v07() {}
    void __stdcall _v08() {} void __stdcall _v09() {} void __stdcall _v10() {} void __stdcall _v11() {}
    void __stdcall _v12() {} void __stdcall _v13() {} void __stdcall _v14() {} void __stdcall _v15() {}
    void __stdcall _v16() {}
    HRESULT __stdcall Present(const void* a, const void* b, void* c, const void* d) { ++g_presents; return (a || b || c || d) ? 1 : g_hr; }
};
void CVsyncCallbackTimer::OnPresent() { ++g_ticks; }
int main()
{
    static unsigned char raw[sizeof(CDisplayManager)];
    CDisplayManager* m = (CDisplayManager*)raw;
    CTestDevice dev; CVsyncCallbackTimer timer;
    m->D3DDevice = &dev; m->VsyncCallbackTimer = &timer; m->NoBackBuffers = 2; m->CurrentScreenIndex = 1; m->FrameCounter = 10; m->DeviceLostFlag = false;
    g_hr = D3D_OK;
    bool ok = m->SwapScreens() && m->FrameCounter == 11 && m->CurrentScreenIndex == 2 && g_ticks == 1;
    ok = ok && m->SwapScreens() && m->CurrentScreenIndex == 0 && g_ticks == 2;
    g_hr = D3DERR_DEVICELOST;
    ok = ok && !m->SwapScreens() && m->DeviceLostFlag && m->CurrentScreenIndex == 0 && g_ticks == 2 && m->FrameCounter == 13;
    m->DeviceLostFlag = false; g_hr = (HRESULT)0x8876086c;
    ok = ok && !m->SwapScreens() && !m->DeviceLostFlag && g_ticks == 2 && g_presents == 4;
    std::printf(ok ? "009beeb0_TEST PASS\n" : "009beeb0_TEST FAIL\n");
    return ok ? 0 : 1;
}