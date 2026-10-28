// behaviour test for 005cc772: CTCInventoryStats::GetXString -- the looked-up TEXT_GUI_MENU_XSTRING text, round-tripped
// through a narrow string, with underscores replaced by spaces; every string temporary is destroyed
#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    char buf[64];
    CCharString(const char* text) { std::strcpy(buf, text); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(buf, o.buf); ++g_live; }
    ~CCharString() { --g_live; }
};
class CWideString { public:
    unsigned short buf[64];
    CWideString(const unsigned short* text) { int i = 0; for (; text[i]; ++i) buf[i] = text[i]; buf[i] = 0; ++g_live; }
    CWideString(const CWideString& o) { std::memcpy(buf, o.buf, sizeof(buf)); ++g_live; }
    ~CWideString() { --g_live; }
    const CWideString& operator=(const CWideString& o) { std::memcpy(buf, o.buf, sizeof(buf)); return *this; }
    CCharString ToCharString() const { char n[64]; int i = 0; for (; buf[i]; ++i) n[i] = (char)buf[i]; n[i] = 0; return CCharString(n); }
    void Replace(const CWideString& from, const CWideString& to)   /* single-character replace, as used here */
    {
        for (int i = 0; buf[i]; ++i) if (buf[i] == from.buf[0]) buf[i] = to.buf[0];
    }
};
static CWideString ToWideString(const CCharString& s)
{
    unsigned short w[64]; int i = 0; for (; s.buf[i]; ++i) w[i] = (unsigned char)s.buf[i]; w[i] = 0; return CWideString(w);
}
static const char* g_lastSymbol = 0;
static CWideString GetTextBySymbol(const CCharString& symbol)
{
    static char seen[64]; std::strcpy(seen, symbol.buf); g_lastSymbol = seen;
    static const unsigned short text[] = { 'x', '_', 'o', 'f', '_', '_', '3', 0 };
    return CWideString(text);
}
static const unsigned short kUnderscore[] = { '_', 0 }, kSpace[] = { ' ', 0 };
static CWideString GetXString()
{
    CWideString text = GetTextBySymbol("TEXT_GUI_MENU_XSTRING");
    CCharString narrow = text.ToCharString();
    text = ToWideString(narrow);
    text.Replace(kUnderscore, kSpace);
    text.Replace(kUnderscore, kSpace);
    return text;
}
int main()
{
    int bad = 0;
    {
        CWideString r = GetXString();
        static const unsigned short want[] = { 'x', ' ', 'o', 'f', ' ', ' ', '3', 0 };
        for (int i = 0; i < 8; ++i) if (r.buf[i] != want[i]) ++bad;
        if (!g_lastSymbol || std::strcmp(g_lastSymbol, "TEXT_GUI_MENU_XSTRING") != 0) ++bad;
        if (g_live != 1) ++bad;
    }
    if (g_live != 0) ++bad;
    if (bad) { std::printf("005cc772_TEST FAIL\n"); return 1; }
    std::printf("005cc772_TEST PASS\n");
    return 0;
}