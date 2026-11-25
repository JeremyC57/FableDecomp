// Behaviour test for 0x004E6F06 CHeroMorphDef::Copy (NF425): links the row object; CHeroMorphDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CHeroMorphDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CHeroMorphDef& operator=(const CHeroMorphDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CHeroMorphDef& CHeroMorphDef::operator=(const CHeroMorphDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CHeroMorphDef* dst = (CHeroMorphDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CHeroMorphDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_004e6f06 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_004e6f06\n");
    return 0;
}
