#include <cstdio>

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
    char* m_data;
    CCharString(const char* s, long n) { m_data = (char*)s; (void)n; }
};

class CTCTavernGameSpotTheAddition {
public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameSpotTheAddition::GetClassTypeName() {
    return CCharString("CTCTavernGameSpotTheAddition", -1);
}

int main() {
    CTCTavernGameSpotTheAddition a;
    CCharString r = a.GetClassTypeName();
    if (RetailTextEquals(r.m_data, "CTCTavernGameSpotTheAddition")) {
        printf("OK ptr=%p\n", (void*)r.m_data);
    } else {
        printf("FAIL\n");
        return 1;
    }
    return 0;
}