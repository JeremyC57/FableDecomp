// CDisplayManager::InitVirtualPixels() -- switch to resolution-independent virtual coordinates: whole virtual pixels per screen pixel (65536 / size, floored) and the resulting virtual screen size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE980, length 130 bytes.
// Callees: floor @ 0x00BFEB8A (the CRT import thunk, called directly). Globals (doubles in .data): the virtual pixel scales
// @ 0x013CA800 (x) / 0x013CA808 (y), the virtual screen width @ 0x013961E8 and height @ 0x013961F0 (read by the landed
// 2D-coordinate helpers). Constant 65536.0 (double) @ 0x012387D8.
// Layout: PDB CDisplayManager: RenderTargetDimensions +0x194 (X) / +0x198 (Y), VirtualCoordsResolutionIndependant +0x254
// (PDB +596; the members before it match the PDB).
// Shape: statement by statement on the globals: each scale is stored, re-loaded for floor's double argument (one 8-byte argument
// slot opened once, `sub esp,8 ; ... add esp,8`) and stored again, then the extents are multiplied by the floored scales
// (`fild ; fmul [global]`).
#pragma function(floor)
extern "C" double __cdecl floor(double value);     // 0x00BFEB8A

extern double g_virtualPixelScaleX;                // 0x013CA800
extern double g_virtualPixelScaleY;                // 0x013CA808
extern double g_virtualScreenWidth;                // 0x013961E8
extern double g_virtualScreenHeight;               // 0x013961F0

struct C2DExtentsI {
    long X;                                        // +0x00
    long Y;                                        // +0x04
};

struct CDisplayManager {
    unsigned char _pad0[0x194];
    C2DExtentsI   RenderTargetDimensions;          // +0x194
    unsigned char _pad19c[0x254 - 0x19c];
    bool          VirtualCoordsResolutionIndependant;   // +0x254
    void InitVirtualPixels();
};

void CDisplayManager::InitVirtualPixels()
{
    VirtualCoordsResolutionIndependant = true;
    g_virtualPixelScaleX = 65536.0 / RenderTargetDimensions.X;
    g_virtualPixelScaleY = 65536.0 / RenderTargetDimensions.Y;
    g_virtualPixelScaleX = floor(g_virtualPixelScaleX);
    g_virtualPixelScaleY = floor(g_virtualPixelScaleY);
    g_virtualScreenWidth = RenderTargetDimensions.X * g_virtualPixelScaleX;
    g_virtualScreenHeight = RenderTargetDimensions.Y * g_virtualPixelScaleY;
}