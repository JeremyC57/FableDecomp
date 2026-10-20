// Behaviour test for CStringParser::Error (0x009B95F0): "<file>(<line+1>) : error : <message>" reaches the callback.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_live = 0;
struct CCharString {
    char* PStringData;
    CCharString(const char* s) { PStringData = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(PStringData, s); ++g_live; }
    CCharString(const CCharString& o) { PStringData = (char*)std::malloc(std::strlen(o.PStringData) + 1); std::strcpy(PStringData, o.PStringData); ++g_live; }
    ~CCharString() { std::free(PStringData); --g_live; }
};
static CCharString Cat(const char* a, const char* b)
{ char buf[512]; std::strcpy(buf, a); std::strcat(buf, b); return CCharString(buf); }
CCharString __fastcall GFIntToCharString(long value) { char b[32]; std::sprintf(b, "%ld", value); return CCharString(b); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return Cat(left.PStringData, right); }
CCharString __fastcall operator+(const CCharString& left, const CCharString& right) { return Cat(left.PStringData, right.PStringData); }
struct CCursorPos { bool InQuotes; unsigned long StringPos; unsigned long Line; };
struct CStringParser {
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString; void* Comments;
    void (__fastcall* ErrorCallback)(const CCharString&); CCursorPos CursorPos;
    CStringParser() : Filename("quests.txt"), TempCharString("") {}
    void Error(const CCharString& message);
};
void CStringParser::Error(const CCharString& message)
{
    CCharString text = Filename + "(" + GFIntToCharString(CursorPos.Line + 1) + ")" + " : error : " + message;
    if (ErrorCallback)
        ErrorCallback(text);
}
static char g_got[256]; static int g_calls = 0; static int g_liveInCallback = 0;
static void __fastcall Sink(const CCharString& s) { std::strcpy(g_got, s.PStringData); ++g_calls; g_liveInCallback = g_live; }
int main()
{
    CStringParser p; p.ErrorCallback = 0; p.CursorPos.Line = 41;
    CCharString msg("missing quote");
    int base = g_live;
    p.Error(msg);
    if (g_calls != 0 || g_live != base) { std::printf("009b95f0_TEST FAIL\n"); return 1; }
    p.ErrorCallback = Sink; p.Error(msg);
    if (g_calls != 1 || std::strcmp(g_got, "quests.txt(42) : error : missing quote") != 0 || g_live != base || g_liveInCallback != base + 1)
    { std::printf("009b95f0_TEST FAIL\n"); return 1; }
    std::printf("009b95f0_TEST PASS\n");
    return 0;
}
