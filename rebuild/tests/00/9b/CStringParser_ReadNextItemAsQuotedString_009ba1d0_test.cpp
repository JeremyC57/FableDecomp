// Behaviour test for CStringParser::ReadNextItemAsQuotedString (0x009BA1D0).
#include <cstdio>
#include <cstring>

struct CCharString {
    char buf[64];
    unsigned long len;
    CCharString() : len(0) { buf[0] = 0; }
    CCharString& operator+=(char c) { buf[len++] = c; buf[len] = 0; return *this; }
    CCharString& operator=(const CCharString& o) { std::memcpy(buf, o.buf, sizeof(buf)); len = o.len; return *this; }
};
struct CCursorPos { bool InQuotes; unsigned long StringPos; unsigned long Line; };
struct CStringParser {
    char* String;
    CCursorPos CursorPos;
    long SkipWhitespace() { while (String[CursorPos.StringPos] == ' ') ++CursorPos.StringPos; return 0; }
    bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const
    {
        while (from != to) { char c = *from; if (c == '"' || c == '\'') inQuotes = !inQuotes; ++from; }
        return inQuotes;
    }
    void MoveStringPos(unsigned long n)
    {
        if (n > 0) {
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, String + CursorPos.StringPos, String + CursorPos.StringPos + n);
            CursorPos.StringPos += n;
        }
    }
    bool ReadNextItemAsQuotedString(CCharString& text)
    {
        SkipWhitespace();
        long end = CursorPos.StringPos;
        bool escaped = false;
        for (++end; String[end]; ++end) {
            if (escaped)
                escaped = false;
            else if (String[end] == '\\')
                escaped = true;
            else if (String[end] == '"' || String[end] == '\'') {
                CCharString quoted;
                for (long i = CursorPos.StringPos + 1; i < end; ++i) {
                    if (String[i] == '\\') {
                        if (i + 1 < end && String[i + 1] == '\\') { quoted += '\\'; ++i; }
                    } else
                        quoted += String[i];
                }
                MoveStringPos(end - CursorPos.StringPos + 1);
                text = quoted;
                return true;
            }
        }
        return false;
    }
};

int main()
{
    int ok = 1;
    char t1[] = "  \"ab\\\"c\\\\d\" rest";       // "ab\"c\\d"
    CStringParser p; p.String = t1; p.CursorPos.InQuotes = false; p.CursorPos.StringPos = 0; p.CursorPos.Line = 0;
    CCharString out;
    if (!p.ReadNextItemAsQuotedString(out)) ok = 0;
    if (std::strcmp(out.buf, "ab\"c\\d") != 0) ok = 0;
    if (p.CursorPos.StringPos != 12 || !p.CursorPos.InQuotes) ok = 0;   // past the closing quote; three quote chars toggle InQuotes
    char t2[] = "'unterminated";
    CStringParser q; q.String = t2; q.CursorPos.InQuotes = false; q.CursorPos.StringPos = 0;
    CCharString out2; out2 += 'z';
    if (q.ReadNextItemAsQuotedString(out2) || q.CursorPos.StringPos != 0 || std::strcmp(out2.buf, "z") != 0) ok = 0;
    char t3[] = "'x'";
    CStringParser r; r.String = t3; r.CursorPos.InQuotes = false; r.CursorPos.StringPos = 0;
    CCharString out3;
    if (!r.ReadNextItemAsQuotedString(out3) || std::strcmp(out3.buf, "x") != 0 || r.CursorPos.StringPos != 3 || r.CursorPos.InQuotes) ok = 0;
    if (!ok) { std::printf("009ba1d0_TEST FAIL\n"); return 1; }
    std::printf("009ba1d0_TEST PASS\n");
    return 0;
}