// CDisplayManager::GetClosestMode(const C2DExtentsI& resolution, unsigned long colourDepth, bool fullScreen) -- the display mode
//   to use: windowed returns the requested size with the current mode's refresh rate and format (from the device when one
//   exists, else the adapter); full screen walks every back-buffer format's adapter modes and keeps the one with the smallest
//   colour-depth error, ties broken by the smallest |dY|+|dX| resolution error (later modes win ties).
// Leaf: GetClosestMode
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BF600, 467 bytes (+int3 padding), __fastcall with a hidden _D3DDISPLAYMODE result, `ret 0x10`.
// Callees: IDirect3DDevice9::GetDisplayMode (slot 8), IDirect3D9::GetAdapterModeCount / EnumAdapterModes / GetAdapterDisplayMode
//   (slots 6/7/8), CPixelFormat::Initialise 0x009E3830, CPixelFormat::GetColourDepth 0x009E3820; the pixel-format table 0x0129BA40.
// Layout: as the landed CDisplayManager::GetCurrentWindowedSettings 0x009BED10 / IsValidRenderTargetFormat 0x009BE930 (D3D +0x54,
//   D3DDevice +0x58, AdapterOrdinal +0x60); BackBufferFormats is Ego_r.pdb's CArray<CPixelFormat> at +0x2a0 but retail's 12-byte
//   STLport vectors put it at +0x298 -- hence a local model (engine/ has no CDisplayManager.h).
// Shape: the mode count is re-queried in the inner loop condition; only best.Format is initialised (D3DFMT_UNKNOWN); abs() is the
//   cdq/xor/sub intrinsic; the windowed result is the queried mode with Width/Height overwritten, returned by value.
#include <stdlib.h>
struct _D3DDISPLAYMODE {
    unsigned long Width;
    unsigned long Height;
    unsigned long RefreshRate;
    unsigned long Format;                          // D3DFORMAT
};

struct IDirect3D9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall RegisterSoftwareDevice(void* initializeFunction);
    virtual unsigned long __stdcall GetAdapterCount();
    virtual long __stdcall GetAdapterIdentifier(unsigned long adapter, unsigned long flags, void* identifier);
    virtual unsigned long __stdcall GetAdapterModeCount(unsigned long adapter, unsigned long format);                       // slot 6
    virtual long __stdcall EnumAdapterModes(unsigned long adapter, unsigned long format, unsigned long mode, _D3DDISPLAYMODE* out);   // slot 7
    virtual long __stdcall GetAdapterDisplayMode(unsigned long adapter, _D3DDISPLAYMODE* mode);                           // slot 8
};
struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall TestCooperativeLevel();
    virtual unsigned long __stdcall GetAvailableTextureMem();
    virtual long __stdcall EvictManagedResources();
    virtual long __stdcall GetDirect3D(IDirect3D9** d3d);
    virtual long __stdcall GetDeviceCaps(void* caps);
    virtual long __stdcall GetDisplayMode(unsigned long swapChain, _D3DDISPLAYMODE* mode);                                 // slot 8
};

class C2DExtentsI { public: long X; long Y; };

struct SPixelFormatInfo {              // 36-byte format table entry
    unsigned long m_d3dFormat;         // +0 D3DFORMAT
    unsigned char _rest[0x20];
};
extern SPixelFormatInfo g_PixelFormatTable[];   // 0x0129BA40

class CPixelFormat { public:
    long FormatIndex;
    void Initialise(unsigned long format);         // 0x009E3830
    unsigned long GetColourDepth();                // 0x009E3820
    unsigned long GetD3DFormat() const { return g_PixelFormatTable[FormatIndex].m_d3dFormat; }
};

struct CPixelFormatArray {                         // STLport vector<CPixelFormat>
    CPixelFormat* _M_start;
    CPixelFormat* _M_finish;
    CPixelFormat* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const CPixelFormat& operator[](unsigned int n) const { return *(_M_start + n); }
};

class CDisplayManager { public:
    unsigned char     _pad0[0x54];
    IDirect3D9*       D3D;                         // +0x54
    IDirect3DDevice9* D3DDevice;                   // +0x58
    unsigned long     DeviceType;                  // +0x5c D3DDeviceCaps.DeviceType
    unsigned long     AdapterOrdinal;              // +0x60 D3DDeviceCaps.AdapterOrdinal
    unsigned char     _pad64[0x298 - 0x64];
    CPixelFormatArray BackBufferFormats;           // +0x298
    _D3DDISPLAYMODE GetClosestMode(const C2DExtentsI& resolution, unsigned long colourDepth, bool fullScreen);
};

_D3DDISPLAYMODE CDisplayManager::GetClosestMode(const C2DExtentsI& resolution, unsigned long colourDepth, bool fullScreen)
{
    if (!fullScreen) {
        _D3DDISPLAYMODE mode;
        if (D3DDevice)
            D3DDevice->GetDisplayMode(0, &mode);
        else
            D3D->GetAdapterDisplayMode(AdapterOrdinal, &mode);
        mode.Width = resolution.X;
        mode.Height = resolution.Y;
        return mode;
    }
    int bestResolutionError = 0x7fffffff;
    int bestDepthError = 0x7fffffff;
    _D3DDISPLAYMODE best;
    best.Format = 0;
    for (unsigned int i = 0; i < BackBufferFormats.size(); ++i) {
        for (unsigned int j = 0; j < D3D->GetAdapterModeCount(AdapterOrdinal, BackBufferFormats[i].GetD3DFormat()); ++j) {
            _D3DDISPLAYMODE mode;
            D3D->EnumAdapterModes(AdapterOrdinal, BackBufferFormats[i].GetD3DFormat(), j, &mode);
            CPixelFormat format;
            format.Initialise(mode.Format);
            int depthError = abs((int)(format.GetColourDepth() - colourDepth));
            if (depthError <= bestDepthError) {
                int resolutionError = abs(resolution.Y - (int)mode.Height) + abs(resolution.X - (int)mode.Width);
                if (resolutionError <= bestResolutionError) {
                    best = mode;
                    bestResolutionError = resolutionError;
                    bestDepthError = depthError;
                }
            }
        }
    }
    return best;
}