// Behaviour test for 0x009254C0 (see src/009254c0.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("009254c0_TEST FAIL\n"); return 1; } } while (0)
struct CSoundPair { int FirstSound; int LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
// Combat-action base model: records what the derived constructor handed it.
struct CCombatActionBase {
    void* owner; void* thing; int actionCode; CSoundPair sound; char animName[48];
    CCombatActionBase(void* o, int code, const CSoundPair& s) : owner(o), thing(0), actionCode(code), sound(s) { animName[0] = 0; }
    CCombatActionBase(void* o, void* t, int code, const CSoundPair& s) : owner(o), thing(t), actionCode(code), sound(s) { animName[0] = 0; }
    void SetAnimationName(const char* n) { std::strcpy(animName, n); }
};
static CSoundPair GetSoundPair(void* owner, const char* name)
{
    CSoundPair p; p.FirstSound = owner ? 1 : 0; p.LastSound = name ? (int)std::strlen(name) : -1; return p;
}

struct COwner { int pad[45]; int b4; };
struct CLowCharge : CCombatActionBase { int value; int ownerValue;
    CLowCharge(COwner* owner, void* thing, int v) : CCombatActionBase(owner, thing, 150, CSoundPair()), value(v), ownerValue(owner->b4) { SetAnimationName("ATTACK_LOW_CHARGE_LOOP"); } };
int main()
{
    COwner o; o.b4 = 77; int t; CLowCharge a(&o, &t, 5);
    CHECK(a.actionCode == 150 && a.value == 5 && a.ownerValue == 77 && std::strcmp(a.animName, "ATTACK_LOW_CHARGE_LOOP") == 0);
    std::printf("009254c0_TEST PASS\n");
    return 0;
}
