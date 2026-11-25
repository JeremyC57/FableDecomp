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
    char* m_p;
    long  m_n;
    CCharString(const char* s, long n) { m_p = (char*)s; m_n = n; }
};

class CEngineComponent {
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineComponent::GetConsoleEnableFunctionName()
{
    return CCharString("", -1);
}

int main()
{
    CEngineComponent c;
    CCharString r = c.GetConsoleEnableFunctionName();
    if (RetailTextEquals(r.m_p, "") && r.m_n == -1) {
        printf("GETCONSOLEENABLE_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}