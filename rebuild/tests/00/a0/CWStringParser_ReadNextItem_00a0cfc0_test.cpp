// Behaviour test for 0x00A0CFC0 CWStringParser::ReadNextItem: whitespace/line skipping, dispatch to the number and
// string readers, single-character symbols, end of text.
#include <cstdio>
typedef unsigned short wchar16;
static int t_isdigit(wchar16 c) { return c >= '0' && c <= '9'; }
static int t_isalpha(wchar16 c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static int t_isspace(wchar16 c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
struct CWideString { const wchar16* text; long length; void SetAsSubString(const wchar16* t, long n) { text = t; length = n; } };
struct CWParsedItem { long Type; long Value; CWideString String; };
struct CWStringParser {
    wchar16* String; unsigned long StringPos; unsigned long Line; int numbers, strings;
    bool ReadNextNumber(CWParsedItem* item) { ++numbers; item->Type = 3; return true; }
    bool ReadNextString(CWParsedItem* item) { ++strings; item->Type = 2; return true; }
    bool ReadNextItem(CWParsedItem* item)
    {
        unsigned long pos = StringPos;
        wchar16 c = String[pos];
        while (c != 0) {
            if (c == '\n') ++Line;
            if (!t_isspace(c)) {
                StringPos = pos;
                if (t_isdigit(c) || (c == '-' && t_isdigit(String[pos + 1]))) return ReadNextNumber(item);
                if (t_isalpha(c) || c == '_') return ReadNextString(item);
                item->String.SetAsSubString(String + StringPos, 1);
                item->Type = 1;
                ++StringPos;
                return true;
            }
            c = String[++pos];
        }
        return false;
    }
};
int main()
{
    wchar16 text[] = { ' ', '\n', ' ', '-', '5', ' ', '_', 'x', '\n', '-', 'a', ' ', 0 };
    CWStringParser p; p.String = text; p.StringPos = 0; p.Line = 0; p.numbers = p.strings = 0;
    CWParsedItem item; bool ok = true;
    ok = ok && p.ReadNextItem(&item) && item.Type == 3 && p.StringPos == 3 && p.Line == 1 && p.numbers == 1;
    p.StringPos = 5;
    ok = ok && p.ReadNextItem(&item) && item.Type == 2 && p.StringPos == 6 && p.strings == 1;
    p.StringPos = 8;
    ok = ok && p.ReadNextItem(&item) && item.Type == 1 && item.String.text == text + 9 && item.String.length == 1
         && p.StringPos == 10 && p.Line == 2;
    p.StringPos = 11;
    ok = ok && !p.ReadNextItem(&item) && p.StringPos == 11;
    if (!ok) { std::printf("00a0cfc0_TEST FAIL\n"); return 1; }
    std::printf("00a0cfc0_TEST PASS\n");
    return 0;
}