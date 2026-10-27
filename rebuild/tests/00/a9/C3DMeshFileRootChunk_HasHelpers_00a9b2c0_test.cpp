#include <cstdio>
#include <cstring>
// Behaviour model: true exactly when a sub chunk of the tag exists.
static const char* subs[3] = { "OBJT", "HLPR", "SEQN" };
static bool has(const char* const* s, int n, const char* tag) { for (int i = 0; i < n; ++i) if (!std::strcmp(s[i], tag)) return true; return false; }
int main()
{
    if (!has(subs, 3, "HLPR") || has(subs, 1, "HLPR")) { std::printf("00a9b2c0_TEST FAIL\n"); return 1; }
    std::printf("00a9b2c0_TEST PASS\n");
    return 0;
}