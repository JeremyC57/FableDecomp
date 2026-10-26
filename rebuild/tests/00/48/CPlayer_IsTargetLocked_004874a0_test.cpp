// behaviour test for 0x004874A0
#include <stdio.h>
#include <string.h>
class CPlayer { public: unsigned char _pad[0x20a]; bool ZTargeting; bool ProjectileTargetLocked; bool RightTriggerTargetLocked; };
struct CPlayer_Methods : CPlayer {
    bool IsTargetLocked() const;
};
bool CPlayer_Methods::IsTargetLocked() const
{
    return ZTargeting ? true : ProjectileTargetLocked ? true : RightTriggerTargetLocked ? true : false;
}

int main()
{
    static CPlayer_Methods p; memset(&p, 0, sizeof(p));
    int ok = !p.IsTargetLocked();
    p.ZTargeting = true; ok &= p.IsTargetLocked(); p.ZTargeting = false;
    p.ProjectileTargetLocked = true; ok &= p.IsTargetLocked(); p.ProjectileTargetLocked = false;
    p.RightTriggerTargetLocked = true; ok &= p.IsTargetLocked();
    printf(ok ? "004874a0_TEST PASS\n" : "004874a0_TEST FAIL\n");
    return ok ? 0 : 1;
}