// CDisplayManager::GetBestSignedTextureFormat(long, CPixelFormat&) -- pick the first signed texture format of the requested bit depth that the device supports on the current mode
// Leaf: ?GetBestSignedTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE590, length 126 bytes.
// Callees: IDirect3D9::CheckDeviceFormat (COM slot 10, `call [ecx+28h]`, `this` pushed), CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830.
// Globals: the pixel-format table @ 0x0129BA40 (36-byte CD3DFormatInfo records ended by BitDepth -1; PFType 6 = signed).
// Layout: PDB CDisplayManager: D3D +0x54, D3DDeviceCaps.DeviceType +0x5c / AdapterOrdinal +0x60, CurrentMode.Format +0x1c4.
// Shape: the walk is an indexed `for` over the table (VC strength-reduces it to a record pointer; the entry test reads the
// global's BitDepth directly; a pointer-cursor loop compiles differently), it does not stop at the
// first hit but keeps the first supported record (`best == 0` is part of the filter, and VC reuses that zero register as the
// Usage argument); the result is `out.Initialise(best->D3DFormat)` + true, else false.
// IDirect3D9: the shared header (d3d9.h slot order and SDK parameter types; engine/IDirect3D9.h).
#include "engine/IDirect3D9.h"













enum { D3DRTYPE_TEXTURE = 3 };

#include "engine/CD3DFormatInfo.h"   // CD3DFormatInfo + GD3DFormats 0x0129BA40 (NF28: was a local struct model and g_D3DFormatInfo)


class CPixelFormat { public:
    long FormatIndex;                              // +0
    void Initialise(_D3DFORMAT d3dFormat);               // 0x009E3830
};

class CDisplayManager { public:
    unsigned char _pad[0x54];                      // vptr slot, SystemManager, PRenderManager, surfaces, back-buffer copy texture
    IDirect3D9*   D3D;                             // +0x54
    void*         D3DDevice;                       // +0x58
    D3DDEVTYPE    DeviceType;                      // +0x5c D3DDeviceCaps.DeviceType
    unsigned long AdapterOrdinal;                  // +0x60 D3DDeviceCaps.AdapterOrdinal
    unsigned char _caps[0x1c4 - 0x64];             // rest of the caps, extents, flags, formats, window handle, mode size
    D3DFORMAT     CurrentModeFormat;               // +0x1c4 CurrentMode.Format
    bool GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat& out);
};

bool CDisplayManager::GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat& out)
{
    const CD3DFormatInfo* best = 0;
    for (long i = 0; GD3DFormats[i].BitDepth != -1; ++i) {
        const CD3DFormatInfo& info = GD3DFormats[i];
        if (info.PFType == 6 && info.BitDepth == bitDepth && best == 0
            && D3D->CheckDeviceFormat(AdapterOrdinal, DeviceType, CurrentModeFormat, 0, D3DRTYPE_TEXTURE, info.D3DFormat) >= 0)
            best = &info;
    }
    if (best != 0) {
        out.Initialise(best->D3DFormat);
        return true;
    }
    return false;
}