// CDisplayManager::GetCurrentWindowedSettings(HWND__*, C2DExtentsI&, unsigned long&) -- report the window's outer size and the colour depth of the adapter's current display mode
// Leaf: ?GetCurrentWindowedSettings@CDisplayManager@@QAEXPAUHWND__@@AAVC2DExtentsI@@AAK@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BED10, length 103 bytes.
// Callees: IDirect3D9::GetAdapterDisplayMode (COM slot 8, `call [edx+20h]`, `this` pushed), user32 GetWindowRect (import
// @ 0x01440308), CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830, CPixelFormat::GetColourDepth() @ 0x009E3820.
// Layout: PDB CDisplayManager: D3D +0x54, D3DDeviceCaps +0x5c (AdapterOrdinal at caps +4 = +0x60).
// Shape: the D3D object, the adapter ordinal and the vtable are loaded before the 0x24-byte frame is opened (pixel format at
// +0, RECT at +4, D3DDISPLAYMODE at +0x14); `this` is dead after the first call; the size is right-left / bottom-top stored
// through the reference; the colour depth comes from a local CPixelFormat initialised with the mode's format.
#include "engine/IDirect3D9.h"   // IDirect3D9 (d3d9.h slots); engine/Direct3D9Types.h: _D3DFORMAT, D3DDISPLAYMODE
struct HWND__;

struct RECT {
    long left;
    long top;
    long right;
    long bottom;
};

extern "C" __declspec(dllimport) int __stdcall GetWindowRect(HWND__* window, RECT* rect);   // 0x01440308




















class C2DExtentsI { public:
    long X;                                        // +0x00
    long Y;                                        // +0x04
};

class CPixelFormat { public:
    unsigned long m_format;
    void Initialise(_D3DFORMAT format);         // 0x009E3830
public:
    unsigned long GetColourDepth() const;   // 0x009E3820
};

class CDisplayManager { public:
    unsigned char _pad0[0x54];
    IDirect3D9*   D3D;                             // +0x54
    void*         D3DDevice;                       // +0x58
    unsigned long DeviceType;                      // +0x5c D3DDeviceCaps.DeviceType
    unsigned long AdapterOrdinal;                  // +0x60 D3DDeviceCaps.AdapterOrdinal
    void GetCurrentWindowedSettings(HWND__* window, C2DExtentsI& size, unsigned long& depth);
};

void CDisplayManager::GetCurrentWindowedSettings(HWND__* window, C2DExtentsI& size, unsigned long& depth)
{
    D3DDISPLAYMODE mode;
    D3D->GetAdapterDisplayMode(AdapterOrdinal, &mode);
    RECT rect;
    GetWindowRect(window, &rect);
    size.X = rect.right - rect.left;
    size.Y = rect.bottom - rect.top;
    CPixelFormat format;
    format.Initialise(mode.Format);
    depth = format.GetColourDepth();
}