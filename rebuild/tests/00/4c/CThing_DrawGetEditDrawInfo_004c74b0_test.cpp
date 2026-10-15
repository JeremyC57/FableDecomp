// Behaviour test for CThing::DrawGetEditDrawInfo @ 0x004C74B0: fills player number, grey opaque colour, empty label, no highlight.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* m_text;
    CCharString& operator=(const char* text) { m_text = text; return *this; }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct SEditDrawInfo {
    CCharString Label;
    CRGBColour  Colour;
    long        PlayerNumber;
    bool        Highlighted;
};
struct CThing {
    signed char PlayerNumber;
    bool DrawGetEditDrawInfo(void* context, SEditDrawInfo* info)
    {
        info->PlayerNumber = PlayerNumber;
        info->Colour.Set(0x80, 0x80, 0x80, 0xff);
        info->Label = "";
        info->Highlighted = false;
        return true;
    }
};
int main()
{
    SEditDrawInfo info; info.Label.m_text = "x"; info.Colour.Set(1, 2, 3, 4); info.PlayerNumber = 9; info.Highlighted = true;
    CThing t; t.PlayerNumber = -1;
    if (!t.DrawGetEditDrawInfo(0, &info) || info.PlayerNumber != -1 || info.Colour.R != 0x80 || info.Colour.G != 0x80 ||
        info.Colour.B != 0x80 || info.Colour.A != 0xff || std::strlen(info.Label.m_text) != 0 || info.Highlighted) {
        std::printf("004c74b0_TEST FAIL\n"); return 1;
    }
    std::printf("004c74b0_TEST PASS\n");
    return 0;
}