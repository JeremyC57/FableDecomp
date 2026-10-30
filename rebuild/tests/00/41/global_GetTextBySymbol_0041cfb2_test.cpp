// Behaviour test for 0x0041CFB2 GetTextBySymbol (see src/0041cfb2.cpp): main bank first, then the front-end bank, each only when
// the text is non-empty; otherwise the symbol itself.
#include <cstdio>
#include <cstring>
struct Bank { const char* key; const char* text; };
static const char* Lookup(const Bank* bank, const char* symbol)
{
    return (bank && !std::strcmp(bank->key, symbol)) ? bank->text : "";
}
static const char* GetTextBySymbol(const Bank* mainBank, const Bank* frontEnd, const char* symbol)
{
    if (mainBank) { const char* t = Lookup(mainBank, symbol); if (*t) return t; }
    if (frontEnd) { const char* t = Lookup(frontEnd, symbol); if (*t) return t; }
    return symbol;
}
int main()
{
    Bank m = { "A", "main-A" }, f = { "A", "fe-A" }, g = { "B", "fe-B" };
    if (std::strcmp(GetTextBySymbol(&m, &f, "A"), "main-A")) { std::printf("0041cfb2_TEST FAIL\n"); return 1; }
    if (std::strcmp(GetTextBySymbol(&m, &g, "B"), "fe-B")) { std::printf("0041cfb2_TEST FAIL\n"); return 1; }
    if (std::strcmp(GetTextBySymbol(0, 0, "C"), "C")) { std::printf("0041cfb2_TEST FAIL\n"); return 1; }
    std::printf("0041cfb2_TEST PASS\n"); return 0;
}