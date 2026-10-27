// CProgressDisplay::GetCharWidthW(unsigned short c) -- word-wrap callback: the width of one character in the CProgressDisplay font, in whole virtual units
// Grade: DIFFER(119v120) -- retail spills the GFAXToVX result into the dead extent.X slot (frame 0xC) and loads the font pointer via eax then esi; every C++ spelling tried keeps a separate float slot (frame 0x10) and loads esi directly
// Leaf: ?GetCharWidthW@CProgressDisplay@@CIIG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00498410, 120 bytes, `__fastcall` static (the character in cx), plain `ret`.
// Identity (proven): its address is taken by CProgressDisplay::DrawRetailDisplay 0x00498490 (`mov ecx, 498410h` before the word-wrap setup call 0x00A0FD90); Ego_r.pdb ?GetCharWidthW@CProgressDisplay@@CII_W@Z (private static __fastcall) between FormatTextForVWindow and DrawRetailDisplay; the font is CProgressDisplay::PFont (Ego_r.pdb dynamic initializer/atexit for it).
// Callees: CWideString() 0x0099AED0, FormatWideString(CWideString*, const wchar_t*, ...) 0x0099BA70 (L"%c" 0x01238154),
//   CWideString::operator const wchar_t* 0x0099AF10, the font's vtable slot 1 GetTextExtent(text, 0) (C2DVector by hidden
//   pointer), GFAXToVX 0x009E1DA0, GFAYToVY 0x009E1DD0 (both __fastcall float), ~CWideString 0x0099B510, __ftol2 0x00BFEA70.
// Global: the class font 0x013B8998 (`mov eax,[g]` = the bank pointer, its vtable in edi).
// Shape (family M074, representative 0x00498410): one-character string, formatted, measured, both axes converted to virtual
//   units (the Y result discarded), the string destroyed, then X truncated. wchar_t is `unsigned short` (no /Zc:wchar_t).
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
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    ~CCountedPointer();
    T* operator->() const { return Data; }
    T* Data;
    CCPPointerInfo* Info;
};
class CProgressDisplay {
    static unsigned int __fastcall GetCharWidthW(unsigned short c);
private:
    static CCountedPointer<CFontBank> PFont;                // 0x013B8998
};
unsigned int __fastcall CProgressDisplay::GetCharWidthW(unsigned short c)
{
    CWideString text;
    FormatWideString(&text, L"%c", c);
    C2DVector extent = PFont->GetTextExtent(text, 0);
    float width = GFAXToVX(extent.X);
    GFAYToVY(extent.Y);
    return (unsigned int)width;
}
