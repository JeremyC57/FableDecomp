#include <cstdio>
#include <cstring>
// Behaviour model: true exactly when a sub chunk of the tag exists.
static const char* subs[3] = { "OBJT", "SMTH", "SEQN" };
static bool has(const char* const* s, int n, const char* tag) { for (int i = 0; i < n; ++i) if (!std::strcmp(s[i], tag)) return true; return false; }
int main()
{
    if (!has(subs, 3, "SMTH") || has(subs, 1, "SMTH")) { std::printf("00abb470_TEST FAIL\n"); return 1; }
    std::printf("00abb470_TEST PASS\n");
    return 0;
}