// Behaviour test for 0x009975E0 CAFile::IsForbiddenSymbolInComponent: any of < > : 0x93 / \ | anywhere in the component
// makes it forbidden; ordinary names (dots, spaces, digits, a plain '"' 0x22) pass.
#include <cstdio>
#include <cstring>
static int g_calls = 0;
static long Find(const char* s, char c) { ++g_calls; const char* p = std::strchr(s, c); return (c && p) ? (long)(p - s) : -1; }
static const char ForbiddenSymbols[] = "<>:\x93/\\|";
static bool IsForbiddenSymbolInComponent(const char* component)
{
    bool forbidden = false;
    for (long i = 0; i < 7; ++i)
        if (Find(component, ForbiddenSymbols[i]) > -1) { forbidden = true; break; }
    return forbidden;
}
int main()
{
    int fails = 0;
    if (IsForbiddenSymbolInComponent("Level 01.wld")) ++fails;
    if (g_calls != 7) ++fails;
    g_calls = 0;
    if (!IsForbiddenSymbolInComponent("<start")) ++fails;
    if (g_calls != 1) ++fails;
    if (!IsForbiddenSymbolInComponent("a|b")) ++fails;
    if (!IsForbiddenSymbolInComponent("c:")) ++fails;
    if (!IsForbiddenSymbolInComponent("x\x93y")) ++fails;
    if (IsForbiddenSymbolInComponent("say \"hi\"")) ++fails;
    std::printf(fails ? "009975e0_TEST FAIL\n" : "009975e0_TEST PASS\n");
    return fails ? 1 : 0;
}