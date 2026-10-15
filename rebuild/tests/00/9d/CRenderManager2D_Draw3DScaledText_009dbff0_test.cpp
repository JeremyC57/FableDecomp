// Behaviour test for CRenderManager2D::Draw3DScaledText @ 0x009DBFF0: forwards to AddText, destroys its string copy, returns true.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CFontBank { int m_id; };
static int g_live = 0;
struct CWideString {
    const char* m_text;
    CWideString(const char* t) : m_text(t) { ++g_live; }
    CWideString(const CWideString& o) : m_text(o.m_text) { ++g_live; }
    ~CWideString() { --g_live; }
};
struct CRenderManager2D {
    int m_calls; float m_scale; const char* m_text; const CFontBank* m_font; unsigned long m_flags; unsigned char m_red;
    void AddText(const C3DVector&, float scale, const CWideString& text, CFontBank* font, const CRGBColour& colour, unsigned long flags)
    { ++m_calls; m_scale = scale; m_text = text.m_text; m_font = font; m_flags = flags; m_red = colour.R; }
    bool Draw3DScaledText(const C3DVector& position, float scale, CWideString text, CFontBank* font, const CRGBColour& colour, unsigned long flags)
    {
        AddText(position, scale, text, font, colour, flags);
        return true;
    }
};

int main()
{
    CRenderManager2D r; r.m_calls = 0;
    C3DVector p = { 1.0f, 2.0f, 3.0f }; CRGBColour c = { 1, 2, 200, 255 }; CFontBank f = { 4 };
    bool ok;
    {
        CWideString s("Hello");
        ok = r.Draw3DScaledText(p, 1.5f, s, &f, c, 9);
        if (g_live != 1) { std::printf("009dbff0_TEST FAIL\n"); return 1; }   // the by-value copy is gone again
    }
    if (!ok || r.m_calls != 1 || r.m_scale != 1.5f || r.m_text[0] != 'H' || r.m_font != &f || r.m_flags != 9 || r.m_red != 200 || g_live != 0) {
        std::printf("009dbff0_TEST FAIL\n"); return 1;
    }
    std::printf("009dbff0_TEST PASS\n");
    return 0;
}