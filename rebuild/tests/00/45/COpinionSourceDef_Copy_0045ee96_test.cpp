// Behaviour test for 0x0045EE96 COpinionSourceDef::Copy (NF425): links the row object; COpinionSourceDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class COpinionSourceDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    COpinionSourceDef& operator=(const COpinionSourceDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
COpinionSourceDef& COpinionSourceDef::operator=(const COpinionSourceDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    COpinionSourceDef* dst = (COpinionSourceDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->COpinionSourceDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_0045ee96 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_0045ee96\n");
    return 0;
}
