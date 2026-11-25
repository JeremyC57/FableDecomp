// Behaviour test for 0x004E074B CTimeAppearanceFadeDef::Copy (NF425): links the row object; CTimeAppearanceFadeDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CTimeAppearanceFadeDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CTimeAppearanceFadeDef& operator=(const CTimeAppearanceFadeDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CTimeAppearanceFadeDef& CTimeAppearanceFadeDef::operator=(const CTimeAppearanceFadeDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CTimeAppearanceFadeDef* dst = (CTimeAppearanceFadeDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CTimeAppearanceFadeDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_004e074b calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_004e074b\n");
    return 0;
}
