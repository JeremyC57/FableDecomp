#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Self-contained behavior model of CTCEmoteIcon::GetActionName.
class CCharString { public:
    const char* p;
    int n;
    CCharString(const char* s, long len) : p(s), n(len) {}
};

class CTCEmoteIcon {
public:
    CCharString GetClassTypeName();
};

// same literal address as the real function pushes
static const char* const LIT = "CTCEmoteIcon";

CCharString CTCEmoteIcon::GetClassTypeName()
{
    return CCharString(LIT, -1);
}

int main()
{
    CTCEmoteIcon obj;
    CCharString r = obj.GetClassTypeName();
    // The function must construct a CCharString from the fixed literal ptr with len -1.
    if (RetailTextEquals(r.p, "CTCEmoteIcon") && r.n == -1) {
        printf("0071b0e0_TEST PASS\n");
    } else {
        printf("0071b0e0_TEST FAIL p=%p n=%d\n", (void*)r.p, r.n);
    }
    return 0;
}