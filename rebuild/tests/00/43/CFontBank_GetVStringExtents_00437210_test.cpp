#include <cstdio>
#include <stddef.h>
class CWideString {
public:
    void* PStringData;
    const wchar_t* GetAsCharArray() const;                     // 0x0099AF10
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
    C2DExtentsF extents = GetStringExtents(text.GetAsCharArray(), false);
    extents.Width = GFAXToVX(extents.Width);
    return C2DExtentsF(extents.Width, GFAYToVY_009e1dd0(extents.Height));
}

static const wchar_t g_empty[1] = {0};
const wchar_t* CWideString::GetAsCharArray() const { return PStringData ? (const wchar_t*)PStringData : g_empty; }
float __fastcall GFAXToVX(float x) { return x * 2.0f; }
float __fastcall GFAYToVY_009e1dd0(float y) { return y * 3.0f; }
CFontBank::~CFontBank() {}
static const wchar_t* g_seen; static bool g_wrap = true;
C2DExtentsF CFontBank::GetStringExtents(const wchar_t* text, bool wrap) const { g_seen = text; g_wrap = wrap; return C2DExtentsF(10.0f, 20.0f); }
int main() {
    CFontBank bank; CWideString s; s.PStringData = 0;
    C2DExtentsF e = bank.GetVStringExtents(s);
    if (e.Width == 20.0f && e.Height == 60.0f && g_seen == g_empty && !g_wrap) printf("00437210_TEST PASS\n");
    return 0;
}