#include <cstdio>

class CCharString { public:
    char* p;
    CCharString(const char* s, long n) { p = (char*)s; last_n = n; }
    static int last_n;
};
int CCharString::last_n = 0;

const char kActionName_0123bffc[] = "DoCreatureAction";

class CTCSkeletalMorph { public:};

CCharString GetClassTypeName(CTCSkeletalMorph* self);

CCharString GetClassTypeName(CTCSkeletalMorph* self)
{
    return CCharString(kActionName_0123bffc, -1);
}

int main()
{
    CTCSkeletalMorph obj;
    CCharString r = GetClassTypeName(&obj);
    if (r.p == kActionName_0123bffc && CCharString::last_n == -1)
        std::printf("007874a0_TEST PASS\n");
    else
        std::printf("FAIL n=%d\n", CCharString::last_n);
    return 0;
}