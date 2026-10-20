// Behaviour test for CStringParser::SkipUntilWholeString (0x009B9AA0): whole-word search from the cursor.
#include <cstdio>
#include <cstring>
#include <cctype>

struct CCharStringData { const char* Chars; unsigned long Length; };
struct CCharString {
    CCharStringData* PStringData;
    operator const char*() const { return PStringData ? PStringData->Chars : ""; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
struct CCursorPos { bool InQuotes; unsigned long StringPos; unsigned long Line; };
struct CStringParser {
    char* String;
    CCursorPos CursorPos;
    bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const
    {
        for (; from != to; ++from) if (*from == '"' || *from == '\'') inQuotes = !inQuotes;
        return inQuotes;
    }
    void MoveStringPos(unsigned long n)
    {
        if (n > 0) {
            unsigned long pos = CursorPos.StringPos;
            const char* from = String + pos;
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, from, from + n);
            CursorPos.StringPos = pos + n;
        }
    }
    bool SkipUntilWholeString(const CCharString& word)
    {
        long offset = CursorPos.StringPos;
        char* found = String + offset;
        found = strstr(found, word);
        while (found) {
            offset = found - String;
            unsigned char before;
            if (offset == 0 || !(isalpha(before = String[offset - 1]) || isdigit(before) || before == '_')) {
                unsigned char after = String[word.GetLength() + offset];
                if (!(isalpha(after) || isdigit(after) || after == '_')) {
                    MoveStringPos(found - (String + CursorPos.StringPos));
                    return true;
                }
            }
            found = strstr(found + 1, word);
        }
        return false;
    }
};

int main()
{
    char text[] = "xEND END_ 'q' END1 a END;";
    CCharStringData d = { "END", 3 };
    CCharString w; w.PStringData = &d;
    CStringParser p; p.String = text; p.CursorPos.InQuotes = false; p.CursorPos.StringPos = 0; p.CursorPos.Line = 0;
    int ok = 1;
    // first whole-word END is at offset 21 ("a END;"); the quote pair toggles twice
    if (!p.SkipUntilWholeString(w) || p.CursorPos.StringPos != 21 || p.CursorPos.InQuotes) ok = 0;
    // searching again from the hit finds it immediately (n == 0: nothing moves)
    if (!p.SkipUntilWholeString(w) || p.CursorPos.StringPos != 21) ok = 0;
    // a hit at offset 0 counts as whole when followed by a non-word char
    char text2[] = "END 'x";
    CStringParser q; q.String = text2; q.CursorPos.InQuotes = true; q.CursorPos.StringPos = 0;
    if (!q.SkipUntilWholeString(w) || q.CursorPos.StringPos != 0 || !q.CursorPos.InQuotes) ok = 0;
    // no whole word: false, cursor unchanged
    char text3[] = "ENDING _END xEND";
    CStringParser r; r.String = text3; r.CursorPos.InQuotes = false; r.CursorPos.StringPos = 2;
    if (r.SkipUntilWholeString(w) || r.CursorPos.StringPos != 2) ok = 0;
    if (!ok) { std::printf("009b9aa0_TEST FAIL\n"); return 1; }
    std::printf("009b9aa0_TEST PASS\n");
    return 0;
}