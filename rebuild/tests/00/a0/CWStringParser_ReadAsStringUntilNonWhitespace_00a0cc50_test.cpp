#include <cstdio>
typedef unsigned short wch;
static int t_isspace(wch c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
static int t_isdigit(wch c) { return c >= '0' && c <= '9'; }
static int t_isalpha(wch c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
struct CWideString {
    const wch* text; long length;
    void SetAsSubString(const wch* t, long n) { text = t; length = n; }
};
struct CWParsedItem {
    long Type; long Integer; CWideString String;
    void InitAsString(const wch* t, unsigned long n) { String.SetAsSubString(t, n); Type = 2; }
};
struct CWStringParser {
    void* vt; wch* String; unsigned long StringPos; unsigned long Line;
};
static wch g_buf[64];
static wch* W(const char* s) { int i = 0; for (; s[i]; ++i) g_buf[i] = (wch)s[i]; g_buf[i] = 0; return g_buf; }
static bool ReadWS(CWStringParser* s, CWParsedItem* item)
{
    unsigned long length = 0;
    wch c = s->String[s->StringPos];
    for (;;) {
        if (c == 10) ++s->Line;
        if (!t_isspace(c) || c == 0) break;
        c = s->String[s->StringPos + ++length];
    }
    if (length > 0) { item->InitAsString(&s->String[s->StringPos], length); s->StringPos += length; return true; }
    return false;
}
int main()
{
    CWStringParser p; CWParsedItem it; it.Type = 0; it.String.length = -1;
    p.String = W("ab \n\tcd"); p.StringPos = 2; p.Line = 0;
    if (!ReadWS(&p, &it) || it.Type != 2 || it.String.length != 3 || p.StringPos != 5 || p.Line != 1) { std::printf("00a0cc50_TEST FAIL\n"); return 1; }
    if (ReadWS(&p, &it) || p.StringPos != 5) { std::printf("00a0cc50_TEST FAIL\n"); return 1; }
    std::printf("00a0cc50_TEST PASS\n");
    return 0;
}