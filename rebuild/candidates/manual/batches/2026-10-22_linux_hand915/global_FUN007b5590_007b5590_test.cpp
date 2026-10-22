#include <cstdio>
#include <cstring>
// Behaviour test for 007b5590: the in-place constructed class name is "CActivateQuestDef" (length -1 = strlen).
struct CCharString {
    char text[32];
    CCharString(const char* s, long length) { if (length < 0) length = (long)std::strlen(s); std::memcpy(text, s, length); text[length] = 0; }
};
struct CDefClassName : CCharString { CDefClassName() : CCharString("CActivateQuestDef", -1) { } };
int main()
{
    CDefClassName n;
    if (std::strcmp(n.text, "CActivateQuestDef") != 0) { std::printf("007b5590_TEST FAIL\n"); return 1; }
    std::printf("007b5590_TEST PASS\n");
    return 0;
}
