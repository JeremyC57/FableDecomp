#include <cstdio>

// Self-contained behavior model of CTCEmoteIcon::GetActionName.
class CCharString { public:
    const char* p;
    int n;
    CCharString(const char* s, int len) : p(s), n(len) {}
};

class CTCEmoteIcon {
public:
    CCharString GetClassTypeName();
};

// same literal address as the real function pushes
static const char* const LIT = (const char*)0x123b314;

CCharString CTCEmoteIcon::GetClassTypeName()
{
    return CCharString(LIT, -1);
}

int main()
{
    CTCEmoteIcon obj;
    CCharString r = obj.GetClassTypeName();
    // The function must construct a CCharString from the fixed literal ptr with len -1.
    if (r.p == (const char*)0x123b314 && r.n == -1) {
        printf("0071b0e0_TEST PASS\n");
    } else {
        printf("0071b0e0_TEST FAIL p=%p n=%d\n", (void*)r.p, r.n);
    }
    return 0;
}