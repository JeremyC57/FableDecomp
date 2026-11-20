// Behaviour test for 0x004C6060 CBulletTimeManager::IsTimeForFullUpdate() const: links the row object; supplies the two callees
// (GetBulletTimeSlowdownMultiplier 0x004C5FA0 and the static CWorld::GetFrame 0x0049D870) through the shared headers.
// (NF108 2026-11-19: rewritten for the const member; the old test called the extern "C" __fastcall stand-in.)
#include <cstdio>
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_SLOWDOWNMULTIPLIER
#include "engine/CBulletTimeManager.h"
#include "engine/CWorld.h"

static int g_div = 3;
static int g_num = 9;

float CBulletTimeManager::GetBulletTimeSlowdownMultiplier() const { return (float)g_div; }
long __fastcall CWorld::GetFrame() { return g_num; }

int main()
{
    static unsigned char storage[sizeof(CBulletTimeManager)];
    const CBulletTimeManager* manager = reinterpret_cast<const CBulletTimeManager*>(storage);
    g_div = 3; g_num = 9;
    bool a = manager->IsTimeForFullUpdate();   // 9%3==0 -> true
    g_div = 4; g_num = 9;
    bool b = manager->IsTimeForFullUpdate();   // 9%4==1 -> false
    if (a && !b) printf("PARITY_OK\n");
    else printf("PARITY_FAIL a=%d b=%d\n", (int)a, (int)b);
    return 0;
}
