// test for 00404310 CCharString::GetCRC
#include <stdio.h>
#include <string.h>
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
class CCharStringData { public:
    char*         PData;                                 // +0x00 CBasicString<char>::PData
    unsigned long StringLength;                          // +0x04
};
extern const char g_EmptyString[];                       // 0x0129AAF4
class CCharString {
public:
    CCharStringData* PStringData;
    const char* GetPString() const { return PStringData == 0 ? g_EmptyString : PStringData->PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    unsigned long GetCRC() const;
};
unsigned long CCharString::GetCRC() const
{
    return CCRC::Calc(0, GetPString(), GetLength());
}

const char g_EmptyString[] = "";
static const void* g_data; static unsigned long g_len, g_seed;
unsigned long __fastcall CCRC::Calc(unsigned long seed, const void* data, unsigned long length) { g_seed = seed; g_data = data; g_len = length; return 0x1234 + length; }
int main()
{
    char text[] = "hello";
    CCharStringData d; d.PData = text; d.StringLength = 5;
    CCharString s; s.PStringData = &d;
    int ok = s.GetCRC() == 0x1239 && g_data == text && g_len == 5 && g_seed == 0;
    CCharString e; e.PStringData = 0; g_seed = 9;
    ok = ok && e.GetCRC() == 0x1234 && g_data == g_EmptyString && g_len == 0 && g_seed == 0;
    printf(ok ? "00404310_TEST PASS\n" : "00404310_TEST FAIL\n");
    return 0;
}