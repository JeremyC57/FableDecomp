#include <stdio.h>
#include <string.h>

class CCharString { public:
    char* p;
    CCharString(const char* s, long len);
};

const char kActionNameLiteral[] = "DoCreatureAction";

// Model ctor: len==-1 means strlen; store pointer for verification.
CCharString::CCharString(const char* s, long len)
{
    p = (char*)s;
    (void)len;
}

struct CTCDegradableBarrier {
    char dummy;
    CCharString GetClassTypeName();
};

CCharString CTCDegradableBarrier::GetClassTypeName()
{
    return CCharString(kActionNameLiteral, -1);
}

int main()
{
    CTCDegradableBarrier obj;
    obj.dummy = 7;
    CCharString r = obj.GetClassTypeName();
    if (r.p == kActionNameLiteral && strcmp(r.p, "DoCreatureAction") == 0) {
        printf("00771100_TEST PASS\n");
    } else {
        printf("00771100_TEST FAIL\n");
    }
    return 0;
}