#include <stdio.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Test for 0x008FD7D9 CAIStateGroup_ButterflyWander::GetName() (static __fastcall, hidden CCharString result in ecx).
// The row constructs the result from the retail literal address 0x0128954C ("SG_BUTTERFLY_WANDER") with length -1;
// the test supplies the CCharString ctor/dtor and checks the arguments the row passes.

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);
    ~CCharString();
};

static const char* g_lastText = 0;
static long g_lastLen = 0;

// Out-of-line ctor so GetName emits a real 'call' (matches retail).
CCharString::CCharString(const char* text, long length)
{
    g_lastText = text;
    g_lastLen = length;
    PStringData = 0;
}
CCharString::~CCharString() {}

class CAIStateGroup_ButterflyWander
{
public:
    static CCharString __fastcall GetName();
};

int main()
{
    {
        CCharString s = CAIStateGroup_ButterflyWander::GetName();
        (void)s;
    }
    if (RetailTextEquals(g_lastText, "SG_BUTTERFLY_WANDER") && g_lastLen == -1)
        printf("008fd7d9_TEST PASS\n");
    else
        printf("FAIL text=%p len=%ld\n", (const void*)g_lastText, g_lastLen);
    return 0;
}
