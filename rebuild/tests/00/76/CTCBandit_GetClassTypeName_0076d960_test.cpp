#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int n) { p = (char*)s; last_n = n; }
    static int last_n;
};
int CCharString::last_n = 0;

const char kActionName[] = "DoCreatureAction";

struct CTCBandit {};

CCharString GetClassTypeName(CTCBandit* self);

CCharString GetClassTypeName(CTCBandit* self)
{
    return CCharString(kActionName, -1);
}

int main()
{
    CTCBandit obj;
    CCharString r = GetClassTypeName(&obj);
    if (r.p == kActionName && CCharString::last_n == -1)
        std::printf("0076d960_TEST PASS\n");
    else
        std::printf("FAIL n=%d\n", CCharString::last_n);
    return 0;
}