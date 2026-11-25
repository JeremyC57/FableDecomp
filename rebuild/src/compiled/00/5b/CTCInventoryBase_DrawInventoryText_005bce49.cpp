// CTCInventoryBase::DrawInventoryText(C2DVector, CFontBank*, CWideString*, ETextLayout, CRGBColour*, bool) -- converts the GFA position to virtual screen coordinates, shifts it left for centred (1) or right-aligned (2) text, and draws the string
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: this row's object is byte-identical without it (size mode keeps the
//   leaf's frame; T3 nf416 audit).
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCE49, length 91 bytes.
// Callees: GFACoordToVCoord(C2DVector* in, C2DVector* out) @ 0x009E21D0 (`__fastcall`, both the by-value position),
//          CFontBank::GetVStringWidth(const CWideString&) @ 0x0043E4D0 (float),
//          Draw2DText(C2DVector*, CWideString*, CFontBank*, CRGBColour*, float, bool) @ 0x005BCAFE (`__fastcall`).
//          The 0.5f multiplier is the .rdata float @ 0x0122F59C.
// Pragmas: frame-pointer size TU (`optimize("s", on)`).
// Shape: `this` is unused (`ret 1Ch` for the seven argument dwords); the by-value position is converted in place
//        (`lea edx,[ebp+8] ; mov ecx,edx`); the two alignment arms tail-merge their `pos.X -= width` store
//        (`fsubr [ebp+8] ; fstp [ebp+8]`), the centred arm scaling the width by 0.5f first; the 1.0f scale is
//        materialised with `fld1` into a `push ecx` slot.

class C2DVector { public:
    float X;                                      // +0x00
    float Y;                                      // +0x04
};

class CWideString;
class CRGBColour;

class CFontBank { public:
public:
    float GetVStringWidth(const CWideString&) const;   // 0x0043E4D0
};

void __fastcall Draw2DText(C2DVector* position, CWideString* text, CFontBank* font, CRGBColour* colour,
                           float scale, bool shadow);                    // 0x005BCAFE

enum ETextLayout {
    TEXT_LAYOUT_LEFT = 0,
    TEXT_LAYOUT_CENTRE = 1,
    TEXT_LAYOUT_RIGHT = 2
};

struct CTCInventoryBase_Retail {
    void DrawInventoryText(C2DVector position, CFontBank* font, CWideString* text, ETextLayout layout,
                           CRGBColour* colour, bool shadow);
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009E21D0 ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z (was ?GFACoordToVCoord@@YIXPBVC2DVector@@PAV1@@Z)
void __fastcall GFACoordToVCoord(C2DVector const&, C2DVector&);
void CTCInventoryBase_Retail::DrawInventoryText(C2DVector position, CFontBank* font, CWideString* text,
                                                ETextLayout layout, CRGBColour* colour, bool shadow)
{
    GFACoordToVCoord(position, position);
    if (layout == TEXT_LAYOUT_CENTRE)
        position.X -= font->GetVStringWidth(*text) * 0.5f;
    else if (layout == TEXT_LAYOUT_RIGHT)
        position.X -= font->GetVStringWidth(*text);
    Draw2DText(&position, text, font, colour, 1.0f, shadow);
}