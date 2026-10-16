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
static bool ReadNWS(CWStringParser* s, CWParsedItem* item)
{
    wch c = s->String[s->StringPos];
    if (c == 0) return false;
    unsigned long length = 0;
    for (;;) {
        if (c == 10) ++s->Line;
        if (t_isspace(c) || c == 0) break;
        c = s->String[s->StringPos + ++length];
    }
    if (length > 0) { item->InitAsString(&s->String[s->StringPos], length); s->StringPos += length; return true; }
    return false;
}
int main()
{
    CWStringParser p; CWParsedItem it; it.Type = 0; it.String.length = -1;
    p.String = W("abc de"); p.StringPos = 0; p.Line = 0;
    if (!ReadNWS(&p, &it) || it.String.length != 3 || p.StringPos != 3) { std::printf("00a0cd40_TEST FAIL\n"); return 1; }
    if (ReadNWS(&p, &it)) { std::printf("00a0cd40_TEST FAIL\n"); return 1; }
    p.StringPos = 6; if (ReadNWS(&p, &it)) { std::printf("00a0cd40_TEST FAIL\n"); return 1; }
    p.StringPos = 4; if (!ReadNWS(&p, &it) || it.String.length != 2 || p.StringPos != 6) { std::printf("00a0cd40_TEST FAIL\n"); return 1; }
    std::printf("00a0cd40_TEST PASS\n");
    return 0;
}