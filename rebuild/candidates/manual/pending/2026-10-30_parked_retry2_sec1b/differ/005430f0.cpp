// Grade: DIFFER(226v1081) (sec1b 2026-10-31) -- partial reconstruction, the first 0x30 bytes exact: Font (+0x164) GetTextExtent(L" ") / (L"  ") space width (literals read from the binary), the icon space count with the retail_asm ceiling helper (0.5f/1.0f frame words, bare fistp), Text (+0x15c) length/chars and the buffer new[]; left: retail keeps the icon run width on the x87 stack adding it->Width from memory and re-reads IconsFinish each pass (its run test compares it->Position with itself, not folded), and the whole copy/primitive loop (about 850 bytes) is not written
// CIconText::InitIcons() -- lay out the inline icons of an icon-text string: copy the text into a wide buffer, replace
//   every icon marker with spaces sized to the icon, and add one icon primitive per marker to the renderer
// Leaf: InitIcons
// Retail-asm: 0x0054317E..0x005431A9 `fld [ebp-20h] ; fadd [ebp-24h] ; fistp dword ptr [ebp-14h] ; fild [ebp-14h] ; fsub [ebp-28h] ; fstp [ebp-1Ch] ; mov eax,[ebp-20h] ; cmp eax,[ebp-1Ch] ; jne ; dec dword ptr [ebp-14h]` -- the engine's inline-assembly ceiling helper (the mirror of the FloorToLong block of GFFloatToLongFloor 0x0048D940), a bare dword fistp VC7.1 does not emit from C++.
#include <stddef.h>
#pragma optimize("s", on)
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float previous;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  previous
        mov   eax, value
        cmp   eax, previous
        jne   done
        dec   result
    done:
    }
    return result;
}
void* __cdecl operator new[](unsigned int bytes);       // 0x00BFEB22
struct C2DVector {
    float X, Y;
    C2DVector() { }
};
struct CWideString {
    void* PStringData;
    long GetLength() const;                            // 0x0099B220
    operator const wchar_t*() const;                   // 0x0099AF10
};
struct CFont {
    virtual void v00();
    virtual C2DVector GetTextExtent(const wchar_t* text, long flags) const;   // slot 1 (+0x04)
};
struct CIconTextIcon {                                 // 0x1c bytes
    long  _unk00;
    long  Position;                                    // +0x04
    long  Type;                                        // +0x08 (1 = an inline icon)
    float Width;                                       // +0x0c
    unsigned char _pad10[0x1c - 0x10];
};
class CIconText {
public:
    unsigned char  _pad00[0x15c];
    CWideString    Text;                               // +0x15c
    unsigned char  _pad160[0x164 - 0x160];
    CFont*         Font;                               // +0x164
    unsigned char  _pad168[0x18c - 0x168];
    CIconTextIcon* IconsStart;                         // +0x18c
    CIconTextIcon* IconsFinish;                        // +0x190
    void InitIcons();
};
void CIconText::InitIcons()
{
    CFont* font = Font;
    float single = font->GetTextExtent(L" ", 0).X;
    float spaceWidth = font->GetTextExtent(L"  ", 0).X - single;
    long extraSpaces = 0;
    for (CIconTextIcon* it = IconsStart; it != IconsFinish; ) {
        if (it->Type != 1) {
            ++it;
            continue;
        }
        float width = it->Width;
        CIconTextIcon* next = it + 1;
        for (; next != IconsFinish && it->Position == it->Position; ++next)
            width += it->Width;
        extraSpaces += CeilToLong(width / spaceWidth);
        it = next;
    }
    long length = Text.GetLength();
    const wchar_t* text = Text;
    wchar_t* buffer = new wchar_t[length + extraSpaces + 1];
    buffer[0] = text[0];
}
