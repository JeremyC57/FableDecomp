// Behaviour test for 0x00A60390 GFGetSubString(const char* text, const char* sub)
#include <cstdio>

char* GFGetSubString(const char* text, const char* sub)
{
    for (long i = 0; text[i] != 0; ++i) {
        long j = 0;
        while (sub[j] != 0 && text[i + j] != 0 && text[i + j] == sub[j])
            ++j;
        if (sub[j] == 0)
            return (char*)&text[i];
    }
    return 0;
}

int main()
{
    const char* hello = "hello world";
    const char* aab = "aab";
    bool ok = GFGetSubString(hello, "wor") == hello + 6 && GFGetSubString(hello, "hello") == hello &&
              GFGetSubString(hello, "worlds") == 0 && GFGetSubString(hello, "xyz") == 0 &&
              GFGetSubString(aab, "ab") == aab + 1 && GFGetSubString(aab, "") == aab && GFGetSubString("", "a") == 0;
    if (!ok) { std::printf("00a60390_TEST FAIL\n"); return 1; }
    std::printf("00a60390_TEST PASS\n");
    return 0;
}