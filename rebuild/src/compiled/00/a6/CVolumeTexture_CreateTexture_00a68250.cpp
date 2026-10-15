// CVolumeTexture::CreateTexture(const C3DVectorI& size, unsigned long mipLevels, unsigned long usage, const CPixelFormat& format, long pool, long flags) -- D3DXCreateVolumeTexture on the render device into m_pTexture; true on success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A68250, length 80 bytes. `ret 18h` (six arguments, the sixth unused).
// Leaf: CreateTexture
// Callees: GFGetSystemManager @ 0x009A4EC0 (device = system +0x60 -> +0x58), D3DXCreateVolumeTexture @ 0x00BFEFF2 (static D3DX,
// __stdcall). The format's D3DFORMAT comes from the 0x24-byte pixel-format table @ 0x0129BA40 (`lea eax,[eax+eax*8]` then `*4`).
// Shape: the device fetched first, the D3D format read into a named local next (before `add esi,4` forms &m_pTexture), then
// the arguments right to left; `test eax,eax ; setge al` needs `cond ? true : false` (a bare bool return goes through ecx).
struct CRenderStatistics {
    void RemoveTexture(void* texture, float weight);    // 0x00A0AA70
};
struct IDirect3DDevice9;
struct CRenderSystem {
    unsigned char      _pad00[8];
    CRenderStatistics* m_pStatistics;       // +0x08
    unsigned char      _pad0c[0x58 - 0x0c];
    IDirect3DDevice9*  m_pDevice;           // +0x58
};
struct CSystemManager {
    unsigned char  _pad00[0x60];
    CRenderSystem* m_pRender;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct IDirect3DVolumeTexture9;
extern "C" long __stdcall D3DXCreateVolumeTexture(IDirect3DDevice9* device, unsigned long width, unsigned long height,
    unsigned long depth, unsigned long mipLevels, unsigned long usage, long format, long pool, IDirect3DVolumeTexture9** out);  // 0x00BFEFF2
struct C3DVectorI { long X, Y, Z; };
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };   // 0x24 bytes
extern SPixelFormatInfo g_pixelFormatInfo[];                 // 0x0129BA40
struct CPixelFormat { long m_format; };
struct CVolumeTexture {
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* m_pTexture;    // +0x04
    long                     m_format;      // +0x08
    long                     m_levels;      // +0x0c
    bool CreateTexture(const C3DVectorI& size, unsigned long mipLevels, unsigned long usage, const CPixelFormat& format, long pool, long flags);
};
bool CVolumeTexture::CreateTexture(const C3DVectorI& size, unsigned long mipLevels, unsigned long usage, const CPixelFormat& format, long pool, long)
{
    IDirect3DDevice9* device = GFGetSystemManager()->m_pRender->m_pDevice;
    long d3dFormat = g_pixelFormatInfo[format.m_format].d3dFormat;
    return D3DXCreateVolumeTexture(device, size.X, size.Y, size.Z, mipLevels, usage, d3dFormat, pool, &m_pTexture) >= 0 ? true : false;
}