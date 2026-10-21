// Grade: DIFFER(342v342) -- frame slot order: retail puts the client position at esp+8 below origin/cursor and keeps origin.y in esi (y difference first); ours puts origin lowest and keeps origin.x in esi
// CMouseDX::ResetNonExclusiveMouse() -- resynchronise the non-exclusive mouse with Windows: if the cursor is inside the client area, take its client position as Pos (when it lies on the back buffer) and re-read the five button states
// Leaf: ResetNonExclusiveMouse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB5030, length 342 bytes (padded to 352).
// Ego_r.pdb: `?ResetNonExclusiveMouse@CMouseDX@@IAEXXZ`.
// Callees: GFGetSystemManager @ 0x009A4EC0 (display manager +0x60, window handle +0x94), user32 GetCursorPos (IAT 0x014403A8),
//          ClientToScreen (IAT 0x01440304), GetClientRect (IAT 0x01440338), CSurface::GetDimensions() const @ 0x009F2F90,
//          CSystemManager::IsMouseButton0..4Pressed @ 0x009A4FC0/FD0/FE0/FF0/0x009A5000 (manifest labels
//          "CTCInventoryBase::GetCanItemBeQuickAssigned" are propagated; the bodies read CSystemManager +0xdd..+0xe1).
// Layout: CMouseDX retail sits 8 bytes below the PDB (landed PeekScreenPos 0x00AB4870): Pos +0x3414,
//   NonExclusiveOldButton0..4Pressed +0x342c..+0x3430, NonExclusiveOldMouseX/Y +0x3434/+0x3438. CSystemManager's mouse
//   button bytes sit 8 above the PDB (+0xdd..+0xe1).
// Shape: the cursor is made client-relative in place and copied into a C2DCoordI; the old-position members are stored
//   first and Pos takes the same converted values.
struct HWND__;
struct POINT { long x; long y; };
struct RECT { long left; long top; long right; long bottom; };
extern "C" __declspec(dllimport) int __stdcall GetCursorPos(POINT* point);                    // IAT 0x014403A8
extern "C" __declspec(dllimport) int __stdcall ClientToScreen(HWND__* window, POINT* point);   // IAT 0x01440304
extern "C" __declspec(dllimport) int __stdcall GetClientRect(HWND__* window, RECT* rect);      // IAT 0x01440338

struct C2DVector {
    float X;
    float Y;
};

struct C2DCoordI {
    long X;
    long Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
    C2DCoordI(const C2DCoordI& o) : X(o.X), Y(o.Y) {}
    C2DCoordI operator-(const C2DCoordI& o) const { return C2DCoordI(X - o.X, Y - o.Y); }
};

inline C2DCoordI ToClient(POINT& p, const POINT& o) { p.x -= o.x; p.y -= o.y; return C2DCoordI(p.x, p.y); }

struct C2DExtentsI {
    long Width;
    long Height;
};

struct CSurface {
    C2DExtentsI GetDimensions() const;              // 0x009F2F90
};

struct CDisplayManager {
    unsigned char _pad00[0x2c];
    CSurface      BackBuffer;                       // +0x2c
};

struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                // +0x60
    unsigned char    _pad64[0x30];
    HWND__*          m_hWnd;                        // +0x94
    bool IsMouseButton0Pressed();                   // 0x009A4FC0
    bool IsMouseButton1Pressed();                   // 0x009A4FD0
    bool IsMouseButton2Pressed();                   // 0x009A4FE0
    bool IsMouseButton3Pressed();                   // 0x009A4FF0
    bool IsMouseButton4Pressed();                   // 0x009A5000
};
CSystemManager* __fastcall GFGetSystemManager();    // 0x009A4EC0

class CMouseDX {
public:
    virtual ~CMouseDX();
    unsigned char _pad04[0x3410];
    C2DVector     Pos;                              // +0x3414
    unsigned char _pad341c[0x10];
    bool          NonExclusiveOldButton0Pressed;    // +0x342c
    bool          NonExclusiveOldButton1Pressed;    // +0x342d
    bool          NonExclusiveOldButton2Pressed;    // +0x342e
    bool          NonExclusiveOldButton3Pressed;    // +0x342f
    bool          NonExclusiveOldButton4Pressed;    // +0x3430
    float         NonExclusiveOldMouseX;            // +0x3434
    float         NonExclusiveOldMouseY;            // +0x3438

    void ResetNonExclusiveMouse();
};

void CMouseDX::ResetNonExclusiveMouse()
{
    if (GFGetSystemManager()->DisplayManager == 0)
        return;
    POINT cursor;
    POINT origin;
    RECT client;
    HWND__* window = GFGetSystemManager()->m_hWnd;
    GetCursorPos(&cursor);
    origin.x = 0;
    origin.y = 0;
    ClientToScreen(window, &origin);
    GetClientRect(window, &client);
    if (cursor.x >= origin.x && cursor.x < client.right + origin.x
        && cursor.y >= origin.y && cursor.y < client.bottom + origin.y) {
        C2DCoordI pos(cursor.x - origin.x, cursor.y - origin.y);
        CSystemManager* system = GFGetSystemManager();
        C2DExtentsI extents = system->DisplayManager->BackBuffer.GetDimensions();
        if (pos.X >= 0 && pos.X < extents.Width && pos.Y >= 0 && pos.Y < extents.Height) {
            NonExclusiveOldMouseX = (float)pos.X;
            NonExclusiveOldMouseY = (float)pos.Y;
            Pos.X = NonExclusiveOldMouseX;
            Pos.Y = NonExclusiveOldMouseY;
        }
        NonExclusiveOldButton0Pressed = GFGetSystemManager()->IsMouseButton0Pressed();
        NonExclusiveOldButton1Pressed = GFGetSystemManager()->IsMouseButton1Pressed();
        NonExclusiveOldButton2Pressed = GFGetSystemManager()->IsMouseButton2Pressed();
        NonExclusiveOldButton3Pressed = GFGetSystemManager()->IsMouseButton3Pressed();
        NonExclusiveOldButton4Pressed = GFGetSystemManager()->IsMouseButton4Pressed();
    }
}
