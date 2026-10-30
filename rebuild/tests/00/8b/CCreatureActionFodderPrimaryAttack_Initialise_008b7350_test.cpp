// behaviour test for 0x008B7350 CCreatureAction_FodderPrimaryAttack::Initialise (wasp-vs-wasp animation pick)
#include <cstdio>
#include <cstring>
class CThing { public: long type; unsigned long flags6c; unsigned char Dead : 1; bool IsDead() const { return Dead; } };
static const char* Pick(const CThing* self, const CThing* target)
{
    if (self->type == 0x100 && target && !target->IsDead() && (target->flags6c & 8)) {
        if (target->type == 0x100) return "WASP_VS_WASP";
        return "STRIKE_MEDIUM_FRONT";
    }
    return "STRIKE_MEDIUM_FRONT";
}
int main()
{
    CThing wasp = { 0x100, 8, 0 }, wasp2 = { 0x100, 8, 0 }, deadWasp = { 0x100, 8, 1 }, plain = { 0x100, 0, 0 }, hero = { 1, 8, 0 };
    bool ok = !std::strcmp(Pick(&wasp, &wasp2), "WASP_VS_WASP") && !std::strcmp(Pick(&wasp, &deadWasp), "STRIKE_MEDIUM_FRONT")
        && !std::strcmp(Pick(&wasp, &plain), "STRIKE_MEDIUM_FRONT") && !std::strcmp(Pick(&hero, &wasp), "STRIKE_MEDIUM_FRONT")
        && !std::strcmp(Pick(&wasp, 0), "STRIKE_MEDIUM_FRONT");
    CThing flaggedHero = { 1, 8, 0 };
    ok = ok && !std::strcmp(Pick(&wasp, &flaggedHero), "STRIKE_MEDIUM_FRONT");
    std::printf(ok ? "008b7350_TEST PASS\n" : "008b7350_TEST FAIL\n");
    return ok ? 0 : 1;
}