// Behaviour test for 0x004D4369 (CTCDExperienceOrb class-type name, stand-in self FableStatGetClassTypeName_5): links the row object
// (declared here, not defined) and asserts what it hands to the CCharString constructor: the retail text "CTCDExperienceOrb", length
// -1, one call, constructed into the buffer it was given, which it returns.
#include <cstdio>
#include <string.h>

class CCharString { public:
    char* p;
    void Init(const char* s, int n);   // the row's callee (0x0099EBF0), defined here
};

CCharString* __fastcall FableStatGetClassTypeName_5(CCharString* retbuf);   // definition linked from the row object

static int g_ctor_calls = 0;
static const char* g_ctor_s = 0;
static int g_ctor_n = 0;
static CCharString* g_ctor_self = 0;

void CCharString::Init(const char* s, int n)
{
    g_ctor_calls++;
    g_ctor_self = this;
    g_ctor_s = s;
    g_ctor_n = n;
    p = (char*)s;
}

int main()
{
    CCharString buf;
    buf.p = 0;
    CCharString* r = FableStatGetClassTypeName_5(&buf);

    int ok = 1;
    if (r != &buf) ok = 0;                       /* returns the buffer ptr */
    if (g_ctor_calls != 1) ok = 0;               /* ctor called exactly once */
    if (g_ctor_self != &buf) ok = 0;             /* this == retbuf */
    if (g_ctor_s == 0 || strcmp(g_ctor_s, "CTCDExperienceOrb") != 0) ok = 0;   /* retail's text */
    if (g_ctor_n != -1) ok = 0;                  /* length arg -1 */

    if (ok) { printf("OK_004d4369\n"); return 0; }
    printf("FAIL_004d4369 text=%s\n", g_ctor_s ? g_ctor_s : "(null)");
    return 1;
}
