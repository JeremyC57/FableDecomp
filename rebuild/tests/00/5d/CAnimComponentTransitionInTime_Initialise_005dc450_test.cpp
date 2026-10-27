// behaviour test for 0x005DC450: Initialise resets, then stores the parsed long and returns true
#include <cstdio>
#include <cstdlib>
struct C { long member; int resets; void Reset() { ++resets; member = -1; }
    bool Initialise(const char* text) { Reset(); member = std::strtol(text, 0, 10); return true; } };
int main()
{
    C c = { 0, 0 };
    bool ok = c.Initialise("42");
    if (!ok || c.member != 42 || c.resets != 1) { std::printf("005dc450_TEST FAIL\n"); return 1; }
    std::printf("005dc450_TEST PASS\n");
    return 0;
}