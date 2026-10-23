#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int n) { p = (char*)s; last_n = n; }
    static int last_n;
};
int CCharString::last_n = 0;

const char kActionName_01268848[] = "DoCreatureAction";

struct CActionDoCreatureAction {};

CCharString GetActionName(CActionDoCreatureAction* self);

CCharString GetActionName(CActionDoCreatureAction* self)
{
    return CCharString(kActionName_01268848, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = GetActionName(&obj);
    if (r.p == kActionName_01268848 && CCharString::last_n == -1)
        std::printf("00744110_TEST PASS\n");
    else
        std::printf("FAIL n=%d\n", CCharString::last_n);
    return 0;
}