// Behaviour test for CTCWeapon::ResetAiming @ 0x005da4e0 (links the row object): AimingTime +0x14 = -1, AimingTimeOverride +0x18 = false,
// every other byte untouched.
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_RESETAIMING
#include "engine/CTCWeapon.h"
#include <cstdio>
#include <cstring>

int main()
{
    unsigned char buf[sizeof(CTCWeapon) + 8];
    std::memset(buf, 0x5A, sizeof(buf));
    CTCWeapon* w = (CTCWeapon*)buf;
    w->AimingTime = 12345;
    w->AimingTimeOverride = true;
    w->ResetAiming();
    if (w->AimingTime != -1) { std::printf("BAD AimingTime=%ld\n", w->AimingTime); return 1; }
    if (w->AimingTimeOverride) { std::printf("BAD AimingTimeOverride\n"); return 1; }
    for (int i = 0; i < (int)sizeof(buf); ++i) {
        if (i >= 0x14 && i < 0x19) continue;
        if (buf[i] != 0x5A) { std::printf("BAD byte %d\n", i); return 1; }
    }
    std::printf("OK_0x005da4e0 AimingTime=%ld AimingTimeOverride=%d\n", w->AimingTime, (int)w->AimingTimeOverride);
    return 0;
}
