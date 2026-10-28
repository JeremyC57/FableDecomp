// behaviour test for 0x00842fd0 CCreatureAction_MagicBlast::PerformAction():
// with an enemy set, look up the "SHOT_SUPER_FIRE" def index; when it is positive, fire that shot from the creature with
// the creature's shot accuracy (as a float) and unit speed / damage scales
#include <cstdio>
#include <cstring>
static long gIndex = 0; static int gShots = 0; static float gAccuracy = 0; static long gShotIndex = 0; static int gLookups = 0;
static long GetDefGlobalIndexFromName(const char* name) { ++gLookups; return std::strcmp(name, "SHOT_SUPER_FIRE") ? -1 : gIndex; }
static void CreateShot(long index, float accuracy, float a, float b) { ++gShots; gShotIndex = index; gAccuracy = accuracy; (void)a; (void)b; }
struct Action {
    void* PEnemy; long accuracy;
    void PerformAction()
    {
        if (PEnemy) {
            long shotIndex = GetDefGlobalIndexFromName("SHOT_SUPER_FIRE");
            if (shotIndex > 0)
                CreateShot(shotIndex, (float)accuracy, 1.0f, 1.0f);
        }
    }
};
int main()
{
    int fails = 0; int enemy = 0;
    Action a; a.PEnemy = 0; a.accuracy = 80;
    a.PerformAction();
    if (gLookups != 0 || gShots != 0) fails |= 1;
    a.PEnemy = &enemy; gIndex = 0; a.PerformAction();
    if (gLookups != 1 || gShots != 0) fails |= 2;
    gIndex = 42; a.PerformAction();
    if (gShots != 1 || gShotIndex != 42 || gAccuracy != 80.0f) fails |= 4;
    if (fails) { std::printf("00842fd0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00842fd0_TEST PASS\n");
    return 0;
}