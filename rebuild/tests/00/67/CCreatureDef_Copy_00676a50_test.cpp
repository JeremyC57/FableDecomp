// Behaviour test for 0x00676A50 CCreatureDef::Copy (NF425): links the row object; CCreatureDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CCreatureDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CCreatureDef& operator=(const CCreatureDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CCreatureDef& CCreatureDef::operator=(const CCreatureDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CCreatureDef* dst = (CCreatureDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CCreatureDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_00676a50 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_00676a50\n");
    return 0;
}
