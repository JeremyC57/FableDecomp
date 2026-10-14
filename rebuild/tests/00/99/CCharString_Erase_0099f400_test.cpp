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
    void Erase(long position, long count)
    {
        if (PStringData != 0) {
            MakeOwnCopyOfData();
            CCharStringData* data = PStringData;
            long end = position + count;
            std::memmove(data->Data.PData + position, data->Data.PData + end, data->Data.StringLength - end);
            data->Data.Truncate(data->Data.StringLength - count);
        }
    }
};
int main()
{
    CCharString s; s.PStringData = new CCharStringData("hello world");
    s.Erase(5, 6);
    if (std::strcmp(s.Text(), "hello") != 0 || s.PStringData->Data.StringLength != 5) { std::printf("0099f400_TEST FAIL\n"); return 1; }
    s.Erase(0, 1);
    if (std::strcmp(s.Text(), "ello") != 0) { std::printf("0099f400_TEST FAIL\n"); return 1; }
    s.Erase(1, 2);
    if (std::strcmp(s.Text(), "eo") != 0) { std::printf("0099f400_TEST FAIL\n"); return 1; }
    CCharString e; e.Erase(0, 1);
    if (e.PStringData != 0) { std::printf("0099f400_TEST FAIL\n"); return 1; }
    std::printf("0099f400_TEST PASS\n"); return 0;
}