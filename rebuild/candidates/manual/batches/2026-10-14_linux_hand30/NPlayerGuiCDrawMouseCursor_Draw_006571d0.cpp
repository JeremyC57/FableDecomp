// NPlayerGui::CDrawMouseCursor::Draw(long) -- when the mouse cursor is globally visible, mark this drawer as drawing and touch the GUI definition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006571D0, length 21 bytes (+ int3 padding).
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments; its result is not used -- the rest of the drawing was compiled out, the call survives).
// The global visibility byte is at 0x0137575B. CDrawBase::Drawing is the byte at +0x18 (PDB CDrawBase: PlayerGui +4 .. Bank +0x14, Drawing +0x18).
// The one stack argument (`ret 4`) is unused (the drawer's Draw(...) signature); it is modelled as a long. Shape: `mov al,[flag] ; test ; je ; mov byte ptr [ecx+18h],1 ; call`.
struct CPlayerGuiDef;
extern CPlayerGuiDef* __fastcall PeekGuiDef();    // 0x00438570
extern bool g_mouseCursorVisible_0137575b;         // 0x0137575B

namespace NPlayerGui {

struct CDrawMouseCursor {
    unsigned char _pad[0x18];                      // vptr and the cached GUI pointers
    bool          Drawing;                         // +0x18 (CDrawBase::Drawing)
    void Draw(long unused);
};

void CDrawMouseCursor::Draw(long)
{
    if (g_mouseCursorVisible_0137575b) {
        Drawing = true;
        PeekGuiDef();
    }
}

}
