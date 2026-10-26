// CMouseDX::PeekScreenPos() const -- the mouse position in screen space: in exclusive mode the tracked Pos as is, otherwise Pos offset by the window's client rectangle moved to screen coordinates
// Leaf: PeekScreenPos
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB4870, 152 bytes (`ret 4`: the C2DVector result by hidden pointer).
// Ego_r.pdb: `?PeekScreenPos@CMouseDX@@UBE?AVC2DVector@@XZ`.
// Callees: GFGetSystemManager @ 0x009A4EC0 (window handle at +0x94), user32 ClientToScreen (IAT 0x01440304),
// GetClientRect (IAT 0x01440338), OffsetRect (IAT 0x014403A4) -- names from the import table.
// Layout: Ego_r.pdb CMouseDX has Pos +0x341c and ExclusiveMode +0x3444; retail sits 8 bytes lower (Pos +0x3414,
// ExclusiveMode +0x343c), so `this` is a local model.
// Shape: the exclusive arm copies Pos word by word (C2DVector has a value ctor but no user copy ctor); the other arm
// builds the result with the value ctor (arguments right to left: y first); the origin POINT is zero-initialised.
struct HWND__;
struct POINT { long x; long y; };
struct RECT { long left; long top; long right; long bottom; };
extern "C" __declspec(dllimport) int __stdcall ClientToScreen(HWND__* window, POINT* point);   // IAT 0x01440304
extern "C" __declspec(dllimport) int __stdcall GetClientRect(HWND__* window, RECT* rect);      // IAT 0x01440338
extern "C" __declspec(dllimport) int __stdcall OffsetRect(RECT* rect, int dx, int dy);         // IAT 0x014403A4

class CSystemManager { public:
    unsigned char _pad00[0x94];
    HWND__*       m_hWnd;                           // +0x94
};
CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0

class C2DVector { public:
    float x;
    float y;
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
};

struct CMouseDX {
    virtual C2DVector PeekScreenPos() const;
    unsigned char _pad04[0x3410];
    C2DVector     Pos;                              // +0x3414
    unsigned char _pad341c[0x20];
    bool          ExclusiveMode;                    // +0x343c
};

C2DVector CMouseDX::PeekScreenPos() const
{
    if (ExclusiveMode)
        return Pos;
    HWND__* window = GFGetSystemManager()->m_hWnd;
    POINT origin;
    origin.x = 0;
    origin.y = 0;
    ClientToScreen(window, &origin);
    RECT client;
    GetClientRect(window, &client);
    OffsetRect(&client, origin.x, origin.y);
    return C2DVector((float)client.left + Pos.x, (float)client.top + Pos.y);
}