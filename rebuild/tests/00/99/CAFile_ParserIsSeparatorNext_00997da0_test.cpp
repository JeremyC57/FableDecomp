// Behaviour test for 0x00997DA0 CAFile::ParserIsSeparatorNext: true only when a next item exists, is a symbol, and is
// exactly "\" or "/"; strings that merely look like separators, longer symbols and a null symbol give false.
#include <cstdio>
#include <cstring>
struct Item { int type; const char* text; };
static bool Equals(const char* s, char c)
{
    if (s != 0) { char t[2]; t[0] = c; t[1] = 0; return std::strcmp(s, t) == 0; }
    return false;
}
static bool IsSeparatorNext(bool exists, Item item)
{
    bool separator = false;
    if (exists && item.type == 1)
        if (Equals(item.text, '\\') || Equals(item.text, '/'))
            separator = true;
    return separator;
}
int main()
{
    int fails = 0;
    Item back = { 1, "\\" }, fwd = { 1, "/" }, str = { 4, "/" }, two = { 1, "//" }, nul = { 1, 0 }, colon = { 1, ":" };
    if (!IsSeparatorNext(true, back)) ++fails;
    if (!IsSeparatorNext(true, fwd)) ++fails;
    if (IsSeparatorNext(false, back)) ++fails;
    if (IsSeparatorNext(true, str)) ++fails;
    if (IsSeparatorNext(true, two)) ++fails;
    if (IsSeparatorNext(true, nul)) ++fails;
    if (IsSeparatorNext(true, colon)) ++fails;
    std::printf(fails ? "00997da0_TEST FAIL\n" : "00997da0_TEST PASS\n");
    return fails ? 1 : 0;
}