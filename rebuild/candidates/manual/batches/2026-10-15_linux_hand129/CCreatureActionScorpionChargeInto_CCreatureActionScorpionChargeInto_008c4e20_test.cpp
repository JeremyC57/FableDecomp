// Behaviour test for 0x008C4E20 (see src/008c4e20.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("008c4e20_TEST FAIL\n"); return 1; } } while (0)
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

struct CPtr { void* item; CPtr() : item(0) {} };
struct CChargeInto : CCombatActionBase { int count; CPtr target;
    CChargeInto(void* owner, void* thing, int c) : CCombatActionBase(owner, thing, 150, GetSoundPair(owner, "SWORD_SWING")) { count = c; } };
int main()
{
    int o, t; CChargeInto a(&o, &t, 3);
    CHECK(a.actionCode == 150 && a.thing == &t && a.count == 3 && a.target.item == 0 && a.sound.LastSound == 11);
    std::printf("008c4e20_TEST PASS\n");
    return 0;
}
