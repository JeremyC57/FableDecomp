#include <cstdio>

// test stand-in for CWideString: a fixed-capacity UTF-16 buffer with the engine operations the rows call
class CWideString {
public:
    enum { CAP = 128 };
    CWideString() { Len = 0; Data[0] = 0; }
    CWideString(const unsigned short* text) { Assign(text); }
    CWideString(const CWideString& other) { Assign(other.Data); }
    ~CWideString() { ++Destroyed; }
    CWideString& operator=(const CWideString& other) { Assign(other.Data); return *this; }
    bool Contains(const CWideString& text) const { return Find(text) >= 0; }
    void Replace(const CWideString& search, const CWideString& replacement)
    {
        int at;
        while ((at = Find(search)) >= 0) {
            unsigned short out[CAP];
            int n = 0, i;
            for (i = 0; i < at; ++i) out[n++] = Data[i];
            for (i = 0; i < replacement.Len; ++i) out[n++] = replacement.Data[i];
            for (i = at + search.Len; i < Len; ++i) out[n++] = Data[i];
            out[n] = 0;
            Assign(out);
        }
    }
    void Format(const unsigned short* fmt, int value)     // "%d"-only stand-in
    {
        (void)fmt;
        char digits[16];
        int n = std::sprintf(digits, "%d", value);
        for (int i = 0; i <= n; ++i) Data[i] = (unsigned short)(unsigned char)digits[i];
        Len = n;
    }
    bool Equals(const char* ascii) const
    {
        int i = 0;
        for (; ascii[i]; ++i) if (i >= Len || Data[i] != (unsigned short)(unsigned char)ascii[i]) return false;
        return i == Len;
    }
    int Find(const CWideString& s) const
    {
        if (s.Len == 0) return -1;
        for (int i = 0; i + s.Len <= Len; ++i) {
            int j = 0;
            while (j < s.Len && Data[i + j] == s.Data[j]) ++j;
            if (j == s.Len) return i;
        }
        return -1;
    }
    void Assign(const unsigned short* text)
    {
        Len = 0;
        while (text[Len] && Len < CAP - 1) { Data[Len] = text[Len]; ++Len; }
        Data[Len] = 0;
    }
    static int Destroyed;
    unsigned short Data[CAP];
    int Len;
};
int CWideString::Destroyed = 0;

static CWideString W(const char* ascii)
{
    unsigned short buf[CWideString::CAP];
    int i = 0;
    for (; ascii[i] && i < CWideString::CAP - 1; ++i) buf[i] = (unsigned short)(unsigned char)ascii[i];
    buf[i] = 0;
    return CWideString(buf);
}

class C2DVector {
public:
    float X, Y;
};

class C2DBoxF {
public:
    C2DVector TopLeft;
    C2DVector BottomRight;
};

class CFontBank { public: int CharsPerLine; };

static int g_wraps = 0;
static CWideString WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text)   // stand-in
{
    ++g_wraps;
    (void)box;
    CWideString r(text);
    for (int i = font.CharsPerLine; i < r.Len; i += font.CharsPerLine + 1)
        if (r.Data[i] == ' ') r.Data[i] = '\n';
    return r;
}

bool FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText, C2DBoxF& outBox)
{
    outText = WordWrapText(box, font, text);
    outBox = box;
    return false;
}

int main()
{
    C2DBoxF box;
    box.TopLeft.X = 10.0f; box.TopLeft.Y = 20.0f; box.BottomRight.X = 300.5f; box.BottomRight.Y = 140.25f;
    CFontBank font; font.CharsPerLine = 3;
    CWideString text = W("abc def ghi");
    CWideString out = W("old");
    C2DBoxF outBox;
    outBox.TopLeft.X = outBox.TopLeft.Y = outBox.BottomRight.X = outBox.BottomRight.Y = -1.0f;
    bool r = FormatTextForVWindow(box, font, text, out, outBox);
    if (r || g_wraps != 1 || !out.Equals("abc\ndef\nghi") || !text.Equals("abc def ghi")) { std::printf("009bc9d0_TEST FAIL\n"); return 1; }
    if (outBox.TopLeft.X != 10.0f || outBox.TopLeft.Y != 20.0f || outBox.BottomRight.X != 300.5f || outBox.BottomRight.Y != 140.25f) {
        std::printf("009bc9d0_TEST FAIL\n"); return 1;
    }
    std::printf("009bc9d0_TEST PASS\n");
    return 0;
}