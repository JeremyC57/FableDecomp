// CDisplayManager::GetBestSignedTextureFormat(long, CPixelFormat&) -- pick the first signed texture format of the requested bit depth that the device supports on the current mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE590, length 126 bytes.
// Callees: IDirect3D9::CheckDeviceFormat (COM slot 10, `call [ecx+28h]`, `this` pushed), CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830.
// Globals: the pixel-format table @ 0x0129BA40 (36-byte CD3DFormatInfo records ended by BitDepth -1; PFType 6 = signed).
// Layout: PDB CDisplayManager: D3D +0x54, D3DDeviceCaps.DeviceType +0x5c / AdapterOrdinal +0x60, CurrentMode.Format +0x1c4.
// Shape: the walk is an indexed `for` over the table (VC strength-reduces it to a record pointer; the entry test reads the
// global's BitDepth directly; a pointer-cursor loop compiles differently), it does not stop at the
// first hit but keeps the first supported record (`best == 0` is part of the filter, and VC reuses that zero register as the
// Usage argument); the result is `out.Initialise(best->D3DFormat)` + true, else false.
struct IDirect3D9 {                                // COM: IUnknown + IDirect3D9 slots up to CheckDeviceFormat
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall RegisterSoftwareDevice(void* initializeFunction);
    virtual unsigned long __stdcall GetAdapterCount();
    virtual long __stdcall GetAdapterIdentifier(unsigned long adapter, unsigned long flags, void* identifier);
    virtual unsigned long __stdcall GetAdapterModeCount(unsigned long adapter, long format);
    virtual long __stdcall EnumAdapterModes(unsigned long adapter, long format, unsigned long mode, void* displayMode);
    virtual long __stdcall GetAdapterDisplayMode(unsigned long adapter, void* displayMode);
    virtual long __stdcall CheckDeviceType(unsigned long adapter, long devType, long adapterFormat, long backBufferFormat, int windowed);
    virtual long __stdcall CheckDeviceFormat(unsigned long adapter, long devType, long adapterFormat, unsigned long usage,
                                             long resourceType, long checkFormat);   // slot 10 (+0x28)
};

enum { D3DRTYPE_TEXTURE = 3 };

struct CD3DFormatInfo {                            // 36-byte entry of the format table (see the landed CPixelFormat::Initialise 0x009E3790)
    long D3DFormat;                                // +0x00 _D3DFORMAT
    long PFType;                                   // +0x04 EPixelFormatType (2 RGB, 6 signed)
    long BitDepth;                                 // +0x08 (-1 terminates the table)
    long AlphaBits;                                // +0x0c
    long RBits;                                    // +0x10
    long GBits;                                    // +0x14
    long BBits;                                    // +0x18
    long StencilBits;                              // +0x1c
    bool Linear;                                   // +0x20
};

extern const CD3DFormatInfo g_D3DFormatInfo[];     // 0x0129BA40

struct CPixelFormat {
    long FormatIndex;                              // +0
    void Initialise(long d3dFormat);               // 0x009E3830
};

struct CDisplayManager {
    unsigned char _pad[0x54];                      // vptr slot, SystemManager, PRenderManager, surfaces, back-buffer copy texture
    IDirect3D9*   D3D;                             // +0x54
    void*         D3DDevice;                       // +0x58
    long          DeviceType;                      // +0x5c D3DDeviceCaps.DeviceType
    unsigned long AdapterOrdinal;                  // +0x60 D3DDeviceCaps.AdapterOrdinal
    unsigned char _caps[0x1c4 - 0x64];             // rest of the caps, extents, flags, formats, window handle, mode size
    long          CurrentModeFormat;               // +0x1c4 CurrentMode.Format
    bool GetBestSignedTextureFormat(long bitDepth, CPixelFormat& out);
};

bool CDisplayManager::GetBestSignedTextureFormat(long bitDepth, CPixelFormat& out)
{
    const CD3DFormatInfo* best = 0;
    for (long i = 0; g_D3DFormatInfo[i].BitDepth != -1; ++i) {
        const CD3DFormatInfo& info = g_D3DFormatInfo[i];
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