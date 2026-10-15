// Behaviour test for 0x0095E870: IsStillValid of a boasting-crowd member.
#include <cstdio>
struct CThing { unsigned char flags91; void* crowd; bool IsDead() const { return (flags91 & 1) != 0; } };
struct CCrowd { CThing* target; long State; };
struct CIP { CThing* p; CThing* GetPItem() const { return p; } };
static long IsStillValid(CIP& PBoaster)
{
    if (PBoaster.GetPItem() != 0 && !PBoaster.GetPItem()->IsDead()) {
        CCrowd* crowd = (CCrowd*)PBoaster.GetPItem()->crowd;
        if (crowd) {
            if (crowd->State != 0 && crowd->State != 10) {
                CThing* target = crowd->target;
                if (target != 0 && !target->IsDead()) return 1;
            }
        }
    }
    return 0;
}
int main()
{
    CThing target = { 0, 0 };
    CCrowd crowd = { &target, 3 };
    CThing boaster = { 0, &crowd };
    CIP p = { &boaster };
    int bad = 0;
    if (IsStillValid(p) != 1) bad = 1;
    crowd.State = 10; if (IsStillValid(p) != 0) bad = 2;
    crowd.State = 0; if (IsStillValid(p) != 0) bad = 3;
    crowd.State = 2; target.flags91 = 1; if (IsStillValid(p) != 0) bad = 4;
    target.flags91 = 0; boaster.flags91 = 1; if (IsStillValid(p) != 0) bad = 5;
    boaster.flags91 = 0; boaster.crowd = 0; if (IsStillValid(p) != 0) bad = 6;
    p.p = 0; if (IsStillValid(p) != 0) bad = 7;
    if (bad) { std::printf("0095e870_TEST FAIL\n"); return 1; }
    std::printf("0095e870_TEST PASS\n");
    return 0;
}