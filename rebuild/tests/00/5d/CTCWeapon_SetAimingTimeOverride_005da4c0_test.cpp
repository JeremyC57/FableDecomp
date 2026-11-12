// Behaviour test for CTCWeapon::SetAimingTimeOverride @ 0x005da4c0 (links the row object): AimingTimeOverride +0x18 = true, every
// other byte untouched.
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_AIMING
#include "engine/CTCWeapon.h"
#include <cstdio>
#include <cstring>

int main()
{
    unsigned char buf[sizeof(CTCWeapon) + 8];
    std::memset(buf, 0x5A, sizeof(buf));
    CTCWeapon* w = (CTCWeapon*)buf;
    w->AimingTimeOverride = false;
    w->SetAimingTimeOverride();
    if (!w->AimingTimeOverride || buf[0x18] != 1) { std::printf("BAD AimingTimeOverride\n"); return 1; }
    for (int i = 0; i < (int)sizeof(buf); ++i) {
        if (i == 0x18) continue;
        if (buf[i] != 0x5A) { std::printf("BAD byte %d\n", i); return 1; }
    }
    std::printf("OK_005da4c0\n");
    return 0;
}
