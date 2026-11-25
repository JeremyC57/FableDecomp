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
    CCharString(const char* s, long n) { n_ = n; s_ = s; }
    int n_;
    const char* s_;
    void* pad;
};

class CTCActionUseBuyOrSteal {
public:
    CCharString GetClassTypeName();
};

CCharString CTCActionUseBuyOrSteal::GetClassTypeName()
{
    return CCharString("CTCActionUseBuyOrSteal", -1);
}

int main()
{
    CTCActionUseBuyOrSteal obj;
    CCharString r = obj.GetClassTypeName();
    if (r.n_ == -1 && RetailTextEquals(r.s_, "CTCActionUseBuyOrSteal")) {
        printf("0072a710_TEST PASS\n");
    } else {
        printf("FAIL n=%d s=%p\n", r.n_, r.s_);
    }
    return 0;
}