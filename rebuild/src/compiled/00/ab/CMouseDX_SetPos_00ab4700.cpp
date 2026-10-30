// CMouseDX::SetPos(const C2DVector* position) -- store the position and its cached copy, then (unless cursor warping is disabled) move the OS cursor to the window's client origin plus the position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB4700, 174 bytes, `ret 4`.
// Callees: ResolveMouseWindowOwner @ 0x009A4EC0 (free helper; its +0x94 is the window handle), ClientToScreen (iat 0x01440304),
//          GetClientRect (iat 0x01440338), OffsetRect (iat 0x014403A4), SetCursorPos (iat 0x014403A0), _ftol @ 0x00BFEA70.
// Layout: CMouseDX PositionX +0x3414, PositionY +0x3418, CachedPositionX +0x3434, CachedPositionY +0x3438, DisableCursorWarp +0x343c.
// Shape: the cached copies are read back from the just-stored members (store-to-load forwarding gives retail's `mov eax,ecx`
//        register copies with ONE load of each component; re-reading `position->X` reloads it, a float local goes x87);
//        the two cursor coordinates are `const int&` locals so the X conversion is evaluated first and kept in edi
//        (plain int locals are sunk to the pushes, Y first).
// Self-contained: engine structs inlined, engine helpers/globals extern-declared.

typedef unsigned char u8;

class C2DVector { public: float X; float Y; };

struct CMousePoint { long x; long y; };
struct CMouseRect { long left; long top; long right; long bottom; };

struct CMouseWindowOwner
{
    u8   pad00[0x94];
    void* Window;   // +0x94
};

// Free helper: retail emits a relocation-masked relative `call` returning the
// owner whose +0x94 field is the OS window handle used for coordinate mapping.
extern "C" CMouseWindowOwner* __cdecl ResolveMouseWindowOwner(void);

// USER32 imports (retail: `call DWORD PTR ds:0x14403xx`, the IAT slots of the dllimport declarations).
extern "C" __declspec(dllimport) int __stdcall ClientToScreen(void*, CMousePoint*); // ds:0x1440304 = USER32.dll!ClientToScreen
extern "C" __declspec(dllimport) int __stdcall GetClientRect(void*, CMouseRect*);   // ds:0x1440338 = USER32.dll!GetClientRect
extern "C" __declspec(dllimport) int __stdcall OffsetRect(CMouseRect*, int, int);   // ds:0x14403a4 = USER32.dll!OffsetRect
extern "C" __declspec(dllimport) int __stdcall SetCursorPos(int, int);              // ds:0x14403a0 = USER32.dll!SetCursorPos

struct CMouseDX
{
    u8    pad000[0x3414];
    float PositionX;        // +0x3414
    float PositionY;        // +0x3418
    u8    pad341C[0x18];
    float CachedPositionX;  // +0x3434
    float CachedPositionY;  // +0x3438
    u8    DisableCursorWarp; // +0x343c
};

// __fastcall(thisptr in ecx, unused edx, position) models the __thiscall member.
void __fastcall CMouseDX_SetPos(CMouseDX* mouseDX, void* /*edx*/, const C2DVector* position)
{
    mouseDX->PositionX = position->X;
    mouseDX->PositionY = position->Y;
    mouseDX->CachedPositionX = mouseDX->PositionX;
    mouseDX->CachedPositionY = mouseDX->PositionY;

    if (mouseDX->DisableCursorWarp != 0)
        return;

    void* window = ResolveMouseWindowOwner()->Window;

    CMousePoint pt;
    pt.x = 0;
    pt.y = 0;
    ClientToScreen(window, &pt);

    CMouseRect rect;
    GetClientRect(window, &rect);
    OffsetRect(&rect, pt.x, pt.y);

    const int& x = (int)((float)rect.left + mouseDX->PositionX);
    const int& y = (int)((float)rect.top + mouseDX->PositionY);
    SetCursorPos(x, y);
}
