// CFontBank::GetVStringWidth(const CWideString& text) -- width of a string in virtual-screen units
// Leaf: ?GetVStringWidth@CFontBank@@QBEMABVCWideString@@@Z
// Retail: Fable.exe @ 0x0043E4D0, 69 bytes, `ret 4`; Ego_r.pdb ?GetVStringWidth@CFontBank@@QBEMABVCWideString@@@Z.
// Body: GetVStringExtents (out of line at 0x00437210) inlined: the virtual GetStringExtents (slot 1, after ??_E) on the
// raw wide characters (CWideString::operator const wchar_t* 0x0099AF10) with no wrap, then GFAXToVX (0x009E1DA0) on the
// width and GFAYToVY (0x009E1DD0) on the height; only the width is returned, the height result is popped.
// Layout: CFontBank (Ego_r.pdb, 0x10) declared with its virtuals so the slot call compiles; the generated header models
// the vptr as a data member. C2DExtentsF {float Width, Height} has constructors (returned through memory).
#include <stddef.h>
class CWideString {
public:
    void* PStringData;
    operator const wchar_t*() const;                     // 0x0099AF10
};
class C2DExtentsF {
public:
    float Width;
    float Height;
    C2DExtentsF() {}
    C2DExtentsF(float width, float height) : Width(width), Height(height) {}
};
float __fastcall GFAXToVX(float x);                      // 0x009E1DA0
float __fastcall GFAYToVY(float y);                      // 0x009E1DD0
class CFontManager;
class CFontBank {
public:
    unsigned long FontHeight;                            // +0x04
    void*         Name_PStringData;                      // +0x08
    CFontManager* PManager;                              // +0x0c
    virtual ~CFontBank();
    virtual C2DExtentsF GetStringExtents(const wchar_t* text, bool wrap) const;
    C2DExtentsF GetVStringExtents(const CWideString& text) const
    {
        C2DExtentsF extents = GetStringExtents(text, false);
        extents.Width = GFAXToVX(extents.Width);
        return C2DExtentsF(extents.Width, GFAYToVY(extents.Height));
    }
    float GetVStringWidth(const CWideString& text) const;
};
float CFontBank::GetVStringWidth(const CWideString& text) const
{
    return GetVStringExtents(text).Width;
}