// Behaviour test for 0x006AF520 CMessageEvent::GetPTarget<CThingCreatureBase>: the target only when all four creature type bits are set (standalone copy of the logic)
#include <cstdio>
struct Thing { unsigned long ThingTypeFlags; };
struct Msg { Thing* PTarget; Thing* GetPTarget() const { Thing* r = 0; if (PTarget) { if (!(~PTarget->ThingTypeFlags & 0x0F)) r = PTarget; } return r; } };

int main()
{
    Thing t; Msg m; m.PTarget = 0; if (m.GetPTarget()) { std::printf("006af520_TEST FAIL\n"); return 1; }
    m.PTarget = &t; t.ThingTypeFlags = 0x1F; if (m.GetPTarget() != &t) { std::printf("006af520_TEST FAIL\n"); return 1; }
    t.ThingTypeFlags = 0x07; if (m.GetPTarget()) { std::printf("006af520_TEST FAIL\n"); return 1; }
    std::printf("006af520_TEST PASS\n");
    return 0;
}