// Behaviour test for CTCWeapon::IsAimedForLongEnough(float) const @ 0x005da490 (links the row object): the override answers true at
// once; otherwise AimingTime / CMainGameComponent::ConstantFPS must be strictly greater than the argument.
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_AIMING
#include "engine/CTCWeapon.h"
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CMainGameComponent.h"
#include <cstdio>
#include <cstring>

long CMainGameComponent::ConstantFPS = 100;

int main()
{
    unsigned char buf[sizeof(CTCWeapon)];
    std::memset(buf, 0, sizeof(buf));
    CTCWeapon* w = (CTCWeapon*)buf;
    w->AimingTime = 500;              // 500 / 100 = 5.0
    w->AimingTimeOverride = false;
    if (!w->IsAimedForLongEnough(4.0f)) { std::printf("FAIL1\n"); return 1; }
    if (w->IsAimedForLongEnough(6.0f)) { std::printf("FAIL2\n"); return 1; }
    if (w->IsAimedForLongEnough(5.0f)) { std::printf("FAIL3\n"); return 1; }    // strict
    w->AimingTimeOverride = true;
    if (!w->IsAimedForLongEnough(1000.0f)) { std::printf("FAIL4\n"); return 1; }
    std::printf("OK_0x005da490\n");
    return 0;
}
