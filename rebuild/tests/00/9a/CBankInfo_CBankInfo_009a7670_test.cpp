// Behaviour test for 0x009A7670 CBankInfo::CBankInfo(): four empty CWideString paths and a null registered bank
#include <cstdio>
#include <new>
static long g_charStrings = 0, g_wideStrings = 0;
class CCharString {
public:
    void* storage_;
    CCharString() : storage_(0) { ++g_charStrings; }
    ~CCharString() { --g_charStrings; }
};
class CWideString {
public:
    void* storage_;
    CWideString() : storage_(0) { ++g_wideStrings; }
    ~CWideString() { --g_wideStrings; }
};

class CBankFile;
struct CBankInfo {
    CWideString DevBankFilePath;
    CWideString RetailBankFilePath;
    CWideString DevHeaderFilePath;
    CWideString RetailHeaderFilePath;
    CBankFile* RegisteredBank;
    CBankInfo();
};
CBankInfo::CBankInfo()
{
    RegisteredBank = 0;
}
int main()
{
    unsigned char raw[sizeof(CBankInfo)];
    unsigned int i;
    for (i = 0; i < sizeof(raw); ++i) raw[i] = 0xcd;
    CBankInfo* b = new (raw) CBankInfo;
    if (b->RegisteredBank != 0 || b->DevBankFilePath.storage_ != 0 || b->RetailHeaderFilePath.storage_ != 0 || g_wideStrings != 4)
        { std::printf("009a7670_TEST FAIL\n"); return 1; }
    b->~CBankInfo();
    if (g_wideStrings != 0) { std::printf("009a7670_TEST FAIL\n"); return 1; }
    std::printf("009a7670_TEST PASS\n");
    return 0;
}