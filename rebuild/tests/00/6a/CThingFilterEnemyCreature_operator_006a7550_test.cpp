// Behaviour test for 0x006A7550 CThingFilter_EnemyCreature::operator()(const CThing&) const, on the shared headers (links the row object):
// the filter asks CTCEnemy::AreThingsEnemies(tested thing, source thing) and returns its answer.
#include <stdio.h>
#include "engine/CThingFilter_EnemyCreature.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#include "engine/CTCEnemy.h"
static const CThing* g_tested = 0;
static const CThing* g_source = 0;
bool FABLE_FASTCALL CTCEnemy::AreThingsEnemies(const CThing& thing1, const CThing& thing2)
{
    g_tested = &thing1;
    g_source = &thing2;
    return &thing1 != &thing2;
}
CThingFilter_EnemyCreature::CThingFilter_EnemyCreature(const CThing& p0) : SourceThing(p0) { }
static long g_storage[4];
int main()
{
    const CThing& a = *(const CThing*)&g_storage[0];
    const CThing& b = *(const CThing*)&g_storage[2];
    CThingFilter_EnemyCreature filter(b);
    bool enemy = filter(a);
    if (!enemy || g_tested != &a || g_source != &b) { printf("FAIL\n"); return 1; }
    bool self = filter(b);
    if (self || g_tested != &b || g_source != &b) { printf("FAIL\n"); return 1; }
    printf("LV_OK_006a7550\n");
    return 0;
}
