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
static bool ReadUntil(CWStringParser* s, wch stop, CWParsedItem* item)
{
    const wch* start = &s->String[s->StringPos]; wch c = *start; unsigned long length = 0; const wch* q = start;
    for (;;) { if (c == 10) ++s->Line; if (c == stop || c == 0) break; c = *++q; ++length; }
    if (length > 0) { item->InitAsString(start, length); s->StringPos += length; return true; }
    if (c == stop) { static const wch e[1] = {0}; item->InitAsString(e, 0); return true; }
    return false;
}
static CWideString ReadUntilValue(CWStringParser* s, wch stop)
{
    CWParsedItem item; item.String.text = 0; item.String.length = -1;
    if (ReadUntil(s, stop, &item)) return item.String;
    CWideString empty; empty.text = 0; empty.length = 0; return empty;
}
int main()
{
    CWStringParser p; CWParsedItem it; it.Type = 0; it.String.length = -1;
    p.String = W("key=val"); p.StringPos = 0; p.Line = 0;
    CWideString r = ReadUntilValue(&p, 61);
    if (r.length != 3 || p.StringPos != 3) { std::printf("00a0ccd0_TEST FAIL\n"); return 1; }
    r = ReadUntilValue(&p, 61); if (r.length != 0 || r.text == 0 || p.StringPos != 3) { std::printf("00a0ccd0_TEST FAIL\n"); return 1; }
    p.StringPos = 4; r = ReadUntilValue(&p, 61); if (r.length != 3 || p.StringPos != 7) { std::printf("00a0ccd0_TEST FAIL\n"); return 1; }
    r = ReadUntilValue(&p, 61); if (r.length != 0 || r.text != 0) { std::printf("00a0ccd0_TEST FAIL\n"); return 1; }
    std::printf("00a0ccd0_TEST PASS\n");
    return 0;
}
