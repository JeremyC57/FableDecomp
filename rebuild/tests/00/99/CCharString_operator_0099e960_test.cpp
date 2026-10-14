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
    bool operator==(const char* text) const { if (PStringData == 0) return std::strcmp(text, "") == 0; return PStringData->Data == text; }
    bool operator!=(const char* text) const { return !(*this == text); }
};
int main()
{
    CCharString e;
    if (e != "") { std::printf("0099e960_TEST FAIL\n"); return 1; }
    if (!(e != "a")) { std::printf("0099e960_TEST FAIL\n"); return 1; }
    CCharString s; s.PStringData = new CCharStringData("abc");
    if (s != "abc") { std::printf("0099e960_TEST FAIL\n"); return 1; }
    if (!(s != "abd")) { std::printf("0099e960_TEST FAIL\n"); return 1; }
    if (!(s != "")) { std::printf("0099e960_TEST FAIL\n"); return 1; }
    std::printf("0099e960_TEST PASS\n"); return 0;
}