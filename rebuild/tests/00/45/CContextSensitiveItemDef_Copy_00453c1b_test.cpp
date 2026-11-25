// Behaviour test for 0x00453C1B CContextSensitiveItemDef::Copy (NF425): links the row object; CContextSensitiveItemDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CContextSensitiveItemDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CContextSensitiveItemDef& operator=(const CContextSensitiveItemDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CContextSensitiveItemDef& CContextSensitiveItemDef::operator=(const CContextSensitiveItemDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CContextSensitiveItemDef* dst = (CContextSensitiveItemDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CContextSensitiveItemDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("CContextSensitiveItemDef_00453c1b_TEST FAIL calls=%d\n", g_calls);
        return 1;
    }
    printf("CContextSensitiveItemDef_00453c1b_TEST PASS\n");
    return 0;
}
