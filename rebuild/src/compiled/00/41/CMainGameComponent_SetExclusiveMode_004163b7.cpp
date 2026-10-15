// CMainGameComponent::SetExclusiveMode(bool exclusive) -- notify the display object (+0x28), switch the system manager's exclusive (full-screen) mode, then re-run the post-resolution-change setup with the back surface's width, height and colour depth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004163B7, length 104 bytes.
// Callees: 0x00434A10 on the object at +0x28 (no arguments; manifest "CScriptThing::operator==" is propagated),
//          GFGetSystemManager @ 0x009A4EC0, CSystemManager::SetExclusiveMode(bool) @ 0x009A59A0,
//          CSurface::GetDimensions @ 0x009F2F90 (C2DExtentsI by hidden pointer), CSurface::GetPixelFormat
//          @ 0x009F2F60 (CPixelFormat by hidden pointer), CPixelFormat::GetColourDepth @ 0x009E3820,
//          CSurface::GetHeight @ 0x00415BF0, CMainGameComponent::PostChangeResolution(ulong, ulong, ulong) @ 0x00416365.
// Globals: the display device at 0x013B8390, its back surface embedded at +0x2c.
// Shape: size TU with a frame pointer (`optimize("s", on)` + `("y", off)`); the width is a local read before the
// call's other arguments (which evaluate right to left: depth, then height); the surface pointer local serves the
// width and the height while the pixel format goes through the global again; the CPixelFormat temporary lands in
// the dead argument slot.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CSystemManager {
    bool SetExclusiveMode(bool exclusive);         // 0x009A59A0
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct C2DExtentsI { long Width; long Height; };
struct CPixelFormat {
    unsigned long m_format;
    unsigned long GetColourDepth() const;          // 0x009E3820
};
struct CSurface {
    C2DExtentsI   GetDimensions() const;           // 0x009F2F90
    CPixelFormat  GetPixelFormat() const;          // 0x009F2F60
    unsigned long GetHeight() const;               // 0x00415BF0
};
struct CDisplayDevice {
    unsigned char _pad00[0x2c];
    CSurface      m_backSurface;                   // +0x2c
};
extern CDisplayDevice* g_pDisplayDevice;           // 0x013B8390
struct CDisplayHolder {
    void OnExclusiveModeChange();                  // 0x00434A10
};
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayHolder* m_pDisplay;                    // +0x28
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x00416365
    void SetExclusiveMode(bool exclusive);
};

void CMainGameComponent::SetExclusiveMode(bool exclusive)
{
    m_pDisplay->OnExclusiveModeChange();
    GFGetSystemManager()->SetExclusiveMode(exclusive);
    CSurface* surface = &g_pDisplayDevice->m_backSurface;
    unsigned long width = surface->GetDimensions().Width;
    PostChangeResolution(width, surface->GetHeight(), g_pDisplayDevice->m_backSurface.GetPixelFormat().GetColourDepth());
}