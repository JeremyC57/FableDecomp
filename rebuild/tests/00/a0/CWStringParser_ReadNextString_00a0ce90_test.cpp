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
static bool ReadNextString(CWStringParser* s, CWParsedItem* item)
{
    unsigned long pos = s->StringPos;
    wch c = s->String[pos];
    while (t_isdigit(c) || t_isalpha(c) || c == 95) { if (c == 10) ++s->Line; c = s->String[++pos]; }
    item->InitAsString(&s->String[s->StringPos], pos - s->StringPos);
    s->StringPos = pos;
    return true;
}
int main()
{
    CWStringParser p; CWParsedItem it; it.Type = 0; it.String.length = -1;
    p.String = W("x_9b+q"); p.StringPos = 0; p.Line = 0;
    if (!ReadNextString(&p, &it) || it.String.length != 4 || p.StringPos != 4 || it.Type != 2) { std::printf("00a0ce90_TEST FAIL\n"); return 1; }
    if (!ReadNextString(&p, &it) || it.String.length != 0 || p.StringPos != 4) { std::printf("00a0ce90_TEST FAIL\n"); return 1; }
    std::printf("00a0ce90_TEST PASS\n");
    return 0;
}