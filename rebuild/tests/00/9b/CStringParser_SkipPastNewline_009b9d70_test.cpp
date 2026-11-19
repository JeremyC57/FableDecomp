// Behaviour test for 0x009B9D70 CStringParser::SkipPastNewline: the row's defines, includes and body verbatim, on a zeroed parser image
// (String / CursorPos set through a derived accessor). The cursor moves past the rest of the line and the whole CR/LF run, the in-quotes
// flag toggles once per quote character (" or ') in the skipped text, the count is returned; at the end of the string nothing moves and
// 0 is returned; text without a newline is skipped to the terminator.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS
#define FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS_EGOD
#include "engine/CStringParser.h"

long CStringParser::SkipPastNewline()
{
    const char* pstring_start = GetStringPos() + String;
    const char* pstring_end = GetStringPos() + String;
    if (false) {} else for (; *pstring_end; ++pstring_end) {
        if (*pstring_end == '\n' || *pstring_end == '\r') {
            while (*pstring_end == '\n' || *pstring_end == '\r')
                ++pstring_end;
            break;
        }
    }
    long no_chars_skipped = pstring_end - pstring_start;
    MoveStringPos(no_chars_skipped);
    return no_chars_skipped;
}

struct CTestParser : CStringParser {
    static void Set(CStringParser* p, const char* s, unsigned long pos, bool q) { CTestParser* t = static_cast<CTestParser*>(p); t->String = s; t->CursorPos.StringPos = pos; t->CursorPos.InQuotes = q; }
    static unsigned long Pos(CStringParser* p) { return static_cast<CTestParser*>(p)->CursorPos.StringPos; }
    static bool Quotes(CStringParser* p) { return static_cast<CTestParser*>(p)->CursorPos.InQuotes; }
};
static double g_buf[sizeof(CStringParser) / 8 + 1];
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    memset(g_buf, 0, sizeof(g_buf));
    CStringParser* p = reinterpret_cast<CStringParser*>(g_buf);
    const char* text = "ab\"c\r\n\n\rnext 'x'\nlast";
    CTestParser::Set(p, text, 0, false);
    check(p->SkipPastNewline() == 8, "line plus the CR/LF run");
    check(CTestParser::Pos(p) == 8 && text[8] == 'n', "cursor after the run");
    check(CTestParser::Quotes(p) == true, "one quote toggles the flag");
    check(p->SkipPastNewline() == 9, "second line");
    check(CTestParser::Pos(p) == 17 && CTestParser::Quotes(p) == true, "two quotes toggle twice");
    check(p->SkipPastNewline() == 4 && CTestParser::Pos(p) == 21, "no newline: to the terminator");
    check(p->SkipPastNewline() == 0 && CTestParser::Pos(p) == 21 && CTestParser::Quotes(p) == true, "at the end: nothing");
    CTestParser::Set(p, "x\r\ry", 1, true);
    check(p->SkipPastNewline() == 2 && CTestParser::Pos(p) == 3 && CTestParser::Quotes(p) == true, "starting on the newline");
    CTestParser::Set(p, "''\n", 0, true);
    check(p->SkipPastNewline() == 3 && CTestParser::Quotes(p) == true, "apostrophes toggle too (twice)");
    CTestParser::Set(p, "'\n", 0, false);
    check(p->SkipPastNewline() == 2 && CTestParser::Quotes(p) == true, "apostrophe toggles");
    if (!g_fail) printf("009b9d70_TEST PASS\n");
    return g_fail;
}