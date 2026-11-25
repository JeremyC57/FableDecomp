// Behaviour test for 0x004643CA CThingBuildingDef::Copy (NF425): links the row object; CThingBuildingDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CThingBuildingDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CThingBuildingDef& operator=(const CThingBuildingDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CThingBuildingDef& CThingBuildingDef::operator=(const CThingBuildingDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CThingBuildingDef* dst = (CThingBuildingDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CThingBuildingDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_004643ca calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_004643ca\n");
    return 0;
}
