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
    static int last;
    CCharString(const char* s, long n);
};
int CCharString::last=0;
CCharString::CCharString(const char* s, long n){ p=(char*)s; last=n; }
class CTCWeaponTrail { public: CCharString GetClassTypeName(); };
int main(){
    CTCWeaponTrail a;
    CCharString r = a.GetClassTypeName();
    if(RetailTextEquals(r.p, "CTCWeaponTrail") && CCharString::last==-1) printf("00770930_TEST PASS\n");
    else printf("FAIL %p %d\n", (void*)r.p, CCharString::last);
    return 0;
}