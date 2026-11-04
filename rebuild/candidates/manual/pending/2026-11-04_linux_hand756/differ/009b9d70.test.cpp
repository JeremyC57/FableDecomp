// Behaviour test for CStringParser::SkipPastNewline (0x009B9D70): a model of the row -- the cursor moves past the rest of the line and the
// whole CR/LF run after it, the in-quotes flag toggles for every quote character skipped, and the count skipped is returned.
#include <stdio.h>
struct Parser { const char* String; bool InQuotes; unsigned long StringPos;
    long SkipPastNewline() {
        const char* start = String + StringPos; const char* p = start;
        while (*p) { if (*p == '\n' || *p == '\r') { while (*p == '\n' || *p == '\r') ++p; break; } ++p; }
        long n = p - start;
        for (const char* q = start; q != p; ++q) if (*q == '"' || *q == '\'') InQuotes = !InQuotes;
        StringPos += n; return n; } };
int main()
{
    Parser ps = { "ab \"c\r\n\r\nnext", false, 0 };
    long n = ps.SkipPastNewline();
    int ok = n == 9 && ps.StringPos == 9 && ps.InQuotes && ps.String[ps.StringPos] == 'n';
    n = ps.SkipPastNewline(); ok = ok && n == 4 && ps.String[ps.StringPos] == 0;
    n = ps.SkipPastNewline(); ok = ok && n == 0;
    printf(ok ? "009b9d70_TEST PASS\n" : "009b9d70_TEST FAIL\n");
    return ok ? 0 : 1;
}
