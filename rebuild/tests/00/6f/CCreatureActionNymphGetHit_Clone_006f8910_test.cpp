// Behaviour test for 0x006F8910 CCreatureAction_NymphGetHit::Clone (NF425): links the row object; operator new and the copy constructor are defined here and
// record their arguments: one allocation of 0x154 bytes, one copy construction into that block from `*this`, the block returned;
// and 0 with no construction when the allocation fails.
#include <stdio.h>
#include <stddef.h>

class CCreatureActionBase;
class CCreatureAction_NymphGetHit
{
    friend int main(void);
public:
    CCreatureAction_NymphGetHit(const CCreatureAction_NymphGetHit& other);
public:
    virtual CCreatureActionBase* Clone() const;   // defined in the row object
};

static unsigned int g_size = 0;
static int g_news = 0, g_ctors = 0, g_fail = 0;
static const void* g_this = 0;
static const void* g_other = 0;
static long g_block[0x154 / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }
CCreatureAction_NymphGetHit::CCreatureAction_NymphGetHit(const CCreatureAction_NymphGetHit& other) { g_this = this; g_other = &other; ++g_ctors; }

static long g_src[64];

int main(void)
{
    const CCreatureAction_NymphGetHit* src = (const CCreatureAction_NymphGetHit*)g_src;
    CCreatureActionBase* r = src->CCreatureAction_NymphGetHit::Clone();
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0x154 && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)g_src;
    g_fail = 1;
    r = src->CCreatureAction_NymphGetHit::Clone();
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("006f8910_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("006f8910_TEST PASS\n");
    return 0;
}
