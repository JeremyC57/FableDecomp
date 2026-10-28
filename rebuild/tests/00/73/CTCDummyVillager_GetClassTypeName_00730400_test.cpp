#include <cstdio>

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
    return CCharString((const char*)0x0123a680, -1);
}

int main()
{
    CTCDummyVillager obj;
    CCharString r = obj.GetClassTypeName();
    if (r.p == (char*)0x0123a680 &&
        CCharString::last_s == (const char*)0x0123a680 &&
        CCharString::last_n == -1) {
        printf("00730400_TEST PASS\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}