#include <stdio.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString {
public:
    CCharString(const char* s, long n);
    char* m_data;
    int m_n;
};

class CTCTavernGameCardPairs {
public:
    CCharString GetClassTypeName();
};

// Local stand-in for the real ctor at 0x99ebf0.
CCharString::CCharString(const char* s, long n)
{
    m_data = (char*)s;
    m_n = n;
}

CCharString CTCTavernGameCardPairs::GetClassTypeName()
{
    return CCharString("CTCTavernGameCardPairs", -1);
}

int main()
{
    CTCTavernGameCardPairs a;
    CCharString r = a.GetClassTypeName();
    if (RetailTextEquals(r.m_data, "CTCTavernGameCardPairs") && r.m_n == -1)
        printf("OK data=%p n=%d\n", r.m_data, r.m_n);
    else
        printf("FAIL\n");
    return 0;
}