// CDisplayManager::ResizeWindowToBackBufferSize() -- in windowed mode (and unless attached to an external window) resize the game window so its client area matches the back buffer, keeping the client origin
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE490, length 148 bytes.
// Callees: user32 imports GetSystemMetrics @ 0x0144038C (held in ebp for the three calls), ClientToScreen @ 0x01440304,
// MoveWindow @ 0x01440334.
// Layout: PDB CDisplayManager: SystemManager +4, BackBufferDimensions +0x18c (X) / +0x190 (Y), ExclusiveMode +0x1a0, WindowHandle
// +0x1b4. CSystemManager AttachedToExternalWindow is the byte at +0xec (PDB +228; the shipped manager is 8 bytes wider before
// it, see the landed CSystemManager::IsApplicationActive 0x009A57B0).
// Shape: the frame borders are 2 * SM_CXFRAME (edi) and 2 * SM_CYFRAME + SM_CYCAPTION (`lea ebp,[eax+ecx*2]`, the y frame
// spilled to the frame across the caption call); the client origin is a zeroed POINT converted with ClientToScreen.
struct HWND__;

struct POINT {
    long x;
    long y;
};

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int index);                          // 0x0144038C
extern "C" __declspec(dllimport) int __stdcall ClientToScreen(HWND__* window, POINT* point);         // 0x01440304
extern "C" __declspec(dllimport) int __stdcall MoveWindow(HWND__* window, int x, int y, int width, int height,
                                                          int repaint);                              // 0x01440334

enum { SM_CYCAPTION = 4, SM_CXFRAME = 32, SM_CYFRAME = 33 };

struct CSystemManager {
    unsigned char _pad[0xec];
    bool          AttachedToExternalWindow;        // +0xec
};

struct C2DExtentsI {
    long X;                                        // +0x00
    long Y;                                        // +0x04
};

struct CDisplayManager {
    void*           _vtable;                       // +0x00
    CSystemManager* SystemManager;                 // +0x04
    unsigned char   _pad08[0x18c - 8];
    C2DExtentsI     BackBufferDimensions;          // +0x18c
    unsigned char   _pad194[0x1a0 - 0x194];
    bool            ExclusiveMode;                 // +0x1a0
    unsigned char   _pad1a1[0x1b4 - 0x1a1];
    HWND__*         WindowHandle;                  // +0x1b4
    void ResizeWindowToBackBufferSize();
};

void CDisplayManager::ResizeWindowToBackBufferSize()
{
    if (!ExclusiveMode && !SystemManager->AttachedToExternalWindow) {
        long borderWidth = GetSystemMetrics(SM_CXFRAME) * 2;
        long borderHeight = GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
        POINT origin;
        origin.x = 0;
        origin.y = 0;
        ClientToScreen(WindowHandle, &origin);
        MoveWindow(WindowHandle, origin.x, origin.y, BackBufferDimensions.X + borderWidth, BackBufferDimensions.Y + borderHeight, 1);
    }
}
