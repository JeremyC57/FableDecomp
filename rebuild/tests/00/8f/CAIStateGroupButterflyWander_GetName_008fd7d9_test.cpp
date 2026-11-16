#include <stdio.h>

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
    if (g_lastText == (const char*)0x0128954C && g_lastLen == -1)
        printf("008fd7d9_TEST PASS\n");
    else
        printf("FAIL text=%p len=%ld\n", (const void*)g_lastText, g_lastLen);
    return 0;
}
