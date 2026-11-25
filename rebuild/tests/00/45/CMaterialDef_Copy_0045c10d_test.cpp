// Behaviour test for 0x0045C10D CMaterialDef::Copy (NF425): links the row object; CMaterialDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CMaterialDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CMaterialDef& operator=(const CMaterialDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CMaterialDef& CMaterialDef::operator=(const CMaterialDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CMaterialDef* dst = (CMaterialDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CMaterialDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_0045c10d calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_0045c10d\n");
    return 0;
}
