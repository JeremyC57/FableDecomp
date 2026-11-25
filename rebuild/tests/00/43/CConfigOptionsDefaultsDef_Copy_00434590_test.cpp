// Behaviour test for 0x00434590 CConfigOptionsDefaultsDef::Copy (NF425): links the row object; CConfigOptionsDefaultsDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
#include <stdio.h>

class CDefClassBase;
class CConfigOptionsDefaultsDef
{
public:
    virtual void Copy(const CDefClassBase* other);   // defined in the row object
    CConfigOptionsDefaultsDef& operator=(const CConfigOptionsDefaultsDef& other);
    char pad[64];
};

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
CConfigOptionsDefaultsDef& CConfigOptionsDefaultsDef::operator=(const CConfigOptionsDefaultsDef& other) { g_this = this; g_other = &other; ++g_calls; return *this; }

static long g_dst[32];
static long g_src[32];

int main(void)
{
    CConfigOptionsDefaultsDef* dst = (CConfigOptionsDefaultsDef*)g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    dst->CConfigOptionsDefaultsDef::Copy(src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_00434590 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_00434590\n");
    return 0;
}
