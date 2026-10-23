// CFontBank::GetVStringExtents(const CWideString& text) -- string extents in virtual-screen units
// Leaf: ?GetVStringExtents@CFontBank@@QBE?AVC2DExtentsF@@ABVCWideString@@@Z
// Retail: Fable.exe @ 0x00437210, 76 bytes, `ret 8` (hidden return pointer); Ego_r.pdb
// ?GetVStringExtents@CFontBank@@QBE?AVC2DExtentsF@@ABVCWideString@@@Z. Catalog label GetVStringWidth is wrong: the width-only
// inline copy is 0x0043E4D0 (`ret 4`, returns float); this one stores both floats through the return pointer.
// Body: the virtual GetStringExtents (slot 1, after ??_E) on the
// raw wide characters (CWideString::operator const wchar_t* 0x0099AF10) with no wrap, then GFAXToVX (0x009E1DA0) on the
// width and GFAYToVY (0x009E1DD0) on the height; both are stored to the result.
// Shape: the converted width written back into the extents local, the height converted inside the return constructor.
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
float __fastcall GFAYToVY_009e1dd0(float y);                      // 0x009E1DD0
class CFontManager;
class CFontBank {
public:
    unsigned long FontHeight;                            // +0x04
    void*         Name_PStringData;                      // +0x08
    CFontManager* PManager;                              // +0x0c
    virtual ~CFontBank();
    virtual C2DExtentsF GetStringExtents(const wchar_t* text, bool wrap) const;
    C2DExtentsF GetVStringExtents(const CWideString& text) const;
    float GetVStringWidth(const CWideString& text) const;
};
C2DExtentsF CFontBank::GetVStringExtents(const CWideString& text) const
{
    C2DExtentsF extents = GetStringExtents(text, false);
    extents.Width = GFAXToVX(extents.Width);
    return C2DExtentsF(extents.Width, GFAYToVY_009e1dd0(extents.Height));
}