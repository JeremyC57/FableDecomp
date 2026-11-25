// Behaviour test for 0x004E1913 CBettingDef::Copy (NF425): links the row object; CBettingDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CBettingDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CBettingDef& operator=(const CBettingDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CBettingDef& CBettingDef::operator=(const CBettingDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CBettingDef* dst = (CBettingDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CBettingDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_004e1913 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_004e1913\n");
    return 0;
}
