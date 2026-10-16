#include <cstdio>
// Behaviour model of CTCHeroCentre::GetQuestCards (0x0077DBC0).
struct Vec { int items[8]; unsigned int n; };
static bool GetQuestCards(const Vec& src, Vec& out)
{
    out.n = 0;
    if (src.n != 0) {
        out.n = src.n;
        for (unsigned int i = 0; i < src.n; ++i) out.items[i] = src.items[i];
        return true;
    }
    return false;
}
int main()
{
    Vec a; a.n = 3; a.items[0] = 4; a.items[1] = 5; a.items[2] = 6;
    Vec b; b.n = 7;
    if (!GetQuestCards(a, b) || b.n != 3 || b.items[2] != 6) { std::printf("0077dbc0_TEST FAIL\n"); return 1; }
    a.n = 0;
    if (GetQuestCards(a, b) || b.n != 0) { std::printf("0077dbc0_TEST FAIL\n"); return 1; }
    std::printf("0077dbc0_TEST PASS\n");
    return 0;
}