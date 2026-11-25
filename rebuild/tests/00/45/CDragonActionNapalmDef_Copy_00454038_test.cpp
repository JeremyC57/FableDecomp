// Behaviour test for 0x00454038 CDragonActionNapalmDef::Copy (NF425): links the row object; CDragonActionNapalmDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CDragonActionNapalmDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CDragonActionNapalmDef& operator=(const CDragonActionNapalmDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CDragonActionNapalmDef& CDragonActionNapalmDef::operator=(const CDragonActionNapalmDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CDragonActionNapalmDef* dst = (CDragonActionNapalmDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CDragonActionNapalmDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("CDragonActionNapalmDef_00454038_TEST FAIL calls=%d\n", g_calls);
        return 1;
    }
    printf("CDragonActionNapalmDef_00454038_TEST PASS\n");
    return 0;
}
