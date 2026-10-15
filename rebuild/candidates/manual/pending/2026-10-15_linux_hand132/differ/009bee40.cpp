// CDisplayManager::SetupDepthAndBackBufferTextures() -- create the back-buffer copy texture in the render target's pixel format, 1024x1024 when the device needs power-of-two textures, else the render-target size
// Grade: DIFFER(101v101) -- register/scheduling only: retail loads RenderTargetDimensions.Y into ecx (ours edx) and stores the sunk X before `push 1` with &size formed in eax (ours after it, in edx); tried X-first, Y-first and a constructor temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BEE40, length 101 bytes.
// Callees: CSurface::GetPixelFormat() @ 0x009F2F60 (CPixelFormat by hidden pointer), CTexture::Initialise(const C2DExtentsI&, long,
// const CPixelFormat&, unsigned long, ESurfacePool, bool, bool) @ 0x009FA280.
// Layout: PDB CDisplayManager: RenderTargetSurface +0xc, BackBufferCopyTexture +0x4c, D3DDeviceCaps +0x5c (TextureCaps at caps +0x3c
// = +0x98; bit 1 is D3DPTEXTURECAPS_POW2), RenderTargetDimensions +0x194 (X) / +0x198 (Y).
// Shape: the 12-byte frame holds the pixel format then the extents; the POW2 test is the byte-narrowed `(caps >> 1) & 1`
// (`shr ecx,1 ; test cl,1`); the extents' X stays in eax across both arms and is stored once before the call; the bool result
// is renormalised (`test al,al ; setne al`) by `if (!Initialise(..)) return false; return true;`. Writing Y before X sinks the X
// store to the join like retail (X first sinks Y; a C2DExtentsI(x, y) temporary duplicates the stores per arm).
struct CPixelFormat {
    unsigned long m_format;
    CPixelFormat() { }
};

struct C2DExtentsI {
    long X;                                        // +0x00
    long Y;                                        // +0x04
};

enum ESurfacePool { SURFACE_POOL_DEFAULT = 0 };

struct CSurface {                                  // 16 bytes
    unsigned char _body[0x10];
    CPixelFormat GetPixelFormat();                 // 0x009F2F60
};

struct CTexture {                                  // 8 bytes
    unsigned char _body[8];
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, ESurfacePool pool,
                    bool dynamic, bool renderTarget);   // 0x009FA280
};

struct CDisplayManager {
    unsigned char _pad0[0xc];                      // vptr slot, SystemManager, PRenderManager
    CSurface      RenderTargetSurface;             // +0x0c
    unsigned char _pad1c[0x4c - 0x1c];             // RenderTargetDepthBuffer, BackBufferSurface, DepthBufferSurface
    CTexture      BackBufferCopyTexture;           // +0x4c
    unsigned char _pad54[0x98 - 0x54];             // D3D, D3DDevice, caps DeviceType .. ShadeCaps
    unsigned long TextureCaps;                     // +0x98 (D3DDeviceCaps.TextureCaps)
    unsigned char _pad9c[0x194 - 0x9c];
    C2DExtentsI   RenderTargetDimensions;          // +0x194
    bool SetupDepthAndBackBufferTextures();
};

bool CDisplayManager::SetupDepthAndBackBufferTextures()
{
    CPixelFormat format = RenderTargetSurface.GetPixelFormat();
    C2DExtentsI size;
    if ((unsigned char)(TextureCaps >> 1) & 1) {
        size.Y = 0x400;
        size.X = 0x400;
    }
    else {
        size.Y = RenderTargetDimensions.Y;
        size.X = RenderTargetDimensions.X;
    }
    if (!BackBufferCopyTexture.Initialise(size, 1, format, 1, SURFACE_POOL_DEFAULT, false, false))
        return false;
    return true;
}
