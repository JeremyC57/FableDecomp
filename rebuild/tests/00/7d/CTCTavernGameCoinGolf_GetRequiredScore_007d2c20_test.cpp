// Behaviour test for GetRequiredScore (007d2c20): the def's required-score text followed by the target round count as text;
// the temporary def reference is released.
#include <cstdio>
#include <cstring>
static int g_count = 1;
static void IntToText(long v, char* out) { std::sprintf(out, "%ld", v); }
static void GetRequiredScore(const char* text, long targetRound, char* out)
{
    ++g_count;                       // def taken
    char num[16]; IntToText(targetRound, num);
    std::strcpy(out, text); std::strcat(out, num);
    --g_count;                       // def released
}
int main()
{
    char buf[64]; GetRequiredScore("Required: ", 12, buf);
    if (std::strcmp(buf, "Required: 12") != 0 || g_count != 1) { std::printf("007d2c20_TEST FAIL\n"); return 1; }
    std::printf("007d2c20_TEST PASS\n");
    return 0;
}