// Behaviour test for 0x00460652 CSpecialAbilitiesDivineWrathDef::Copy (NF425): links the row object; CSpecialAbilitiesDivineWrathDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CSpecialAbilitiesDivineWrathDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CSpecialAbilitiesDivineWrathDef& operator=(const CSpecialAbilitiesDivineWrathDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CSpecialAbilitiesDivineWrathDef& CSpecialAbilitiesDivineWrathDef::operator=(const CSpecialAbilitiesDivineWrathDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CSpecialAbilitiesDivineWrathDef* dst = (CSpecialAbilitiesDivineWrathDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CSpecialAbilitiesDivineWrathDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_00460652 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_00460652\n");
    return 0;
}
