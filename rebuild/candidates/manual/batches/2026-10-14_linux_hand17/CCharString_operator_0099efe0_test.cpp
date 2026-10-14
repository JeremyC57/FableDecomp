#include <cstdio>
#include <cstring>
#include <cstdlib>
#pragma pack(push, 1)
struct CBasicStringChar {
    char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1;
    CBasicStringChar(const char* text) { StringLength = std::strlen(text); PData = (char*)std::malloc(StringLength + 1); std::strcpy(PData, text); DataLength = StringLength; UseFastExtend = false; }
    void Truncate(unsigned long length) { if (length < StringLength) { PData[length] = 0; StringLength = length; } }
    void Insert(long position, const char* text) { unsigned long n = std::strlen(text); char* d = (char*)std::malloc(StringLength + n + 1); std::memcpy(d, PData, position); std::memcpy(d + position, text, n); std::strcpy(d + position + n, PData + position); std::free(PData); PData = d; StringLength += n; }
    bool operator==(const char* text) const { return std::strcmp(PData, text) == 0; }
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; CCharStringData(const char* text) : Data(text), NoRefs(1) { } };
#pragma pack(pop)
static int g_unassigned = 0;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { }
    void UnassignString() { ++g_unassigned; if (PStringData && --PStringData->NoRefs <= 0) { std::free(PStringData->Data.PData); delete PStringData; } PStringData = 0; }
    CCharStringData* AllocStringData(const char* text, long length) { char buf[64]; std::memcpy(buf, text, length); buf[length] = 0; return new CCharStringData(buf); }
    void MakeOwnCopyOfDataOrEmptyString() { if (PStringData == 0) PStringData = new CCharStringData(""); else MakeOwnCopyOfData(); }
    const char* Text() const { return PStringData ? PStringData->Data.PData : ""; }
    void MakeOwnCopyOfData() { }
    const CCharString& operator=(const char* text)
    {
        if (PStringData != 0) UnassignString();
        if (text != 0 && *text != '\0') PStringData = new CCharStringData(text);
        return *this;
    }
};
int main()
{
    CCharString s;
    s = "abc";
    if (s.PStringData == 0 || s.PStringData->NoRefs != 1 || std::strcmp(s.Text(), "abc") != 0) { std::printf("0099efe0_TEST FAIL\n"); return 1; }
    s = "defg";
    if (g_unassigned != 1 || std::strcmp(s.Text(), "defg") != 0) { std::printf("0099efe0_TEST FAIL\n"); return 1; }
    s = "";
    if (g_unassigned != 2 || s.PStringData != 0) { std::printf("0099efe0_TEST FAIL\n"); return 1; }
    s = (const char*)0;
    if (s.PStringData != 0 || g_unassigned != 2) { std::printf("0099efe0_TEST FAIL\n"); return 1; }
    std::printf("0099efe0_TEST PASS\n"); return 0;
}
