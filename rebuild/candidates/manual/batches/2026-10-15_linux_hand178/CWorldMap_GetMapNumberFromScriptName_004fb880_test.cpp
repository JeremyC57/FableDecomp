#include <cstdio>
#include <cstring>
#include <cctype>
static int ieq(const char* a, const char* b) { while (*a && std::tolower(*a) == std::tolower(*b)) { ++a; ++b; } return std::tolower(*a) - std::tolower(*b); }
static long Find(const char* const* names, unsigned long n, const char* name) { for (unsigned long i = 1; i < n; ++i) if (ieq(names[i] ? names[i] : "", name) == 0) return (long)i; return 0; }
static bool Run() { const char* names[4] = { "Zero", "Oakvale", 0, "Bowerstone" }; return Find(names, 4, "BOWERSTONE") == 3 && Find(names, 4, "zero") == 0 && Find(names, 4, "") == 2 && Find(names, 1, "x") == 0; }
int main()
{
    if (!Run()) { std::printf("004fb880_TEST FAIL\n"); return 1; }
    std::printf("004fb880_TEST PASS\n");
    return 0;
}
