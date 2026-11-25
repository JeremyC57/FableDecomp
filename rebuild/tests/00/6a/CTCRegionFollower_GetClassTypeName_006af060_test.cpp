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
    CCharString(const char* s, long len) { p = (char*)s; last_len = len; }
    static int last_len;
    static const char* last_s;
};
int CCharString::last_len = 0;
const char* CCharString::last_s = 0;

// redefine ctor to capture (single TU test)
class CTCRegionFollower {
    int dummy;
public:
    CCharString GetClassTypeName();
};

CCharString CTCRegionFollower::GetClassTypeName()
{
    return CCharString("CTCRegionFollower", -1);
}

int main()
{
    CTCRegionFollower a;
    CCharString r = a.GetClassTypeName();
    bool ok = (RetailTextEquals(r.p, "CTCRegionFollower"));
    if (ok) printf("006af060_TEST PASS\n");
    else printf("FAIL p=%p\n", r.p);
    return 0;
}