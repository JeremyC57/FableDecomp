// Behaviour test for 00897220 CGameScriptInterface::IsObjectInThingsPossession: index gate, null thing, five component stores.
#include <cstdio>
struct Thing { bool has[5]; long count[5]; };   // 0x11 inv, 0x46 carrying, 0x13, 0x12, 0x3C house
static bool InPossession(long index, const Thing* thing)
{
    if (index > 0) {
        if (thing) {
            if ((thing->has[0] && thing->count[0] > 0) || (thing->has[1] && thing->count[1] != 0)
                || (thing->has[2] && thing->count[2] > 0) || (thing->has[3] && thing->count[3] > 0)
                || (thing->has[4] && thing->count[4] != 0))
                return true;
        }
    }
    return false;
}
int main()
{
    Thing t = { { false, false, false, false, false }, { 0, 0, 0, 0, 0 } };
    bool ok = !InPossession(3, &t) && !InPossession(3, 0);
    t.has[3] = true; t.count[3] = 2; ok = ok && InPossession(3, &t) && !InPossession(0, &t);
    t.has[3] = false; t.has[4] = true; t.count[4] = 1; ok = ok && InPossession(1, &t);
    t.has[4] = false; t.has[0] = true; t.count[0] = 0; ok = ok && !InPossession(1, &t);
    if (!ok) { std::printf("00897220_TEST FAIL\n"); return 1; }
    std::printf("00897220_TEST PASS\n");
    return 0;
}
