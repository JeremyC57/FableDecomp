#include <cstdio>
#include <cstring>
// Behaviour model: true exactly when a sub chunk of the tag exists.
static const char* subs[3] = { "OBJT", "UNIV", "SEQN" };
static bool has(const char* const* s, int n, const char* tag) { for (int i = 0; i < n; ++i) if (!std::strcmp(s[i], tag)) return true; return false; }
int main()
{
    if (!has(subs, 3, "UNIV") || has(subs, 1, "UNIV")) { std::printf("00abb610_TEST FAIL\n"); return 1; }
    std::printf("00abb610_TEST PASS\n");
    return 0;
}