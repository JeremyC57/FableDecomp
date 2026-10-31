// CDisplayManager::SetupDepthAndBackBufferTextures() -- create the back-buffer copy texture in the render target's pixel format: 1024x1024 when the device needs power-of-two textures, else the render-target size
// Grade: DIFFER(101v101) -- register choice + store schedule: retail loads RenderTargetDimensions.Y into ecx (ours edx) in the else arm and forms &format in edx with the sunk X store after `push edx` (ours stores X before `lea eax` and uses eax/ecx for the two address pushes); 14 masked lines
// Leaf: ?SetupDepthAndBackBufferTextures@CDisplayManager@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BEE40, length 101 bytes, thiscall, plain `ret`.
// Identity: FableWin.pdb ?SetupDepthAndBackBufferTextures@CDisplayManager@@AAE_NXZ (private).
// Callees: CSurface::GetPixelFormat() const 0x009F2F60 (?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ, hidden-pointer result:
//   CPixelFormat has a user constructor), CTexture::Initialise(const C2DExtentsI&, long, const CPixelFormat&, unsigned long,
//   ESurfacePool, bool, bool) 0x009FA280 (PDB ?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z).
// Layout: PDB CDisplayManager: RenderTargetSurface +0xc, BackBufferCopyTexture +0x4c, D3DDeviceCaps +0x5c (TextureCaps at +0x98,
//   bit 1 = D3DPTEXTURECAPS_POW2), RenderTargetDimensions +0x194. No engine header for CDisplayManager.
// Shape: the named X local sinks the X store to the join (retail stores it once, after `push edx`); Y is stored per arm.
//   C2DExtentsI must stay an aggregate here: giving it constructors (PDB class) stops the sinking (107 bytes).
class CPixelFormat {
public:
    long FormatIndex;                              // +0x00
    CPixelFormat() { }
};

class C2DExtentsI {
public:
    long X;                                        // +0x00
    long Y;                                        // +0x04
};

enum ESurfacePool { SURFACE_POOL_DEFAULT = 0 };

class CSurface {                                   // 16 bytes
public:
    unsigned char _body[0x10];
    CPixelFormat GetPixelFormat() const;           // 0x009F2F60
};

class CTexture {                                   // 8 bytes
public:
    unsigned char _body[8];
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, ESurfacePool pool,
                    bool dynamic, bool renderTarget);   // 0x009FA280
};

class CDisplayManager {
public:
    unsigned char _pad0[0xc];                      // vptr slot, SystemManager, PRenderManager
    CSurface      RenderTargetSurface;             // +0x0c
    unsigned char _pad1c[0x4c - 0x1c];             // RenderTargetDepthBuffer, BackBufferSurface, DepthBufferSurface
    CTexture      BackBufferCopyTexture;           // +0x4c
    unsigned char _pad54[0x98 - 0x54];             // D3D, D3DDevice, caps DeviceType .. ShadeCaps
    unsigned long TextureCaps;                     // +0x98 (D3DDeviceCaps.TextureCaps)
    unsigned char _pad9c[0x194 - 0x9c];
    C2DExtentsI   RenderTargetDimensions;          // +0x194
private:
    bool SetupDepthAndBackBufferTextures();
};
bool CDisplayManager::SetupDepthAndBackBufferTextures()
{
    CPixelFormat format = RenderTargetSurface.GetPixelFormat();
    C2DExtentsI size;
    long x;
    if ((unsigned char)(TextureCaps >> 1) & 1) {
        x = 0x400;
        size.Y = 0x400;
    }
    else {
        x = RenderTargetDimensions.X;
        size.Y = RenderTargetDimensions.Y;
    }
    size.X = x;
    if (!BackBufferCopyTexture.Initialise(size, 1, format, 1, SURFACE_POOL_DEFAULT, false, false))
        return false;
    return true;
}
