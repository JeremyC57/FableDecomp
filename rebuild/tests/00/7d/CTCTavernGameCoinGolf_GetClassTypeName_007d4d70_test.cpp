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
    int d;
    const char* gotLit;
    int gotLen;
    CCharString(const char* lit, long len) {
        gotLit = lit;
        gotLen = len;
        d = 0x1234;
    }
};

class CTCTavernGameCoinGolf { public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameCoinGolf::GetClassTypeName() {
    return CCharString("CTCTavernGameCoinGolf", -1);
}

int main() {
    CTCTavernGameCoinGolf obj;
    CCharString r = obj.GetClassTypeName();
    if (RetailTextEquals(r.gotLit, "CTCTavernGameCoinGolf") && r.gotLen == -1) {
        printf("007d4d70_TEST PASS\n");
    } else {
        printf("FAIL lit=%p len=%d\n", r.gotLit, r.gotLen);
    }
    return 0;
}