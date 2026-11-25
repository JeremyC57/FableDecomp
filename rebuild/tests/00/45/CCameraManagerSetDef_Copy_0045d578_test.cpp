// Behaviour test for 0x0045D578 CCameraManagerSetDef::Copy (NF425): links the row object; CCameraManagerSetDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CCameraManagerSetDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CCameraManagerSetDef& operator=(const CCameraManagerSetDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CCameraManagerSetDef& CCameraManagerSetDef::operator=(const CCameraManagerSetDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CCameraManagerSetDef* dst = (CCameraManagerSetDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CCameraManagerSetDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_0045d578 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_0045d578\n");
    return 0;
}
