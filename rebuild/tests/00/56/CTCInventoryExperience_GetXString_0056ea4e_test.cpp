#include <cstdio>
#include <cstring>
// Behaviour test for 0x0056EA4E: "" for a zero count, else GetXString() + the count as text.
class CWideString { public:
    char m_text[32];
    CWideString(const char* text) { std::strcpy(m_text, text); }
    CWideString(const CWideString& other) { std::strcpy(m_text, other.m_text); }
    ~CWideString() {}
};
static CWideString GFIntToWideString(long value) { char b[32]; std::sprintf(b, "%ld", value); return CWideString(b); }
static CWideString GetXString() { return CWideString("x"); }
static CWideString operator+(const CWideString& left, const CWideString& right)
{
    CWideString r(left); std::strcat(r.m_text, right.m_text); return r;
}
class CTCInventoryExperience { public:
    CWideString GetXString(long count);
};
CWideString CTCInventoryExperience::GetXString(long count)
{
    if (count == 0)
        return CWideString("");
    return ::GetXString() + GFIntToWideString(count);
}
int main()
{
    CTCInventoryExperience e;
    if (std::strcmp(e.GetXString(0).m_text, "") != 0) { std::printf("0056ea4e_TEST FAIL\n"); return 1; }
    if (std::strcmp(e.GetXString(12).m_text, "x12") != 0) { std::printf("0056ea4e_TEST FAIL\n"); return 1; }
    if (std::strcmp(e.GetXString(-3).m_text, "x-3") != 0) { std::printf("0056ea4e_TEST FAIL\n"); return 1; }
    std::printf("0056ea4e_TEST PASS\n");
    return 0;
}