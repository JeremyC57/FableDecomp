#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString { public:
    char* p;
    CCharString(const char* s, long n) { p = (char*)s; last_s = s; last_n = n; }
    static const char* last_s;
    static int last_n;
};
const char* CCharString::last_s = 0;
int CCharString::last_n = 0;

class CTCDummyVillager { public:
    CCharString GetClassTypeName();
};

CCharString CTCDummyVillager::GetClassTypeName()
{
    return CCharString("CTCDummyVillager", -1);
}

int main()
{
    CTCDummyVillager obj;
    CCharString r = obj.GetClassTypeName();
    if (RetailTextEquals(r.p, "CTCDummyVillager") &&
        RetailTextEquals(CCharString::last_s, "CTCDummyVillager") &&
        CCharString::last_n == -1) {
        printf("00730400_TEST PASS\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}