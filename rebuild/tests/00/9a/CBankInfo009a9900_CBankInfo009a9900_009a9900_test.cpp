// Behaviour test for 0x009A9900 CBankInfo_009a9900::CBankInfo_009a9900(long unused): four empty CWideString paths and a null registered bank
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
struct CBankInfo_009a9900 {
    CWideString DevBankFilePath;
    CWideString RetailBankFilePath;
    CWideString DevHeaderFilePath;
    CWideString RetailHeaderFilePath;
    CBankFile* RegisteredBank;
    CBankInfo_009a9900(long unused);
};
CBankInfo_009a9900::CBankInfo_009a9900(long unused)
{
    RegisteredBank = 0;
}
int main()
{
    unsigned char raw[sizeof(CBankInfo_009a9900)];
    unsigned int i;
    for (i = 0; i < sizeof(raw); ++i) raw[i] = 0xcd;
    CBankInfo_009a9900* b = new (raw) CBankInfo_009a9900(5);
    if (b->RegisteredBank != 0 || b->DevBankFilePath.storage_ != 0 || b->RetailHeaderFilePath.storage_ != 0 || g_wideStrings != 4)
        { std::printf("009a9900_TEST FAIL\n"); return 1; }
    b->~CBankInfo_009a9900();
    if (g_wideStrings != 0) { std::printf("009a9900_TEST FAIL\n"); return 1; }
    std::printf("009a9900_TEST PASS\n");
    return 0;
}