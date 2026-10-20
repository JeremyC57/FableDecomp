// Behaviour test for CSubtitleRenderer::WordWrapText (0x0048DC8B): lines of the box width each end in "\n", a line
// that starts with ',' is pulled back onto the previous one.
#include <cstdio>
#include <cstring>

typedef unsigned short WCH;
struct C2DBoxF { float TLX, TLY, BRX, BRY; };

class CWideString {
public:
    WCH buf[128];
    long len;
    CWideString() : len(0) { buf[0] = 0; }
    CWideString(const CWideString& o) : len(o.len) { std::memcpy(buf, o.buf, sizeof(buf)); }
    operator const WCH*() const { return buf; }
    const CWideString& operator=(const WCH* t) { len = 0; while (t[len]) { buf[len] = t[len]; ++len; } buf[len] = 0; return *this; }
    CWideString& operator=(const CWideString& o) { std::memcpy(buf, o.buf, sizeof(buf)); len = o.len; return *this; }
    const CWideString& operator+=(const WCH* t) { while (*t) buf[len++] = *t++; buf[len] = 0; return *this; }
    const CWideString& operator+=(const CWideString& o) { return *this += o.buf; }
    void SetAsSubString(const WCH* t, long n) { for (len = 0; len < n; ++len) buf[len] = t[len]; buf[len] = 0; }
    long GetLength() const { return len; }
    void SetLength(long n) { len = n; buf[len] = 0; }
    CWideString GetSubString(unsigned long start, unsigned long end) const
    {
        CWideString r; r.SetAsSubString(buf + start, (long)(end - start)); return r;
    }
};
static CWideString operator+(const CWideString& a, const WCH* b) { CWideString r(a); r += b; return r; }

static unsigned long g_width = 0;
// stand-in finder: next line starts after `width` characters, null at the end of the text
static const WCH* WordWrap_FindNextLineW(const WCH* text, unsigned long width, const WCH** lineEnd)
{
    g_width = width;
    unsigned long n = 0;
    while (text[n] && n < width) ++n;
    *lineEnd = text + n;
    return text[n] ? text + n : 0;
}

static CWideString WordWrapText(const C2DBoxF& box, void* font, const CWideString& text)
{
    CWideString wrapped;
    const WCH* line = text;
    const WCH* next;
    do {
        float left = box.TLX;
        const WCH* lineEnd = 0;
        float right = box.BRX;
        next = WordWrap_FindNextLineW(line, (long)(right - left), &lineEnd);
        CWideString lineText;
        if (next)
            lineText.SetAsSubString(line, next - line);
        else
            lineText = line;
        if (*(const WCH*)lineText == ',') {
            wrapped.SetLength(wrapped.GetLength() - 1);
            static const WCH commaNl[] = { ',', '\n', 0 };
            wrapped += commaNl;
            lineText = lineText.GetSubString(1, lineText.GetLength());
        }
        static const WCH nl[] = { '\n', 0 };
        wrapped += lineText + nl;
        line = next;
    } while (next);
    (void)font;
    return wrapped;
}

static int Same(const CWideString& s, const char* expect)
{
    long n = (long)std::strlen(expect);
    if (s.len != n) return 0;
    for (long i = 0; i < n; ++i) if (s.buf[i] != (WCH)(unsigned char)expect[i]) return 0;
    return 1;
}

int main()
{
    C2DBoxF box = { 10.0f, 0.0f, 14.5f, 0.0f };   // width 4
    CWideString text; const WCH t1[] = { 'a','b','c','d',',','e','f','g','h', 0 };
    text = t1;
    CWideString out = WordWrapText(box, 0, text);
    int ok = 1;
    // "abcd" / ",efg" / "h": the comma line is pulled back: "abcd,\nefg\nh\n"
    if (!Same(out, "abcd,\nefg\nh\n") || g_width != 4) ok = 0;
    CWideString shortText; const WCH t2[] = { 'x','y', 0 };
    shortText = t2;
    if (!Same(WordWrapText(box, 0, shortText), "xy\n")) ok = 0;
    if (!ok) { std::printf("0048dc8b_TEST FAIL\n"); return 1; }
    std::printf("0048dc8b_TEST PASS\n");
    return 0;
}