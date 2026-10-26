// CMainGameComponent::SetDisplayMode(unsigned long width, unsigned long height, unsigned char bitsPerPixel) -- switch the display manager to a new mode, then re-run the post-resolution-change setup with the back surface's real width, height and colour depth
// Leaf: ?SetDisplayMode@CMainGameComponent@@QAEXKKE@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041641F, length 125 bytes, `ret 0Ch` after `leave`.
// Name: Ego_r.pdb / FableWin.pdb `?SetDisplayMode@CMainGameComponent@@QAEXKKE@Z` (width, height, an unsigned char
// depth that is widened with `movzx` for the display manager); the catalog's "ChangeDisplayMode" is descriptive.
// Callees: CDisplayEngine::PreChangeResolution @ 0x00434A10 on PDisplayEngine (+0x28; the manifest's propagated
//          "CScriptThing::operator==" label), CDisplayManager::SetDisplayMode(const C2DExtentsI&, ulong,
//          ESurfaceMultisampleType, ulong) @ 0x009BFF40, CSurface::GetDimensions @ 0x009F2F90 (C2DExtentsI by hidden
//          pointer), CSurface::GetPixelFormat @ 0x009F2F60 (CPixelFormat by hidden pointer),
//          CPixelFormat::GetColourDepth @ 0x009E3820, CSurface::GetHeight @ 0x00415BF0,
//          CMainGameComponent::PostChangeResolution(ulong, ulong, ulong) @ 0x00416365.
// Globals: the display manager at 0x013B8390, its back surface embedded at +0x2c.
// Shape: the same tail as the landed SetExclusiveMode (0x004163B7): size TU with a frame pointer
// (`optimize("s", on)` + `("y", off)`); the C2DExtentsI(width, height) temporary is built in [ebp-8] and its slot is
// reused for the GetDimensions result; the width is a local read before the call's other arguments (which
// evaluate right to left: depth, then height); the surface pointer local (edi) serves width and height while the
// pixel format goes through the global again, its CPixelFormat temporary homed in the dead `bitsPerPixel` slot.
// Model: local -- engine/CDisplayEngine.h, CPixelFormat.h, CSurface.h and CWorld.h exist, but the generated header is data-only
// (pack(1), no member functions, no virtuals: the vptr and counted pointers are flattened into raw words); this body calls their
// member functions and constructors, so the classes are declared locally with only the members it touches.
#pragma optimize("s", on)
#pragma optimize("y", off)
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };
class C2DExtentsI { public:
    long Width;
    long Height;
    C2DExtentsI(long width, long height) : Width(width), Height(height) {}
};
class CPixelFormat { public:
    unsigned long m_format;
    unsigned long GetColourDepth() const;          // 0x009E3820
};
class CSurface { public:
    C2DExtentsI   GetDimensions() const;           // 0x009F2F90
    CPixelFormat  GetPixelFormat() const;          // 0x009F2F60
    unsigned long GetHeight() const;               // 0x00415BF0
};
enum ECreateDeviceSuccess { CREATE_DEVICE_OK = 0 };
class CDisplayManager { public:
    unsigned char _pad00[0x2c];
    CSurface      m_backSurface;                   // +0x2c
    ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& resolution, unsigned long bitsPerPixel,
                                        ESurfaceMultisampleType antialiasing, unsigned long refreshRate);   // 0x009BFF40
};
extern CDisplayManager* g_pDisplayManager;         // 0x013B8390
class CDisplayEngine {
public:
    void PreChangeResolution();                    // 0x00434A10
};
class CWorld;
class CMainGameComponent {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x00416365
    void SetDisplayMode(unsigned long width, unsigned long height, unsigned char bitsPerPixel);
    unsigned char   _pad00[0x24];
    CWorld*         PWorld;                        // +0x24 scoped_ptr<CWorld>
    CDisplayEngine* PDisplayEngine;                // +0x28 scoped_ptr<CDisplayEngine>
};

void CMainGameComponent::SetDisplayMode(unsigned long width, unsigned long height, unsigned char bitsPerPixel)
{
    PDisplayEngine->PreChangeResolution();
    g_pDisplayManager->SetDisplayMode(C2DExtentsI(width, height), bitsPerPixel, SURFACE_MULTISAMPLE_NONE, 0);
    CSurface* surface = &g_pDisplayManager->m_backSurface;
    unsigned long newWidth = surface->GetDimensions().Width;
    PostChangeResolution(newWidth, surface->GetHeight(), g_pDisplayManager->m_backSurface.GetPixelFormat().GetColourDepth());
}