// CVolumeTexture::Initialise(const C3DExtentsI& extents, long no_levels, const CPixelFormat& format, unsigned long usage, EVolumePool pool, bool flag) -- create the D3D volume texture: uninitialise, round each extent up to a power of two, force one mip level, D3DXCreateVolumeTexture, then count its size
// Leaf: ?Initialise@CVolumeTexture@@QAE_NABVC3DExtentsI@@JABVCPixelFormat@@KW4EVolumePool@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A68370, 613 bytes, thiscall `ret 18h`.
// Identity: Ego_r.pdb / FableWin.pdb `?Initialise@CVolumeTexture@@QAE_NABVC3DExtentsI@@JABVCPixelFormat@@KW4EVolumePool@@_N@Z`
//   (the manifest row carries the same signature); Ego_r donor order CalcByteLength, Uninitialise, ..., CreateTexture,
//   operator=, InitialiseFromPreallocatedVolumeTexture, Initialise matches retail 0xA67FF0, 0xA68060, ..., 0xA68250,
//   0xA682A0, 0xA682E0, 0xA68370.
// Callees: CVolumeTexture::Uninitialise @ 0x00A68060, GFGetSystemManager @ 0x009A4EC0 (device = PDisplayManager->D3DDevice,
//   +0x60 / +0x58), D3DXCreateVolumeTexture @ 0x00BFEFF2, CVolumeTexture::CalcByteLength @ 0x00A67FF0 (protected),
//   CMemoryAllocator::GetAllocationSize @ 0x009F2390 (inline Sizeof()).
// Globals: format table 0x0129BA40 (36-byte Ego_r CD3DFormatInfo entries), the global allocator 0x013CAA90,
//   the volume-texture byte counter 0x013D28E0 (descriptive names, the spellings the landed siblings bind).
// Shape: CreateTexture (0x00A68250, protected, its retail body) is defined in this TU and auto-inlined (/Ob2), as in
//   retail. The three `1 << top-bit search` blocks are FableWin's GFGetNextPowerOfTwo / GFGetTopBit (both __fastcall
//   there, inline here: they need __forceinline). `C3DExtentsI size(extents);` (FableWin's user copy ctor) loads X, Y, Z
//   up front; X lands in the push-ecx slot, Y in the dead `extents` slot, Z in edi. `if (no_levels != 1) no_levels = 1;`
//   gives retail's `cmp ebx,1 ; je ; mov ebx,1`; the device is read as `PDisplayManager.Data->D3DDevice` (operator-> adds `add eax,60h`). Sizeof() (Ego_r `?Sizeof@CVolumeTexture@@QBEKXZ`) is the landed
//   InitialiseFromPreallocatedVolumeTexture's inline size body.
struct IDirect3DDevice9;
struct IDirect3DVolumeTexture9;
class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                   // +0x00
    CCPPointerInfo* Info;                   // +0x04
    T* operator->() const { return Data; }
};
class CDisplayManager {
public:
    unsigned char     _pad00[0x58];
    IDirect3DDevice9* D3DDevice;            // +0x58
};
class CSystemManager {
public:
    unsigned char                    _pad00[0x60];
    CCountedPointer<CDisplayManager> PDisplayManager;   // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();        // 0x009A4EC0

extern "C" long __stdcall D3DXCreateVolumeTexture(IDirect3DDevice9* device, unsigned int width, unsigned int height,
    unsigned int depth, unsigned int mip_levels, unsigned long usage, long format, long pool,
    IDirect3DVolumeTexture9** volume_texture);         // 0x00BFEFF2 (d3dx9 import)

struct CD3DFormatInfo {                     // 36-byte entry (Ego_r)
    long D3DFormat;                         // +0x00 _D3DFORMAT
    long PFType;                            // +0x04 EPixelFormatType
    long BitDepth;                          // +0x08
    long AlphaBits;                         // +0x0c
    long RBits;                             // +0x10
    long GBits;                             // +0x14
    long BBits;                             // +0x18
    long StencilBits;                       // +0x1c
    bool Linear;                            // +0x20
};
extern const CD3DFormatInfo g_D3DFormatInfo[];          // 0x0129BA40

class CPixelFormat {
public:
    long FormatIndex;                       // +0x00
};
class C2DExtentsI {
public:
    long X;                                 // +0x00
    long Y;                                 // +0x04
};
class C3DExtentsI : public C2DExtentsI {
public:
    long Z;                                 // +0x08
    C3DExtentsI(const C3DExtentsI& extents) { X = extents.X; Y = extents.Y; Z = extents.Z; }
};
enum EVolumePool { };

class CMemoryAllocator {
public:
    unsigned long GetAllocationSize(void* block);       // 0x009F2390
};
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90
extern unsigned long g_VolumeTextureMemoryBytes;        // 0x013D28E0

__forceinline unsigned long __fastcall GFGetTopBit(unsigned long value)
{
    if (value == 0)
        return (unsigned long)-1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}
__forceinline unsigned long __fastcall GFGetNextPowerOfTwo(unsigned long value)
{
    if (value == 0)
        return 0;
    return 1 << GFGetTopBit(value * 2 - 1);
}

class CVolumeTexture {
public:
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* PD3DVolumeTexture;    // +0x04
    long                     ByteLength;           // +0x08
    void*                    PData;                // +0x0c

    unsigned long Sizeof() const
    {
        unsigned long size = 0;
        if (PD3DVolumeTexture) {
            if (PData)
                size = g_pGlobalAllocator_013caa90->GetAllocationSize(PData) + 4;
            else
                return ByteLength + 4;
        }
        return size;
    }
    void Uninitialise();                           // 0x00A68060
    bool Initialise(const C3DExtentsI& extents, long no_levels, const CPixelFormat& format, unsigned long usage,
                    EVolumePool pool, bool flag);
protected:
    void CalcByteLength();                         // 0x00A67FF0
    bool CreateTexture(const C3DExtentsI& size, long no_levels, unsigned long usage, const CPixelFormat& format,
                       EVolumePool pool, bool flag);   // 0x00A68250
};

bool CVolumeTexture::CreateTexture(const C3DExtentsI& size, long no_levels, unsigned long usage, const CPixelFormat& format,
                                   EVolumePool pool, bool)
{
    IDirect3DDevice9* device = GFGetSystemManager()->PDisplayManager.Data->D3DDevice;
    long d3d_format = g_D3DFormatInfo[format.FormatIndex].D3DFormat;
    return D3DXCreateVolumeTexture(device, size.X, size.Y, size.Z, no_levels, usage, d3d_format, pool,
                                   &PD3DVolumeTexture) >= 0 ? true : false;
}

bool CVolumeTexture::Initialise(const C3DExtentsI& extents, long no_levels, const CPixelFormat& format, unsigned long usage,
                                EVolumePool pool, bool flag)
{
    Uninitialise();
    C3DExtentsI size(extents);
    size.X = GFGetNextPowerOfTwo(size.X);
    size.Y = GFGetNextPowerOfTwo(size.Y);
    size.Z = GFGetNextPowerOfTwo(size.Z);
    if (no_levels != 1)
        no_levels = 1;
    if (!CreateTexture(size, no_levels, usage, format, pool, flag))
        return false;
    CalcByteLength();
    g_VolumeTextureMemoryBytes += Sizeof();
    return true;
}