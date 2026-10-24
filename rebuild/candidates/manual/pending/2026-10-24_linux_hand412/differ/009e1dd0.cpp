// GFAYToVY(float) -- convert an actual (render-target pixel) Y to the virtual 65536-unit Y space: y * VirtualHeight / renderTargetHeight
// Grade: DIFFER(49v49) -- operand order of the scaling product: retail `fld qword [g_VirtualHeight] ; fmul dword [y]` with the `fstp` after `push ecx`, ours `fld dword [y] ; fmul qword [g]` with the `fstp` before it (the landed GFAXToVX 0x009E1DA0 needs a never-cached (cv) qualifier on the global for the same order)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1DD0, length 49 bytes (+ int3 padding).
// PDB (FableWin): ?GFAYToVY@@YIMM@Z -- __fastcall with the float on the stack (`ret 4`).
// Callees: GFGetSystemManager (0x009A4EC0, __fastcall, the system manager; +0x60 is the display manager) and
// CDisplayManager::GetRenderTargetDimensions (0x009BEDC0, __thiscall, C2DExtentsI by hidden pointer into the
// 8-byte frame temporary). Global: the virtual height, double 65536.0 at 0x013961F0.
// Shape: unlike GFAXToVX (0x009E1DA0, product formed after the dimension call) the scaled value is formed BEFORE the
// dimension call and parked, as a float, in the dead argument slot (`fstp [esp+10h]` ... `fdivr [esp+0Ch]`): the
// source scales the argument in its own statement between the two calls.
// Tried: `y * g`, `g * y`, an inline `GetVirtualHeight()` getter (all load the float first), `/Op` (adds rounding stores).
struct C2DExtentsI
{
    long X;
    long Y;
};

class CDisplayManager
{
public:
    C2DExtentsI GetRenderTargetDimensions() const;     // 0x009BEDC0
};

class CSystemManager
{
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* m_pDisplayManager;                // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0
extern double g_VirtualHeight;                         // 0x013961F0 (65536.0)

float __fastcall GFAYToVY(float y)
{
    CDisplayManager* display = GFGetSystemManager()->m_pDisplayManager;
    y = (float)(y * g_VirtualHeight);
    return y / display->GetRenderTargetDimensions().Y;
}
