// Behaviour test for CTCDayOrNightOnlySupport::GetSectionNameSuffix(const CCharString&) @ 0x00702AC0: type 1 appends
// "DayOnly", type 2 appends "NightOnly", anything else gives an empty string.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharString { public:
    char m_text[64];
    CCharString(const char* text, long length)
    {
        if (length < 0) length = (long)std::strlen(text);
        std::memcpy(m_text, text, length); m_text[length] = 0;
    }
    CCharString(const CCharString& other) { std::strcpy(m_text, other.m_text); }
    ~CCharString() { }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right)
{
    CCharString result(left);
    std::strcat(result.m_text, right.m_text);
    return result;
}
CCharString g_DayOnlySuffix("DayOnly", -1);
CCharString g_NightOnlySuffix("NightOnly", -1);
class CTCDayOrNightOnlySupport { public:
    virtual void v00() { }
    unsigned char _base[0x8];
    long          DayNightOrBoth;
    CCharString GetSectionNameSuffix(const CCharString& name) const;
};
CCharString CTCDayOrNightOnlySupport::GetSectionNameSuffix(const CCharString& name) const
{
    switch (DayNightOrBoth) {
    case 1:
        return name + g_DayOnlySuffix;
    case 2:
        return name + g_NightOnlySuffix;
    }
    return CCharString("", -1);
}
int main()
{
    CTCDayOrNightOnlySupport s;
    CCharString name("Section", -1);
    s.DayNightOrBoth = 1; CCharString a = s.GetSectionNameSuffix(name);
    s.DayNightOrBoth = 2; CCharString b = s.GetSectionNameSuffix(name);
    s.DayNightOrBoth = 0; CCharString c = s.GetSectionNameSuffix(name);
    s.DayNightOrBoth = 3; CCharString d = s.GetSectionNameSuffix(name);
    if (std::strcmp(a.m_text, "SectionDayOnly") != 0 || std::strcmp(b.m_text, "SectionNightOnly") != 0
        || c.m_text[0] != 0 || d.m_text[0] != 0) { std::printf("00702ac0_TEST FAIL\n"); return 1; }
    std::printf("00702ac0_TEST PASS\n");
    return 0;
}