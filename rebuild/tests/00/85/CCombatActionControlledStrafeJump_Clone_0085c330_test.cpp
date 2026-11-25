// Behaviour test for 0x0085C330 CCombatAction_ControlledStrafeJump::Clone (NF425): links the row object; operator new and the copy constructor are defined here and
// record their arguments: one allocation of 0x130 bytes, one copy construction into that block from `*this`, the block returned;
// and 0 with no construction when the allocation fails.
#include <stdio.h>
#include <stddef.h>

class CCreatureActionBase;
class CCombatAction_ControlledStrafeJump
{
    friend int main(void);
public:
    CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump& other);
public:
    virtual CCreatureActionBase* Clone() const;   // defined in the row object
};

static unsigned int g_size = 0;
static int g_news = 0, g_ctors = 0, g_fail = 0;
static const void* g_this = 0;
static const void* g_other = 0;
static long g_block[0x130 / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }
CCombatAction_ControlledStrafeJump::CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump& other) { g_this = this; g_other = &other; ++g_ctors; }

static long g_src[64];

int main(void)
{
    const CCombatAction_ControlledStrafeJump* src = (const CCombatAction_ControlledStrafeJump*)g_src;
    CCreatureActionBase* r = src->CCombatAction_ControlledStrafeJump::Clone();
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0x130 && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)g_src;
    g_fail = 1;
    r = src->CCombatAction_ControlledStrafeJump::Clone();
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("0085c330_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("0085c330_TEST PASS\n");
    return 0;
}
