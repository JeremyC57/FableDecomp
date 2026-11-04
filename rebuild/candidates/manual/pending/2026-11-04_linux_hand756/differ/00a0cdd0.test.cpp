// Behaviour test for CWStringParser::ReadNextNumber (0x00A0CDD0): a model of the row -- digits, one '.', and a '-' only at the
// start form the number; a second '.' or any other character ends it; it is stored as a float when it had a point, else as
// an integer, and the cursor moves past it.
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
struct Item { int Type; unsigned long Start, Length; };
static unsigned long Read(const wchar_t* s, unsigned long pos, Item& item)
{
    unsigned long start = pos; bool isFloat = false; wchar_t c = s[pos];
    for (;;) {
        if (!iswdigit(c)) {
            if (c == L'.') { if (isFloat) break; }
            else if (c != L'-' || pos != start) break;
        }
        if (c == L'.') isFloat = true;
        c = s[++pos];
    }
    item.Type = isFloat ? 4 : 3; item.Start = start; item.Length = pos - start;
    return pos;
}
int main()
{
    Item a, b, c;
    unsigned long pa = Read(L"-12.5.3x", 0, a), pb = Read(L"a 42;", 2, b), pc = Read(L"7-3", 0, c);
    int ok = pa == 5 && a.Type == 4 && a.Length == 5 && pb == 4 && b.Type == 3 && b.Start == 2 && pc == 1 && c.Type == 3;
    printf(ok ? "00a0cdd0_TEST PASS\n" : "00a0cdd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
