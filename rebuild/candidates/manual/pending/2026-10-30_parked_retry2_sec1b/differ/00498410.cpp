// CProgressDisplay::GetCharWidthW(unsigned short c) -- word-wrap callback: the width of one character in the CProgressDisplay font, in whole virtual units
// Grade: DIFFER(119v120) sec1b 2026-10-30 -- with `float width` declared outside a block that holds the CWideString text and the extent, the GFAX/GFAY calls, the ~CWideString and the late `fld width ; __ftol2` now run in retail's order; left: width keeps its own frame word (frame 0x10 v 0xC: retail spills it into the dead extent.X slot) and PFont is loaded straight into esi (retail via eax). Tried also: extent.X = GFAXToVX(extent.X) (123: ftol before the dtor), C2DVector size(GFAX, GFAY) (119), extent outside the block with a default ctor (116: X/Y read through the returned pointer). Next: a form that lets width share the extent slot.
// Leaf: ?GetCharWidthW@CProgressDisplay@@CIIG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00498410, 120 bytes, `__fastcall` static (the character in cx), plain `ret`.
// Identity (proven): its address is taken by CProgressDisplay::DrawRetailDisplay 0x00498490 (`mov ecx, 498410h` before the word-wrap setup call 0x00A0FD90); Ego_r.pdb ?GetCharWidthW@CProgressDisplay@@CII_W@Z (private static __fastcall) between FormatTextForVWindow and DrawRetailDisplay; the font is CProgressDisplay::PFont (Ego_r.pdb dynamic initializer/atexit for it).
// Callees: CWideString() 0x0099AED0, FormatWideString(CWideString*, const wchar_t*, ...) 0x0099BA70 (L"%c" 0x01238154),
//   CWideString::operator const wchar_t* 0x0099AF10, the font's vtable slot 1 GetTextExtent(text, 0) (C2DVector by hidden
//   pointer), GFAXToVX 0x009E1DA0, GFAYToVY 0x009E1DD0 (both __fastcall float), ~CWideString 0x0099B510, __ftol2 0x00BFEA70.
// Global: the class font 0x013B8998 (`mov eax,[g]` = the bank pointer, its vtable in edi).
// Shape (family M074, representative 0x00498410): one-character string, formatted, measured, both axes converted to virtual
//   units (the Y result discarded), the string destroyed, then X truncated. wchar_t is `unsigned short` (no /Zc:wchar_t).
#include "engine/CCountedPointer.h"
class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    operator const unsigned short*() const;                 // 0x0099AF10
};
void __cdecl FormatWideString(CWideString* text, const unsigned short* format, ...);   // 0x0099BA70
float __fastcall GFAXToVX(float x);                         // 0x009E1DA0
float __fastcall GFAYToVY(float y);                         // 0x009E1DD0
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CFontBank {
public:
    virtual void VirtualSlot00();
    virtual C2DVector GetTextExtent(const unsigned short* text, long flags);   // slot 1 (+4)
};
class CProgressDisplay {
    static unsigned int __fastcall GetCharWidthW(unsigned short c);
private:
    static CCountedPointer<CFontBank> PFont;                // 0x013B8998
};
unsigned int __fastcall CProgressDisplay::GetCharWidthW(unsigned short c)
{
    float width;
    {
        CWideString text;
        FormatWideString(&text, L"%c", c);
        C2DVector extent = PFont->GetTextExtent(text, 0);
        width = GFAXToVX(extent.X);
        GFAYToVY(extent.Y);
    }
    return (unsigned int)width;
}
