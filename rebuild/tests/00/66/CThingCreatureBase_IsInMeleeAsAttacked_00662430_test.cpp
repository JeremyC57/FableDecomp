// Test for 0x00662430 CThingCreatureBase::IsInMeleeAsAttacked: links the source object; GetWorld (stubbed) hands back a world whose
// +0x4C combat manager records the creature it was asked about and answers; the answer comes back unchanged.
#include <stdio.h>
#include <string.h>
class CThingCreatureBase;
struct CWorld;
struct CCombatManager {
    bool IsAttacked(const CThingCreatureBase& creature) const;
};
static const void* g_asked;
static bool g_answer;
static int g_worldCalls;
bool CCombatManager::IsAttacked(const CThingCreatureBase& creature) const { g_asked = &creature; return g_answer; }
static unsigned char g_world[0x50];
static CCombatManager g_cm;
class CThing {
protected:
    CWorld& GetWorld();
};
CWorld& CThing::GetWorld() { ++g_worldCalls; return *(CWorld*)g_world; }
class CThingCreatureBase : public CThing {
public:
    int dummy;
    bool IsInMeleeAsAttacked() const;
};
int main()
{
    CCombatManager* p = &g_cm; memcpy(g_world + 0x4c, &p, 4);
    CThingCreatureBase c;
    g_answer = true;
    bool ok = c.IsInMeleeAsAttacked() == true && g_asked == &c;
    g_answer = false; g_asked = 0;
    ok = ok && c.IsInMeleeAsAttacked() == false && g_asked == &c && g_worldCalls == 2;
    if (!ok) { puts("00662430_TEST FAIL"); return 1; }
    puts("00662430_TEST PASS");
    return 0;
}
