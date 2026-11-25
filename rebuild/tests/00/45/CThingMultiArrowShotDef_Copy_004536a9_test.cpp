// Behaviour test for 0x004536A9 CThingMultiArrowShotDef::Copy (NF425): links the row object; CThingMultiArrowShotDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CThingMultiArrowShotDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CThingMultiArrowShotDef& operator=(const CThingMultiArrowShotDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CThingMultiArrowShotDef& CThingMultiArrowShotDef::operator=(const CThingMultiArrowShotDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CThingMultiArrowShotDef* dst = (CThingMultiArrowShotDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CThingMultiArrowShotDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("CThingMultiArrowShotDef_004536a9_TEST FAIL calls=%d\n", g_calls);
        return 1;
    }
    printf("CThingMultiArrowShotDef_004536a9_TEST PASS\n");
    return 0;
}
